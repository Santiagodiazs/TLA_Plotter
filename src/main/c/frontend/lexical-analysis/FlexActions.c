#include "FlexActions.h"
#include "FlexScanner.h"
#include "FlexExport.h"

/* MODULE INTERNAL STATE */

static bool _logIgnoredLexemes = true;
static InputBuffer * _inputBuffer = NULL;
static LexicalAnalyzer * _lexicalAnalyzer = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownFlexActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: FlexActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	if (_inputBuffer != NULL) {
		destroyInputBuffer(_inputBuffer);
		_inputBuffer = NULL;
	}
	_lexicalAnalyzer = NULL;
}

ModuleDestructor initializeFlexActionsModule(LexicalAnalyzer * lexicalAnalyzer) {
	_inputBuffer = NULL;
	_lexicalAnalyzer = lexicalAnalyzer;
	_logger = createLogger("FlexActions");
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	return _shutdownFlexActionsModule;
}

/* PRIVATE FUNCTIONS */

static void _logTokenAction(const char * actionName, Token * token);

/**
 * Logs a lexical-analyzer action over a token in DEBUGGING level.
 */
static void _logTokenAction(const char * actionName, Token * token) {
	char * _lexeme = escape(token->lexeme);
	logDebugging(_logger, WARNING_COLOR "%s" DEFAULT_COLOR ": Token(context=%d, label=%d, length=%d, lexeme=%s\"%s\"%s, line=%d, semanticValue=%p)",
		actionName,
		token->context,
		token->label,
		token->length,
		INFORMATION_COLOR, _lexeme, DEFAULT_COLOR,
		token->line,
		token->semanticValue);
	free(_lexeme);
	_lexeme = NULL;
}

/* PUBLIC FUNCTIONS */

CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus EnterImportExpressionLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, OPEN_BRACE);
		_logTokenAction(__FUNCTION__, token);
		// destroyToken(token); // se destruye en _cleanupTokens
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, OPEN_COMMENT);
		_logTokenAction(__FUNCTION__, token);
		// destroyToken(token); // se destruye en _cleanupTokens
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EOFLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus IgnoredLexemeAction() {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
	
		_addTokenToList(token);
	}
	return IN_PROGRESS;
}

CompilationStatus IntegerLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, INTEGER);
	token->semanticValue->integer = atoi(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); //  se destruye en _cleanupTokens
	return status;
}

CompilationStatus LeaveImportExpressionLexemeAction() {
	pushInputBuffer(_inputBuffer);
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_BRACE);
		_logTokenAction(__FUNCTION__, token);
		// destroyToken(token); // se destruye en _cleanupTokens
	}
	return IN_PROGRESS;
}

CompilationStatus LeaveMultilineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_COMMENT);
		_logTokenAction(__FUNCTION__, token);
		// destroyToken(token); // se destruye en _cleanupTokens
	}
	return IN_PROGRESS;
}

CompilationStatus ParenthesisLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus SubexpressionLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IGNORED);
	_inputBuffer = createInputBuffer(_lexicalAnalyzer, token->lexeme);
	if (_logIgnoredLexemes) {
		_logTokenAction(__FUNCTION__, token);
	}
	// destroyToken(token); // se destruye en _cleanupTokens
	return IN_PROGRESS;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return FAILED;
}

// Acciones específicas del DSL 

CompilationStatus KeywordLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus IdentifierLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IDENTIFIER);
	
	if (token == NULL) {
		return FAILED;
	}
	
	if (token->lexeme == NULL) {
		// destroyToken(token); // se destruye en _cleanupTokens
		return FAILED;
	}
	
	if (token->semanticValue == NULL) {
		// destroyToken(token); // se destruye en _cleanupTokens
		return FAILED;
	}
	
	token->semanticValue->string = strdup(token->lexeme);
	
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); 
	return status;
}

CompilationStatus StringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, STRING);
	char * content = strdup(token->lexeme + 1); // Saltar comilla de apertura
	content[strlen(content) - 1] = '\0'; // Remover comilla de cierre
	token->semanticValue->string = content;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // 
	return status;
}

CompilationStatus DecimalLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DECIMAL);
	token->semanticValue->decimal = atof(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); //  se destruye en _cleanupTokens
	return status;
}

CompilationStatus DimensionsLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DIMENSIONS);
	char * x_pos = strchr(token->lexeme, 'x');
	if (x_pos != NULL) {
		*x_pos = '\0';
		token->semanticValue->dimensions.width = atoi(token->lexeme);
		token->semanticValue->dimensions.height = atoi(x_pos + 1);
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus CoordinatesLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COORDINATES);
	char * content = strdup(token->lexeme + 1); 
	content[strlen(content) - 1] = '\0';
	char * comma_pos = strchr(content, ',');
	if (comma_pos != NULL) {
		*comma_pos = '\0';
		token->semanticValue->coordinates.x = atoi(content);
		token->semanticValue->coordinates.y = atoi(comma_pos + 1);
	}
	free(content);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus ColorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	token->semanticValue->string = strdup(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus BraceLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus SemicolonLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, SEMICOLON);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); // se destruye en _cleanupTokens
	return status;
}

CompilationStatus CommaLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COMMA);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); //  se destruye en _cleanupTokens
	return status;
}

CompilationStatus ColonLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COLON);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	// destroyToken(token); //  se destruye en _cleanupTokens
	return status;
}
