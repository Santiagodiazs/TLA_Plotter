#include "Frontend.h"

/* MODULE INTERNAL STATE */

static LexicalAnalyzer * _lexicalAnalyzer = NULL;
static Logger * _logger = NULL;


static Token ** _tokensList = NULL;
static int _tokensCount = 0;
static int _tokensCapacity = 0;

/** Agregar token a la lista para limpieza posterior */
void _addTokenToList(Token * token) {
	if (_tokensList == NULL) {
		_tokensCapacity = 10;
		_tokensList = calloc(_tokensCapacity, sizeof(Token*));
		_tokensCount = 0;
	}
	
	if (_tokensCount >= _tokensCapacity) {
		_tokensCapacity *= 2;
		_tokensList = realloc(_tokensList, _tokensCapacity * sizeof(Token*));
	}
	
	_tokensList[_tokensCount++] = token;
}

/** Limpiar todos los tokens de la lista */
void _cleanupTokens() {
	if (_logger != NULL) {
		logDebugging(_logger, "Cleaning up %d tokens...", _tokensCount);
	}
	for (int i = 0; i < _tokensCount; i++) {
		if (_tokensList[i] != NULL) {
			destroyToken(_tokensList[i]);
			_tokensList[i] = NULL; 
		}
	}
	free(_tokensList);
	_tokensList = NULL;
	_tokensCount = 0;
	_tokensCapacity = 0;
}


void _shutdownFrontendModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Frontend...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	
	_cleanupTokens();
	
	_lexicalAnalyzer = NULL;
}

ModuleDestructor initializeFrontendModule(LexicalAnalyzer * lexicalAnalyzer) {
	_lexicalAnalyzer = lexicalAnalyzer;
	_logger = createLogger("Frontend");
	return _shutdownFrontendModule;
}

/* IMPORTED FUNCTIONS */

extern bool flexHasBuffer(LexicalAnalyzer * lexicalAnalyzer);
extern FlexContext flexCurrentContext(LexicalAnalyzer * lexicalAnalyzer);
extern void flexEnterContext(LexicalAnalyzer * lexicalAnalyzer, FlexContext flexContext);
extern void flexLeaveContext(LexicalAnalyzer * lexicalAnalyzer);

/* PRIVATE FUNCTIONS */

static const char * _compilationStatusAsString(const CompilationStatus compilationStatus) {
	switch (compilationStatus) {
		case FAILED:
			return "FAILED";
		case IN_PROGRESS:
			return "IN_PROGRESS";
		case OUT_OF_MEMORY:
			return "OUT_OF_MEMORY";
		case SUCCEEDED:
			return "SUCCEEDED";
		default:
			return "UNKNOWN_ERROR";
	}
}

/* PUBLIC FUNCTIONS */

InputBuffer * createInputBuffer(LexicalAnalyzer * lexicalAnalyzer, const char * path) {
	InputBuffer * inputBuffer = (InputBuffer *) calloc(1, sizeof(InputBuffer));
	inputBuffer->bufferSizeInBytes = YY_BUF_SIZE;
	inputBuffer->file = fopen(path, "r");
	inputBuffer->lexicalAnalyzer = lexicalAnalyzer;
	inputBuffer->buffer = yy_create_buffer(inputBuffer->file, inputBuffer->bufferSizeInBytes, lexicalAnalyzer->scanner);
	return inputBuffer;
}

LexicalAnalyzer * createLexicalAnalyzer() {
	LexicalAnalyzer * lexicalAnalyzer = (LexicalAnalyzer *) calloc(1, sizeof(LexicalAnalyzer));
	lexicalAnalyzer->location = calloc(1, sizeof(YYLTYPE));
	lexicalAnalyzer->logger = createLogger("LexicalAnalyzer");
	yylex_init(&lexicalAnalyzer->scanner);
	lexicalAnalyzer->parser = yypstate_new();
	flexEnterContext(lexicalAnalyzer, 0);
	return lexicalAnalyzer;
}

Token * createToken(LexicalAnalyzer * lexicalAnalyzer, TokenLabel label) {
	Token * token = (Token *) calloc(1, sizeof(Token));
	token->context = flexCurrentContext(lexicalAnalyzer);
	token->label = label;
	token->length = yyget_leng(lexicalAnalyzer->scanner);
	token->lexeme = (char *) calloc(token->length + 1, sizeof(char));
	token->line = yyget_lineno(lexicalAnalyzer->scanner);
	token->semanticValue = (SemanticValue *) calloc(1, sizeof(SemanticValue));
	
	token->semanticValue->string = NULL;
	strncpy(token->lexeme, yyget_text(lexicalAnalyzer->scanner), token->length);
	return token;
}

FlexContext currentLexicalAnalyzerContext(LexicalAnalyzer * lexicalAnalyzer) {
	return flexCurrentContext(lexicalAnalyzer);
}

