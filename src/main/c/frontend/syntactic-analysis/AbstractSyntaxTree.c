#include "AbstractSyntaxTree.h"
#include <string.h>

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
		destroyLayer(scene->layers); 
		if (scene->symbols) destroySymbol(scene->symbols);
		if (scene->uses) destroyUseInstance(scene->uses);
		if (scene->groups) destroyGroup(scene->groups);
		if (scene->palette) destroyPalette(scene->palette);
		free(scene);
	}
}

void destroySymbol(Symbol * symbol){
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    while (symbol) {
        Symbol * next = symbol->next;

        if (symbol->name) free(symbol->name);

        if (symbol->figure.id) {
            free(symbol->figure.id);
            symbol->figure.id = NULL;
        }
        if (symbol->figure.properties) {
            destroyProperty(symbol->figure.properties);
            symbol->figure.properties = NULL;
        }
        if (symbol->figure.transforms) {
            destroyTransform(symbol->figure.transforms);
            symbol->figure.transforms = NULL;
        }
        symbol->figure.next = NULL;

        free(symbol);
        symbol = next;
    }
}


void destroyUseInstance(UseInstance * useInstance){
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    while (useInstance) {
        UseInstance * nxt = useInstance->next;
        if (useInstance->symbolName) free(useInstance->symbolName);
        if (useInstance->properties) destroyProperty(useInstance->properties);
        free(useInstance);
        useInstance = nxt;
    }
}

void destroyGroup(Group * group) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    while (group) {
        Group * nxt = group->next;
        if (group->name) free(group->name);
        if (group->figures) destroyFigure(group->figures);
        if (group->properties) destroyProperty(group->properties);
        free(group);
        group = nxt;
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
    if (!figure) return;

    Figure * current = figure;
    while (current) {
        Figure * next = current->next;

        if (current->id) free(current->id);

        if (current->properties) {
            destroyProperty(current->properties);
            current->properties = NULL;
        }

        if (current->transforms) {
            destroyTransform(current->transforms);
            current->transforms = NULL;
        }

        free(current);
        current = next;
    }
}

void destroyProperty(Property * property) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (property != NULL) {
		Property * current = property;
		while (current != NULL) {
			Property * next = current->next;
			
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
			
			if (current->name != NULL) {
				free(current->name);
			}
			
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

PaletteEntry * createPaletteEntry(const char *name, Color *color) {
    logDebugging(_logger, "Creating palette entry '%s'", name ? name : "(null)");
    PaletteEntry *e = calloc(1, sizeof(PaletteEntry));
    if (!e) return NULL;
    e->name = name ? strdup(name) : NULL;
    e->color = color;   
    e->next = NULL;
    return e;
}

void destroyPalette(PaletteEntry *head) {
    logDebugging(_logger, "Destroying palette");
    while (head) {
        PaletteEntry *next = head->next;
        if (head->name) free(head->name);
        if (head->color) destroyColor(head->color);
        free(head);
        head = next;
    }
}


Color * duplicateColor(Color *original) {
    if (!original) return NULL;
    
    Color *copy = createColor(original->type);
    if (!copy) return NULL;
    
    switch (original->type) {
        case NAMED_COLOR:
            copy->value.name = original->value.name ? strdup(original->value.name) : NULL;
            break;
        case HEX_COLOR_TYPE:
            copy->value.hex = original->value.hex ? strdup(original->value.hex) : NULL;
            break;
        case RGB_COLOR_TYPE:
            copy->value.rgb.r = original->value.rgb.r;
            copy->value.rgb.g = original->value.rgb.g;
            copy->value.rgb.b = original->value.rgb.b;
            break;
        case RGBA_COLOR_TYPE:
            copy->value.rgba.r = original->value.rgba.r;
            copy->value.rgba.g = original->value.rgba.g;
            copy->value.rgba.b = original->value.rgba.b;
            copy->value.rgba.a = original->value.rgba.a;
            break;
    }
    return copy;
}

PaletteEntry * duplicatePalette(PaletteEntry *original) {
    if (!original) return NULL;
    
    PaletteEntry *head = NULL;
    PaletteEntry *tail = NULL;
    
    for (PaletteEntry *it = original; it; it = it->next) {
        Color *colorCopy = duplicateColor(it->color);
        PaletteEntry *entryCopy = createPaletteEntry(it->name, colorCopy);
        
        if (!head) {
            head = tail = entryCopy;
        } else {
            tail->next = entryCopy;
            tail = entryCopy;
        }
    }
    
    return head;
}


