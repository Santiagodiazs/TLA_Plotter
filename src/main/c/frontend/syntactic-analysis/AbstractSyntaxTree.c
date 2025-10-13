#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
	return _shutdownAbstractSyntaxTreeModule;
}

/* PUBLIC FUNCTIONS */

void destroyConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void destroyExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				destroyExpression(expression->leftExpression);
				destroyExpression(expression->rightExpression);
				break;
			case FACTOR:
				destroyFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void destroyFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				destroyConstant(factor->constant);
				break;
			case EXPRESSION_FACTOR:
				destroyExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void destroyScene(Scene * scene) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (scene != NULL) {
		if (scene->name != NULL) {
			free(scene->name);
		}
		if (scene->backgroundColor != NULL) {
			free(scene->backgroundColor);
		}
		destroyFigure(scene->figures);
		destroyLayer(scene->layers); // Destruir layers
		if (scene->symbols) destroySymbol(scene->symbols);
		if (scene->uses) destroyUseInstance(scene->uses);
		free(scene);
	}
}

void destroySymbol(Symbol * symbol){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  while (symbol) {
    Symbol * next = symbol->next;
    if (symbol->name) free(symbol->name);
    free(symbol);
    symbol = next;
  }
}

void destroyUseInstance(UseInstance * useInstance){
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  while (useInstance) {
    UseInstance * nxt = useInstance->next;
    if (useInstance->symbolName) free(useInstance->symbolName);
    free(useInstance);
    useInstance = nxt;
  }
}

void destroyProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		switch (program->type) {
			case EXPRESSION:
				destroyExpression(program->expression);
				break;
			case SCENE_PROGRAM:
				destroyScene(program->scene);
				break;
		}
		free(program);
	}
}

void destroyFigure(Figure * figure) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (figure != NULL) {
		Figure * current = figure;
		while (current != NULL) {
			Figure * next = current->next;
			
			if (current->id != NULL) {
				free(current->id);
			}
			destroyProperty(current->properties);

			if (current->transforms) {
                destroyTransform(current->transforms);
                current->transforms = NULL;
            }

			free(current);
			current = next;
		}
	}
}

void destroyProperty(Property * property) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (property != NULL) {
		Property * current = property;
		while (current != NULL) {
			Property * next = current->next;
			
			// Liberar valores color si es necesario
			if (current->type == FILL_PROPERTY || current->type == STROKE_PROPERTY) {
				if (current->value.colorValue != NULL) {
					destroyColor(current->value.colorValue);
				}
			}
			
			free(current);
			current = next;
		}
	}
}

Color * createColor(ColorType type) {
	logDebugging(_logger, "Creating color of type %d", type);
	Color * color = calloc(1, sizeof(Color));
	if (color != NULL) {
		color->type = type;
	}
	return color;
}

void destroyColor(Color * color) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (color != NULL) {
		switch (color->type) {
			case NAMED_COLOR:
				if (color->value.name != NULL) {
					free(color->value.name);
				}
				break;
			case HEX_COLOR_TYPE:
				if (color->value.hex != NULL) {
					free(color->value.hex);
				}
				break;
			case RGB_COLOR_TYPE:
			case RGBA_COLOR_TYPE:
				
				break;
		}
		free(color);
	}
}

// ============= LAYER FUNCTIONS =============

Layer * createLayer(const char * name, int zLevel) {
	logDebugging(_logger, "Creating layer with name '%s' and z-level %d", name ? name : "NULL", zLevel);
	Layer * layer = calloc(1, sizeof(Layer));
	if (layer != NULL) {
		layer->zLevel = zLevel;
		layer->figures = NULL;
		layer->next = NULL;
		layer->name = name ? strdup(name) : NULL;
	}
	return layer;
}

void destroyLayer(Layer * layer) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (layer != NULL) {
		Layer * current = layer;
		while (current != NULL) {
			Layer * next = current->next;
			
			// Liberar nombre
			if (current->name != NULL) {
				free(current->name);
			}
			
			// Liberar figuras
			if (current->figures != NULL) {
				destroyFigure(current->figures);
			}
			
			free(current);
			current = next;
		}
	}
}

Transform * createTransformScale(float sx, float sy) {
	Transform *t = calloc(1, sizeof(Transform));
	if (!t) return NULL;
	t->type = TRANSFORM_SCALE;
	t->value.scale.sx = sx;
	t->value.scale.sy = sy;
	return t;
}

Transform * createTransformRotate(float degrees) {
	Transform *t = calloc(1, sizeof(Transform));
	if (!t) return NULL;
	t->type = TRANSFORM_ROTATE;
	t->value.rotate.degrees = degrees;
	return t;
}

Transform * createTransformTranslate(float tx, float ty) {
	Transform *t = calloc(1, sizeof(Transform));
	if (!t) return NULL;
	t->type = TRANSFORM_TRANSLATE;
	t->value.translate.tx = tx;
	t->value.translate.ty = ty;
	return t;
}

void destroyTransform(Transform *t) {
	while (t) {
		Transform *next = t->next;
		free(t);
		t = next;
	}
}

