#include "BisonActions.h"
#include "../../support/type/CompilationStatus.h"
#include <string.h>
#include <stdio.h>

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Scene * _currentScene = NULL;
static PaletteEntry * _globalPalette = NULL;
Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_globalPalette = NULL; // La palette se libera con la scene principal
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

Property * SetPropertyDimensionsWithUnitSemanticAction(
    Property * property,
    float width,  UnitType wUnit,
    float height, UnitType hUnit
) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (property) {
        property->value.dimensions.width      = width;
        property->value.dimensions.height     = height;
        property->value.dimensions.widthUnit  = wUnit;
        property->value.dimensions.heightUnit = hUnit;
    }
    return property;
}

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
    scene->figures = NULL;
    scene->backgroundColor = NULL;
    scene->layers = NULL;
    scene->symbols = NULL;
    scene->uses = NULL;
    scene->palette = NULL;
    _currentScene = scene;

	
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

	if (program->scene && ValidateLayerZLevel(program->scene) == FAILED) {
        destroyProgram(program);
        return NULL;
    }

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
	Color *pal = LookupPaletteColor(name);
    if (pal) return pal;
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

PaletteEntry * CreatePaletteEntrySemanticAction(char *name, Color *color) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    PaletteEntry *e = createPaletteEntry(name, color);
    if (name) free(name);
    return e;
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

CompilationStatus ValidateLayerZLevel(const Scene *scene) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!scene) return SUCCEEDED;

    for (Layer *a = scene->layers; a; a = a->next) {
        for (Layer *b = a->next; b; b = b->next) {
            if (a->zLevel == b->zLevel) {
                const char *na = (a->name ? a->name : "<unnamed>");
                const char *nb = (b->name ? b->name : "<unnamed>");
                fprintf(stderr,
                        "ERROR: Duplicate layer z-level %d detected between '%s' and '%s'.\n",
                        a->zLevel, na, nb);
                return FAILED;
            }
        }
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
	figure->transforms = NULL;

	ApplyTransformPropertiesToFigure(figure, &figure->properties);
	
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
    if (property) {
        property->value.dimensions.width      = (float)width;
        property->value.dimensions.height     = (float)height;
        property->value.dimensions.widthUnit  = UNIT_PX;
        property->value.dimensions.heightUnit = UNIT_PX;
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

Symbol * CreateSymbolMove(const char *name, Figure * figure) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Symbol *s = calloc(1, sizeof(Symbol));
    if (!s) return NULL;
    s->name = name ? strdup(name) : NULL;
    if (figure) {
        s->figure = *figure;        // copia superficial
        figure->id = NULL; figure->properties = NULL; figure->transforms = NULL; figure->next = NULL;
        free(figure);
    }
    return s;
}

UseInstance * CreateUseInstance(const char *symbolName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    UseInstance * useInstace = calloc(1, sizeof(UseInstance));
    if (!useInstace) return NULL;
    useInstace->symbolName = symbolName ? strdup(symbolName) : NULL;
    return useInstace;
}

Scene * AddSymbolToSceneSemanticAction(Scene *scene, Symbol * symbol) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!scene || !symbol) return scene;
    symbol->next = scene->symbols;
    scene->symbols = symbol;
    return scene;
}

Scene * AddUseToSceneSemanticAction(Scene *scene, UseInstance *useInstance) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!scene || !useInstance) return scene;
    useInstance->next = scene->uses;
    scene->uses = useInstance;
    return scene;
}

Property * SetPropertyTranslateSemanticAction(Property * property, int x, int y) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (property != NULL) {
        property->value.coordinates.x = x;
        property->value.coordinates.y = y;
    }
    return property;
}

void AppendTransform(Figure *figure, Transform *t) {
    if (!figure || !t) return;
    if (!figure->transforms) {
        figure->transforms = t;
        return;
    }
    Transform *it = figure->transforms;
    while (it->next) it = it->next;
    it->next = t;
}