void destroyInputBuffer(InputBuffer * inputBuffer) {
	if (inputBuffer != NULL) {
		if (inputBuffer->buffer != NULL) {
			/**
			 * @todo
			 *	Because "yypop_buffer_state" in "popInputBuffer" deletes the
			 *	buffer, this line produces a double-free error. However,
			 *	commenting the line produces a memory-leak when a syntax error
			 *	takes place inside a secondary input buffer.
			 */
			// yy_delete_buffer((YY_BUFFER_STATE) inputBuffer->buffer, (yyscan_t) inputBuffer->lexicalAnalyzer->scanner);
			inputBuffer->buffer = NULL;
		}
		if (inputBuffer->file != NULL) {
			fclose(inputBuffer->file);
			inputBuffer->file = NULL;
		}
		inputBuffer->bufferSizeInBytes = 0;
		inputBuffer->lexicalAnalyzer = NULL;
		free(inputBuffer);
	}
}

void destroyLexicalAnalyzer(LexicalAnalyzer * lexicalAnalyzer) {
	if (lexicalAnalyzer != NULL) {
		if (lexicalAnalyzer->parser != NULL) {
			yypstate_delete((yypstate *) lexicalAnalyzer->parser);
			lexicalAnalyzer->parser = NULL;
		}
		if (lexicalAnalyzer->scanner != NULL) {
			yylex_destroy((yyscan_t) lexicalAnalyzer->scanner);
			lexicalAnalyzer->scanner = NULL;
		}
		if (lexicalAnalyzer->logger != NULL) {
			destroyLogger(lexicalAnalyzer->logger);
			lexicalAnalyzer->logger = NULL;
		}
		if (lexicalAnalyzer->location != NULL) {
			free(lexicalAnalyzer->location);
			lexicalAnalyzer->location = NULL;
		}
		free(lexicalAnalyzer);
	}
}

void destroyToken(Token * token) {
	if (token != NULL) {
		if (token->lexeme != NULL) {
			free(token->lexeme);
			token->lexeme = NULL;
		}
		if (token->semanticValue != NULL) {
			// Solo liberar string para tokens que realmente usan string
			if (token->label == IDENTIFIER || token->label == STRING) {
				if (token->semanticValue->string != NULL) {
					free(token->semanticValue->string);
					token->semanticValue->string = NULL;
				}
			}
			free(token->semanticValue);
			token->semanticValue = NULL;
		}
		free(token);
	}
}

void enterLexicalAnalyzerContext(LexicalAnalyzer * lexicalAnalyzer, FlexContext flexContext) {
	flexEnterContext(lexicalAnalyzer, flexContext);
}

CompilationStatus executeLexicalAnalysis(LexicalAnalyzer * lexicalAnalyzer) {
	return (CompilationStatus) yylex(
		NULL,
		(YYLTYPE *) lexicalAnalyzer->location,
		lexicalAnalyzer->scanner);
}

CompilationStatus executeSyntacticAnalysis() {
	logDebugging(_logger, "Parsing...");
	CompilationStatus status = IN_PROGRESS;
	CompilationStatus lastStatus = IN_PROGRESS;
	int errorCount = 0;
	const int MAX_ERRORS = 3; 
	
	while (status == IN_PROGRESS && errorCount < MAX_ERRORS) {
		status = executeLexicalAnalysis(_lexicalAnalyzer);
		
	
		if (status == FAILED) {
			logDebugging(_logger, "Parser returned FAILED, attempting to continue... (error %d/%d)", errorCount + 1, MAX_ERRORS);
			lastStatus = status;
			errorCount++;
			
			if (errorCount >= MAX_ERRORS) {
				logDebugging(_logger, "Too many parser errors (%d), stopping", errorCount);
				break;
			}
			
			// Continuar procesando
			status = IN_PROGRESS;
		} else if (status == 4) { // YYPUSH_MORE 
			// Continuar procesando
		} else if (status == SUCCEEDED) {
		logDebugging(_logger, "Reached EOF, parsing complete");
		int eofResult = yypush_parse(
			(yypstate *) _lexicalAnalyzer->parser,
			0, 
			NULL, 
			(YYLTYPE *) _lexicalAnalyzer->location);
		break;
	}
	}
	
	
	if (status == IN_PROGRESS && lastStatus != IN_PROGRESS) {
		status = lastStatus;
	}
	
	logDebugging(_logger, "Compilation status: %s.", _compilationStatusAsString(status));
	logDebugging(_logger, "Parsing is done.");
	
	
	_cleanupTokens();
	
	return status;
}

void leaveLexicalAnalyzerContext(LexicalAnalyzer * lexicalAnalyzer) {
	flexLeaveContext(lexicalAnalyzer);
}

bool popInputBuffer(LexicalAnalyzer * lexicalAnalyzer) {
	yypop_buffer_state((yyscan_t) lexicalAnalyzer->scanner);
	return flexHasBuffer(lexicalAnalyzer);
}

void pushInputBuffer(InputBuffer * inputBuffer) {
	yypush_buffer_state((YY_BUFFER_STATE) inputBuffer->buffer, (yyscan_t) inputBuffer->lexicalAnalyzer->scanner);
}

CompilationStatus pushToken(LexicalAnalyzer * lexicalAnalyzer, Token * token) {
	
	_addTokenToList(token);
	
	int result = yypush_parse(
		(yypstate *) lexicalAnalyzer->parser,
		token->label,
		token->semanticValue,
		(YYLTYPE *) lexicalAnalyzer->location);
	
	return (CompilationStatus) result;
}
