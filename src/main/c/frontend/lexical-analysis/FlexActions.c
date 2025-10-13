#include "FlexActions.h"
#include "FlexScanner.h"
#include "FlexExport.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* MODULE INTERNAL STATE */

static bool _logIgnoredLexemes = true;
static InputBuffer * _inputBuffer = NULL;
static LexicalAnalyzer * _lexicalAnalyzer = NULL;
static Logger * _logger = NULL;


static UnitType _parse_unit_suffix(const char *s);
static void _trim(char *s);
static int _parse_value_and_unit(const char *side, float *outVal, UnitType *outUnit);


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
		destroyToken(token);
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, OPEN_COMMENT);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
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
	destroyToken(token);
	return status;
}

CompilationStatus LeaveImportExpressionLexemeAction() {
	pushInputBuffer(_inputBuffer);
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_BRACE);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus LeaveMultilineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_COMMENT);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
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
	destroyToken(token);
	return IN_PROGRESS;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	if(token) destroyToken(token);
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
	destroyToken(token);
	return status;
}

CompilationStatus StringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, STRING);
	char * content = strdup(token->lexeme + 1); 
	content[strlen(content) - 1] = '\0'; 
	token->semanticValue->string = content;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
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

	/* Copiamos el lexema porque vamos a partirlo en 'x' / 'X' */
	char txt[128];
	snprintf(txt, sizeof(txt), "%s", token->lexeme);

	/* Buscamos separador 'x' o 'X' entre ancho y alto */
	char *sep = strpbrk(txt, "xX");
	float w = 0.f, h = 0.f;
	UnitType wu = UNIT_PX, hu = UNIT_PX;

	if (sep) {
		/* Partimos en dos lados */
		*sep = '\0';
		char *lhs = txt;           /* lado izquierdo */
		char *rhs = sep + 1;       /* lado derecho   */

		_trim(lhs);
		_trim(rhs);

		int ok1 = _parse_value_and_unit(lhs, &w, &wu);
		int ok2 = _parse_value_and_unit(rhs, &h, &hu);

		if (!(ok1 && ok2)) {
			/* Fallback total: intenta legacy "int x int" directo desde el lexema original */
			int wi = 0, hi = 0;
			if (sscanf(token->lexeme, " %d %*[*xX] %d ", &wi, &hi) == 2) {
				w = (float)wi; h = (float)hi; wu = hu = UNIT_PX;
			} else {
				/* No se pudo parsear */
				w = h = 0.f; wu = hu = UNIT_PX;
			}
		}
	} else {
		/* No hay 'x' → último intento legacy completo */
		int wi = 0, hi = 0;
		if (sscanf(token->lexeme, " %d %*[*xX] %d ", &wi, &hi) == 2) {
			w = (float)wi; h = (float)hi; wu = hu = UNIT_PX;
		} else {
			w = h = 0.f; wu = hu = UNIT_PX;
		}
	}

	/* Escribir semanticValue (float + unidad) */
	token->semanticValue->dimensions.width      = w;
	token->semanticValue->dimensions.height     = h;
	token->semanticValue->dimensions.widthUnit  = wu;
	token->semanticValue->dimensions.heightUnit = hu;

	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

static UnitType _parse_unit_suffix(const char *s) {
	if (!s || !*s) return UNIT_PX;
	if (strcmp(s, "px")  == 0) return UNIT_PX;
	if (strcmp(s, "rem") == 0) return UNIT_REM;
	if (strcmp(s, "em")  == 0) return UNIT_EM;
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


/* parsea un "lado" como "800px" o "12.5rem" o "50%" o "800"
   - Devuelve 1 si pudo extraer número (y opcionalmente unidad)
   - Escribe en *outVal y outUnit (UNIT_PX por defecto si no hay sufijo) */
static int _parse_value_and_unit(const char *side, float *outVal, UnitType *outUnit) {
	if (!side || !outVal || !outUnit) return 0;
	char buf[64];
	snprintf(buf, sizeof(buf), "%s", side);
	_trim(buf);

	float v = 0.f;
	char suf[8] = {0};
	int matched = sscanf(buf, " %f%7s ", &v, suf);
	if (matched >= 1) {
		*outVal  = v;
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
