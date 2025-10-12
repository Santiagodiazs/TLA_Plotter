#include "BisonActions.h"
#include "../../support/type/CompilationStatus.h"

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

// ============= COLOR PARSING FUNCTIONS =============

Color * ParseNamedColor(const char * name) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Color * color = createColor(NAMED_COLOR);
	if (color != NULL && name != NULL) {
		color->value.name = malloc(strlen(name) + 1);
		if (color->value.name != NULL) {
			strcpy(color->value.name, name);
		}
	}
	return color;
}

Color * ParseHexColor(const char * hex) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Color * color = createColor(HEX_COLOR_TYPE);
	if (color != NULL && hex != NULL) {
		color->value.hex = malloc(strlen(hex) + 1);
		if (color->value.hex != NULL) {
			strcpy(color->value.hex, hex);
		}
	}
	return color;
}

Color * ParseRgbColor(const char * rgb) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Color * color = createColor(RGB_COLOR_TYPE);
	if (color != NULL && rgb != NULL) {
		// Parsear "rgb(255,0,0)" -> r=255, g=0, b=0
		int r, g, b;
		if (sscanf(rgb, "rgb(%d,%d,%d)", &r, &g, &b) == 3) {
			color->value.rgb.r = r;
			color->value.rgb.g = g;
			color->value.rgb.b = b;
		}
	}
	return color;
}

Color * ParseRgbaColor(const char * rgba) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Color * color = createColor(RGBA_COLOR_TYPE);
	if (color != NULL && rgba != NULL) {
		// Parsear "rgba(255,0,0,1.0)" -> r=255, g=0, b=0, a=1.0
		int r, g, b;
		float a;
		if (sscanf(rgba, "rgba(%d,%d,%d,%f)", &r, &g, &b, &a) == 4) {
			color->value.rgba.r = r;
			color->value.rgba.g = g;
			color->value.rgba.b = b;
			color->value.rgba.a = a;
		}
	}
	return color;
}

// ============= SEMANTIC VALIDATION =============

CompilationStatus ValidateFigureProperties(FigureType type, Property * properties) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (properties == NULL) {
		return SUCCEEDED; 
	}
	
	Property * current = properties;
	while (current != NULL) {
		switch (type) {
			case CIRCLE_FIGURE:
				if (current->type == SIZE_PROPERTY) {
					fprintf(stderr, "ERROR: Circle figures cannot use 'size' property. Use 'radius' instead.\n");
					return FAILED;
				}
				break;
				
			case LINE_FIGURE:
				if (current->type == SIZE_PROPERTY) {
					fprintf(stderr, "ERROR: Line figures cannot use 'size' property. Use 'from' and 'to' instead.\n");
					return FAILED;
				}
				if (current->type == FILL_PROPERTY) {
					fprintf(stderr, "ERROR: Line figures cannot use 'fill' property. Use 'stroke' instead.\n");
					return FAILED;
				}
				break;
				
			case RECTANGLE_FIGURE:
			case ELLIPSE_FIGURE:
				if (current->type == RADIUS_PROPERTY) {
					fprintf(stderr, "ERROR: Rectangle/Ellipse figures cannot use 'radius' property. Use 'size' instead.\n");
					return FAILED;
				}
				break;
				
			default:
				break;
		}
		current = current->next;
	}
	
	return SUCCEEDED;
}

// ============= DSL SEMANTIC ACTIONS =============

Figure * CreateFigureSemanticAction(FigureType type, const char * id, Property * properties) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	// Validate properties for this figure type
	if (ValidateFigureProperties(type, properties) == FAILED) {
		return NULL; 
	}
	
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

Property * SetPropertyColorSemanticAction(Property * property, Color * color) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (property != NULL) {
		property->value.colorValue = color;
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
