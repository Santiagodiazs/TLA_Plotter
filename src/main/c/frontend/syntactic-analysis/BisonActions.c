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
	if(_globalPalette){
		destroyPalette(_globalPalette);
		_globalPalette = NULL;
	}
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

  

    program->type = SCENE_PROGRAM;

    if (_compilerState != NULL) {
        _compilerState->abstractSyntaxtTree = program;
    }

    logDebugging(_logger, "Created program with scene");
    return program;
}

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

Color * ParsePaletteColor(const char * palette_name, const char * color_name) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Color *found_color = LookupPaletteColor(color_name);
	if (found_color) {
		return found_color;
	}
	
	Color * color = createColor(NAMED_COLOR);
	if (color != NULL && color_name != NULL) {
		color->value.name = malloc(strlen(color_name) + 1);
		if (color->value.name != NULL) {
			strcpy(color->value.name, color_name);
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

Color * ParseColorFunction(const char * colorFunc) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	
	Color * color = createColor(RGB_COLOR_TYPE);
	if (color != NULL && colorFunc != NULL) {
		int r, g, b;
		if (sscanf(colorFunc, "color(%d,%d,%d)", &r, &g, &b) == 3) {
			color->value.rgb.r = r;
			color->value.rgb.g = g;
			color->value.rgb.b = b;
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



Figure * CreateFigureSemanticAction(FigureType type, const char * id, Property * properties) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	

	
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

Symbol * CreateSymbolMove(const char *name, Figure *src) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Symbol * symbol = calloc(1, sizeof(Symbol));
    if (!symbol) return NULL;

    symbol->name = name ? strdup(name) : NULL;
    symbol->next = NULL;

    if (src) {
        symbol->figure = *src;
        src->id = NULL;
        src->properties = NULL;
        src->next = NULL;
        free(src);
    }
    return symbol;
}

UseInstance * CreateUseInstance(const char *symbolName) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    UseInstance * useInstace = calloc(1, sizeof(UseInstance));
		useInstace->next = NULL;
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
                // Enforce uniform scaling as per requirement
                Transform *t = createTransformScale(cur->value.scale.x, cur->value.scale.x);
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
            destroyProperty(toFree);  
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
    
    if (!entries) {
        return BasicSceneSemanticAction(NULL);
    }

    // Add to _globalPalette immediately
    PaletteEntry *globalCopy = duplicatePalette(entries);
    if (_globalPalette == NULL) {
        _globalPalette = globalCopy;
    } else {
        PaletteEntry *tail = _globalPalette;
        while (tail->next) tail = tail->next;
        tail->next = globalCopy;
    }

    // Create a new scene with the palette
    Scene *scene = BasicSceneSemanticAction(NULL);
    if (scene) {
        scene->palette = entries;
    }
    
    return scene;
}

Scene * SceneWithNamedPaletteBlock(const char *name, PaletteEntry *entries) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    
    Scene * result = SceneWithPaletteBlock(entries);
    
    if (name) {
        free((char *)name);
    }
    
    return result;
}

Color * LookupPaletteColor(const char *name) {
    if (!name) return NULL;
    
    PaletteEntry *palette = _globalPalette;
    if (!palette) {
        return NULL;
    }
    
    Color *found = NULL;
    for (PaletteEntry *it = palette; it; it = it->next) {
        if (it->name && strcmp(it->name, name) == 0 && it->color) {
            found = it->color; 
        }
    }
    if (!found) {
        return NULL;
    }

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
			Layer * last = acc->layers;
			while (last->next) last = last->next;
			last->next = item->layers;
		}
		item->layers = NULL;
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

	if (item->palette) {
		if (!acc->palette) {
			acc->palette = item->palette;
		} else {
			PaletteEntry * last = acc->palette;
			while (last->next) last = last->next;
			last->next = item->palette;
		}
		item->palette = NULL; 
	}

    if (item->groups) {
        if (!acc->groups) {
            acc->groups = item->groups;
        } else {
            Group * last = acc->groups;
            while (last->next) last = last->next;
            last->next = item->groups;
        }
        item->groups = NULL;
    }

	destroyScene(item);
	return acc;
}

Group * CreateGroup(const char *name, GroupContent *content) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Group *group = calloc(1, sizeof(Group));
    if (!group) return NULL;
    
    group->name = name ? strdup(name) : NULL;
    group->figures = NULL;
    group->properties = NULL;
    group->next = NULL;
    
    if (content) {
        group->figures = content->figures;
        group->properties = content->properties;
        free(content);
    }
    
    return group;
}

GroupContent * GroupFromFigure(Figure *figure) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    GroupContent *content = calloc(1, sizeof(GroupContent));
    if (!content) return NULL;
    
    content->figures = figure;
    content->properties = NULL;
    return content;
}

GroupContent * GroupFromProperty(Property *property) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    GroupContent *content = calloc(1, sizeof(GroupContent));
    if (!content) return NULL;
    
    content->figures = NULL;
    content->properties = property;
    return content;
}

GroupContent * MergeGroupContent(GroupContent *acc, GroupContent *item) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!acc) return item;
    if (!item) return acc;
    
    if (item->figures) {
        Figure *last = acc->figures;
        if (last) {
            while (last->next) last = last->next;
            last->next = item->figures;
        } else {
            acc->figures = item->figures;
        }
        item->figures = NULL;
    }
    
    if (item->properties) {
        Property *last = acc->properties;
        if (last) {
            while (last->next) last = last->next;
            last->next = item->properties;
        } else {
            acc->properties = item->properties;
        }
        item->properties = NULL;
    }
    
    free(item);
    return acc;
}

Scene * AddGroupToSceneSemanticAction(Scene *scene, Group *group) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!scene || !group) return scene;
    group->next = scene->groups;
    scene->groups = group;
    return scene;
}

Property * MergeProperties(Property *acc, Property *item) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (!acc) return item;
    if (!item) return acc;
    
    Property *last = acc;
    while (last->next) last = last->next;
    last->next = item;
    return acc;
}