void ApplyTransformPropertiesToFigure(Figure *figure, Property **propertiesHead) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!figure || !propertiesHead || !*propertiesHead) return;

    Property *prev = NULL;
    Property *cur = *propertiesHead;

    while (cur) {
        int removeNode = 0;

        switch (cur->type) {
            case SCALE_PROPERTY: {
                Transform *t = createTransformScale(cur->value.scale.x, cur->value.scale.y);
                if (t) AppendTransform(figure, t);
                removeNode = 1;
                break;
            }
            case ROTATE_PROPERTY: {
                Transform *t = createTransformRotate(cur->value.floatValue);
                if (t) AppendTransform(figure, t);
                removeNode = 1;
                break;
            }
            case TRANSLATE_PROPERTY: {
                int tx = cur->value.coordinates.x;
                int ty = cur->value.coordinates.y;
                Transform *t = createTransformTranslate((float)tx, (float)ty);
                if (t) AppendTransform(figure, t);
                removeNode = 1;
                break;
            }
            default:
                break;
        }

        if (removeNode) {
            Property *toFree = cur;
            if (prev) prev->next = cur->next;
            else *propertiesHead = cur->next;
            cur = cur->next;

            toFree->next = NULL;
            destroyProperty(toFree);  // ya no es propiedad "visible"
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
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

// ============= NEW SCENE HELPER FUNCTIONS =============

Scene * SceneFromFigure(Figure * figure) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Scene * scene = BasicSceneSemanticAction(NULL);
	if (scene && figure) {
		scene->figures = figure;
		logDebugging(_logger, "Created scene from figure");
	}
	return scene;
}


Scene * SceneWithPaletteBlock(PaletteEntry *entries) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!_currentScene) {
        // Si no hay escena actual, podés crear una básica o retornar NULL.
        // _currentScene = BasicSceneSemanticAction(NULL);
        return NULL;
    }
    if (!entries) return _currentScene;

    // Guardar COPIA en palette global para evitar problemas de memoria
    PaletteEntry *globalCopy = duplicatePalette(entries);
    if (_globalPalette == NULL) {
        _globalPalette = globalCopy;
    } else {
        PaletteEntry *tail = _globalPalette;
        while (tail->next) tail = tail->next;
        tail->next = globalCopy;
    }

    // Mantener original en la scene actual
    if (_currentScene->palette == NULL) {
        _currentScene->palette = entries;
    } else {
        PaletteEntry *tail = _currentScene->palette;
        while (tail->next) tail = tail->next;
        tail->next = entries;
    }
    return _currentScene;
}

Color * LookupPaletteColor(const char *name) {
    if (!name) return NULL;
    
    // Usar SOLO palette global para evitar problemas de memoria
    PaletteEntry *palette = _globalPalette;
    if (!palette) return NULL;
    
    Color *found = NULL;
    for (PaletteEntry *it = palette; it; it = it->next) {
        if (it->name && strcmp(it->name, name) == 0 && it->color) {
            found = it->color; // no retornes directamente; hacé copia profunda
        }
    }
    if (!found) return NULL;

    // copia profunda de Color
    Color *copy = createColor(found->type);
    if (!copy) return NULL;
    switch (found->type) {
        case NAMED_COLOR:
            copy->value.name = found->value.name ? strdup(found->value.name) : NULL;
            break;
        case HEX_COLOR_TYPE:
            copy->value.hex = found->value.hex ? strdup(found->value.hex) : NULL;
            break;
        case RGB_COLOR_TYPE:
            copy->value.rgb = found->value.rgb;
            break;
        case RGBA_COLOR_TYPE:
            copy->value.rgba = found->value.rgba;
            break;
    }
    return copy;
}


