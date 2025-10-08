#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION_FACTOR;
	return factor;
}

Program * ExpressionProgramSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->expression = expression;
	_compilerState->abstractSyntaxtTree = program;
	return program;
}


Scene * BasicSceneSemanticAction(const char * sceneName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Scene * scene = calloc(1, sizeof(Scene));
	if (scene == NULL) {
		return NULL;
	}
	
	scene->type = BASIC_SCENE;
	
	if (sceneName != NULL && strlen(sceneName) > 0) {
		scene->name = malloc(strlen(sceneName) + 1);
		if (scene->name != NULL) {
			strcpy(scene->name, sceneName);
		} else {
			scene->name = NULL;
		}
	} else {
		scene->name = NULL;
	}
	
	logDebugging(_logger, "Created scene successfully");
	return scene;
}

Program * SceneProgramSemanticAction(Scene * scene) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Program * program = calloc(1, sizeof(Program));
	if (program == NULL) {
		return NULL;
	}
	
	program->scene = scene;
	program->type = SCENE_PROGRAM; 
	
	if (_compilerState != NULL) {
		_compilerState->abstractSyntaxtTree = program;
	}
	
	logDebugging(_logger, "Created program with scene");
	return program;
}
