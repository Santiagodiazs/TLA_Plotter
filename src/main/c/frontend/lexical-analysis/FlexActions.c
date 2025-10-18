#include "FlexActions.h"
#include "FlexExport.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* MODULE INTERNAL STATE */

static bool _logIgnoredLexemes = true;
static LexicalAnalyzer * _lexicalAnalyzer = NULL;
static Logger * _logger = NULL;

/* PRIVATE FUNCTIONS FOR UNIT PARSING */

static UnitType _parse_unit_suffix(const char *s);
static void _trim(char *s);
static int _parse_value_and_unit(const char *side, float *outVal, UnitType *outUnit);

void _shutdownFlexActionsModule();

ModuleDestructor initializeFlexActionsModule(LexicalAnalyzer * lexicalAnalyzer) {
	_lexicalAnalyzer = lexicalAnalyzer;
	_logger = createLogger("FlexActions");
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	return _shutdownFlexActionsModule;
}

void _shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
	_lexicalAnalyzer = NULL;
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

CompilationStatus IgnoredLexemeAction() {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus BraceLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	destroyToken(token);
	return FAILED;
}

CompilationStatus KeywordLexemeAction(TokenLabel label) {
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

CompilationStatus ColonLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COLON);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus DotLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DOT);
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

CompilationStatus IdentifierLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IDENTIFIER);
	if (token && token->semanticValue) {
		token->semanticValue->string = strdup(token->lexeme);
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus StringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, STRING);
	if (token && token->semanticValue) {
		token->semanticValue->string = strdup(token->lexeme);
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus IntegerLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, INTEGER);
	token->semanticValue->integer = atoi(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus DecimalLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DECIMAL);
	if (token && token->semanticValue) {
		token->semanticValue->decimal = atof(token->lexeme);
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

// Funciones específicas del DSL para tokens complejos
CompilationStatus DimensionsLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, DIMENSIONS);
	if (token && token->semanticValue) {
		int width = 0, height = 0;
		if (sscanf(token->lexeme, "%dx%d", &width, &height) == 2 ||
			sscanf(token->lexeme, "%d x %d", &width, &height) == 2) {
			token->semanticValue->dimensions.width = width;
			token->semanticValue->dimensions.height = height;
		}
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus CoordinatesLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, COORDINATES);
	if (token && token->semanticValue) {
		
		int x = 0, y = 0;
		if (sscanf(token->lexeme, "(%d,%d)", &x, &y) == 2 ||
			sscanf(token->lexeme, "(%d, %d)", &x, &y) == 2) {
			token->semanticValue->coordinates.x = x;
			token->semanticValue->coordinates.y = y;
		}
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus ColorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	if (token && token->semanticValue) {
		token->semanticValue->string = strdup(token->lexeme);
	}
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}


CompilationStatus EOFLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus ParenthesisLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus EnterImportExpressionLexemeAction(FlexContext context) {
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus LeaveImportExpressionLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context) {
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus LeaveMultilineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

CompilationStatus EnterOnelineCommentLexemeAction(FlexContext context) {
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus LeaveOnelineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

CompilationStatus EnterStringContextLexemeAction(FlexContext context) {
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus LeaveStringContextLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

CompilationStatus SemicolonAndLeaveStringContextLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	Token * token = createToken(_lexicalAnalyzer, SEMICOLON);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus SubexpressionLexemeAction() {
	return IgnoredLexemeAction();
}

/* PRIVATE FUNCTIONS FOR UNIT PARSING */

static UnitType _parse_unit_suffix(const char *s) {
    if (!s || !*s) return UNIT_PX;
    if (strcmp(s, "px")  == 0) return UNIT_PX;
    if (strcmp(s, "rem") == 0) return UNIT_REM;
    if (strcmp(s, "em")  == 0) return UNIT_EM;
    if (strcmp(s, "vw")  == 0) return UNIT_VW;
    if (strcmp(s, "vh")  == 0) return UNIT_VH;
    if (strcmp(s, "%")   == 0) return UNIT_PERCENT;
    return UNIT_PX;
}

/* recorta espacios in-place */
static void _trim(char *s) {
	if (!s) return;
	char *p = s;
	while (*p && isspace((unsigned char)*p)) p++;
	if (p != s) memmove(s, p, strlen(p) + 1);
	size_t n = strlen(s);
	while (n > 0 && isspace((unsigned char)s[n-1])) s[--n] = '\0';
}

static int _parse_value_and_unit(const char *side, float *outVal, UnitType *outUnit) {
	if (!side || !outVal || !outUnit) return 0;
	
	char buf[256];
	strncpy(buf, side, sizeof(buf)-1);
	buf[sizeof(buf)-1] = '\0';
	_trim(buf);
	
	float val = 0.0f;
	char suf[16] = {0};
	int matched = sscanf(buf, "%f%15s", &val, suf);
	if (matched >= 1) {
		*outVal  = val;
		*outUnit = (matched == 2) ? _parse_unit_suffix(suf) : UNIT_PX;
		return 1;
	}

	int vint = 0;
	if (sscanf(buf, " %d ", &vint) == 1) {
		*outVal  = (float)vint;
		*outUnit = UNIT_PX;
		return 1;
	}

	return 0;
}