Scene * SceneWithBackground(Color * color) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Scene * scene = BasicSceneSemanticAction(NULL);
	if (scene && color) {
		// Convertir Color a string para mantener compatibilidad con backgroundColor
		char * colorStr = NULL;
		switch (color->type) {
			case NAMED_COLOR:
				colorStr = malloc(strlen(color->value.name) + 1);
				if (colorStr) strcpy(colorStr, color->value.name);
				break;
			case HEX_COLOR_TYPE:
				colorStr = malloc(strlen(color->value.hex) + 1);
				if (colorStr) strcpy(colorStr, color->value.hex);
				break;
			case RGB_COLOR_TYPE:
				colorStr = malloc(20);
				if (colorStr) sprintf(colorStr, "rgb(%d,%d,%d)", color->value.rgb.r, color->value.rgb.g, color->value.rgb.b);
				break;
			case RGBA_COLOR_TYPE:
				colorStr = malloc(30);
				if (colorStr) sprintf(colorStr, "rgba(%d,%d,%d,%.1f)", color->value.rgba.r, color->value.rgba.g, color->value.rgba.b, color->value.rgba.a);
				break;
		}
		scene->backgroundColor = colorStr;
		destroyColor(color);
		logDebugging(_logger, "Created scene with background");
	}
	return scene;
}


Scene * SceneWithLayerBlock(const char * name, int zLevel, Scene * blockContent) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    Scene * scene = BasicSceneSemanticAction(NULL);
    if (scene && name) {
        Layer * layer = createLayer(name, zLevel);
        if (layer && blockContent) {
            layer->figures = blockContent->figures;
            blockContent->figures = NULL;
        }
        scene->layers = layer;

        if (blockContent) {
            destroyScene(blockContent);
        }
        logDebugging(_logger, "Created scene with layer block '%s' z=%d", name, zLevel);
    }
    return scene;
}

Scene * MergeSceneContent(Scene * acc, Scene * item) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	if (!acc) return item;
	if (!item) return acc;
	
	logDebugging(_logger, "Merging scene content - simplified version");
	
	// Versión simplificada: solo merge básico
	if (item->backgroundColor) {
		if (acc->backgroundColor) {
			free(acc->backgroundColor);
		}
		acc->backgroundColor = item->backgroundColor;
		item->backgroundColor = NULL;
	}
	
	if (item->figures) {
		if (!acc->figures) {
			acc->figures = item->figures;
		} else {
			// Concatenar al final
			Figure * last = acc->figures;
			while (last->next) last = last->next;
			last->next = item->figures;
		}
		item->figures = NULL;
	}
	
	if (item->layers) {
		if (!acc->layers) {
			acc->layers = item->layers;
		} else {
			// Concatenar al final
			Layer * last = acc->layers;
			while (last->next) last = last->next;
			last->next = item->layers;
		}
		item->layers = NULL;
	}

	if (ValidateLayerZLevel(acc) == FAILED) {
            fprintf(stderr, "ERROR: Scene has duplicate layer z-levels.\n");
  }

	if (item->symbols) {
    if (!acc->symbols) {
			acc->symbols = item->symbols;
		} else {
        Symbol *last = acc->symbols; 
				while (last->next) last = last->next;
        last->next = item->symbols;
    }
    item->symbols = NULL;
	}

	if (item->uses) {
    if (!acc->uses) {
			acc->uses = item->uses;
		} else {
        UseInstance * last = acc->uses; 
                                while (last->next) last = last->next;
        last->next = item->uses;
    }
    item->uses = NULL;
}	

	// Preservar la palette - no debe ser liberada prematuramente
	if (item->palette) {
		if (!acc->palette) {
			acc->palette = item->palette;
		} else {
			// Concatenar palettes al final 
			PaletteEntry * last = acc->palette;
			while (last->next) last = last->next;
			last->next = item->palette;
		}
		item->palette = NULL; // Evitar doble liberación
	}

	destroyScene(item);
	return acc;
}

// ============= SCENE SEMANTIC ACTIONS =============


