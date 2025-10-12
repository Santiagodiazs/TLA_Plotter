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

/* HELPER FUNCTIONS */

static inline Property* new_property(PropertyType type) {
	Property* p = calloc(1, sizeof(Property));
	if (p == NULL) {
		return NULL;
	}
	p->type = type;
	p->next = NULL;  
	return p;
}

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

// ============= DSL SEMANTIC ACTIONS =============

Figure * CreateFigureSemanticAction(FigureType type, const char * id, Property * properties) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Figure * figure = calloc(1, sizeof(Figure));
	if (figure == NULL) {
		return NULL;
	}
	
	figure->type = type;
	figure->properties = properties;
	figure->next = NULL;
	
	if (id != NULL && strlen(id) > 0) {
		figure->id = malloc(strlen(id) + 1);
		if (figure->id != NULL) {
			strcpy(figure->id, id);
		}
	} else {
		figure->id = NULL;
	}
	
	logDebugging(_logger, "Created figure of type %d", type);
	return figure;
}

Property * CreatePropertySemanticAction(PropertyType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return new_property(type);
}

Property * SetPropertyCoordinatesSemanticAction(Property * property, int x, int y) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL) {
		property->value.coordinates.x = x;
		property->value.coordinates.y = y;
	}
	
	return property;
}

Property * SetPropertyDimensionsSemanticAction(Property * property, int width, int height) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL) {
		property->value.dimensions.width = width;
		property->value.dimensions.height = height;
	}
	
	return property;
}

Property * SetPropertyScaleSemanticAction(Property * property, float x, float y) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL) {
		property->value.scale.x = x;
		property->value.scale.y = y;
	}
	
	return property;
}

Property * SetPropertyIntValueSemanticAction(Property * property, int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL) {
		property->value.intValue = value;
	}
	
	return property;
}

Property * SetPropertyFloatValueSemanticAction(Property * property, float value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL) {
		property->value.floatValue = value;
	}
	
	return property;
}

Property * SetPropertyStringValueSemanticAction(Property * property, const char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL && value != NULL) {
		property->value.stringValue = malloc(strlen(value) + 1);
		if (property->value.stringValue != NULL) {
			strcpy(property->value.stringValue, value);
		}
	}
	
	return property;
}



Scene * AddFigureToSceneSemanticAction(Scene * scene, Figure * figure) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (scene == NULL || figure == NULL) {
		return scene;
	}
	
	if (scene->figures == NULL) {
		scene->figures = figure;
	} else {
		
		Figure * current = scene->figures;
		size_t guard = 0;
		while (current->next != NULL) {
			if (current->next == current) { 
				fprintf(stderr, "[BUG] ciclo en figuras de escena\n"); 
				abort(); 
			}
			current = current->next;
			if (++guard > 1000000) { 
				fprintf(stderr, "[BUG] lista de figuras interminable\n"); 
				abort(); 
			}
		}
		current->next = figure;
	}
	
	logDebugging(_logger, "Added figure to scene");
	return scene;
}
