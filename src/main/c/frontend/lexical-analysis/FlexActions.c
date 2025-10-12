#include "FlexActions.h"
#include "FlexScanner.h"
#include "FlexExport.h"
#include <limits.h>

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
	destroyToken(token);
	return status;
}

CompilationStatus EnterImportExpressionLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, OPEN_BRACE);
		_logTokenAction(__FUNCTION__, token);
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, OPEN_COMMENT);
		_logTokenAction(__FUNCTION__, token);
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EOFLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus IgnoredLexemeAction() {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		// No necesitamos registrar para limpieza posterior en arquitectura PUSH
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus IntegerLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, INTEGER);
	
	if (token == NULL) {
		return FAILED;
	}
	
	if (token->lexeme == NULL) {
		return FAILED;
	}
	
	if (token->semanticValue == NULL) {
		return FAILED;
	}
	
	
	if (token->length == 0) {
		return FAILED;
	}
	
	
	char *endptr;
	long value = strtol(token->lexeme, &endptr, 10);
	if (*endptr != '\0') {
		
		return FAILED;
	}
	
	if (value < INT_MIN || value > INT_MAX) {
		return FAILED;
	}
	
	token->semanticValue->integer = (int) value;
	
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	return status;
}

CompilationStatus LeaveImportExpressionLexemeAction() {
	pushInputBuffer(_inputBuffer);
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_BRACE);
		_logTokenAction(__FUNCTION__, token);
	}
	return IN_PROGRESS;
}

CompilationStatus LeaveMultilineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_COMMENT);
		_logTokenAction(__FUNCTION__, token);
	}
	return IN_PROGRESS;
}

CompilationStatus ParenthesisLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus SubexpressionLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IGNORED);
	_inputBuffer = createInputBuffer(_lexicalAnalyzer, token->lexeme);
	if (_logIgnoredLexemes) {
		_logTokenAction(__FUNCTION__, token);
	}
	return IN_PROGRESS;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	return FAILED;
}

// Acciones específicas del DSL 

CompilationStatus KeywordLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus IdentifierLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IDENTIFIER);
	
	if (token == NULL) {
		return FAILED;
	}
	
	if (token->lexeme == NULL) {
		return FAILED;
	}
	
	if (token->semanticValue == NULL) {
		return FAILED;
	}
	
	token->semanticValue->string = strdup(token->lexeme);
	
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	return status;
}

CompilationStatus StringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, STRING);
	char * content = strdup(token->lexeme + 1); 
	content[strlen(content) - 1] = '\0'; 
	token->semanticValue->string = content;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
// 
	return status;
}

CompilationStatus DecimalLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DECIMAL);
	token->semanticValue->decimal = atof(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus DimensionsLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DIMENSIONS);
	if (!token || !token->lexeme || !token->semanticValue) {
		if (token) destroyToken(token);
		return FAILED;
	}
	int w = 0, h = 0;
	// Acepta espacios opcionales (ya que el lexeme los permite por la regla Flex)
	if (sscanf(token->lexeme, " %d %*[*xX] %d ", &w, &h) != 2) {
		w = h = 0;
	}
	token->semanticValue->dimensions.width  = w;
	token->semanticValue->dimensions.height = h;

	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);         
	return status;               
}

CompilationStatus CoordinatesLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COORDINATES);
	if (!token || !token->semanticValue || !token->lexeme) {
		if (token) destroyToken(token);
		return FAILED;
	}

	int x = 0, y = 0;
	if (sscanf(token->lexeme, " ( %d %*[, ] %d ) ", &x, &y) != 2) {
		x = y = 0;
	}
	token->semanticValue->coordinates.x = x;
	token->semanticValue->coordinates.y = y;

	_logTokenAction(__FUNCTION__, token);
	CompilationStatus st = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return st;
}

CompilationStatus ColorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	if (!token || !token->lexeme || !token->semanticValue) {
		if (token) destroyToken(token);
		return FAILED;
	}
	
	
	token->semanticValue->string = strdup(token->lexeme);
	if (!token->semanticValue->string) {
		destroyToken(token);
		return FAILED;
	}
	
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);                // NO libera string; la liberará el AST
	return status;
}

CompilationStatus BraceLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus SemicolonLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, SEMICOLON);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus CommaLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COMMA);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus ColonLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COLON);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}
