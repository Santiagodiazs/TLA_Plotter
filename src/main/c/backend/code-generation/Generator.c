#include "Generator.h"
#include "../domain-specific/Calculator.h"
#include <stdlib.h>
#include <string.h>

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static FILE *f = NULL;
static Variable * _currentVariables = NULL;


/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/** PRIVATE FUNCTIONS */

static char * _indentation(const unsigned int indentationLevel);
static void _generateEpilogue(void);
static void _generateScene(const unsigned int indentationLevel, Scene * scene);
static void _generateLayers(const unsigned int indentationLevel, Layer * layers);
static void _generateFigure(const unsigned int indentationLevel, Figure * figure);
static void _generateFigures(const unsigned int indentationLevel, Figure * figures);
static void _generateProperties(const unsigned int indentationLevel, Property * properties, FigureType figureType);
static void _generateSymbols(const unsigned int indentationLevel, Scene * scene);
static void _generateUseInstances(const unsigned int indentationLevel, Scene * scene);
static void _generateTransforms(const unsigned int indentationLevel, Transform * transforms);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

// Helper for Z-Index Sorting
typedef enum { ITEM_FIGURE, ITEM_USE, ITEM_GROUP } RenderableType;
typedef struct {
    int zIndex;
    RenderableType type;
    void * item;
} RenderableItem;

static int _compareRenderables(const void * a, const void * b) {
    RenderableItem * itemA = (RenderableItem *)a;
    RenderableItem * itemB = (RenderableItem *)b;
    return itemA->zIndex - itemB->zIndex;
}

static void _generateGroupProperties(const unsigned int indentationLevel, Property * properties);
static void _generateGroup(const unsigned int indentationLevel, Group * group);

/**
 * Creates the epilogue of the generated output.
 */
static void _generateEpilogue(void) {
	fprintf(f, "</svg>\n");
}

/**
 * Generates the scene of the program.
 */
static void _generateScene(const unsigned int indentationLevel, Scene * scene){
    if (scene == NULL) return;

    _currentVariables = scene->variables;

    // 1. Generate Definitions (Symbols)
    _generateSymbols(indentationLevel, scene);

    // 2. Collect all renderable items
    int count = 0;
    
    // Count figures in scene (default Z=0)
    Figure * fig = scene->figures;
    while(fig) { count++; fig = fig->next; }
    
    // Count uses in scene (default Z=0)
    UseInstance * use = scene->uses;
    while(use) { count++; use = use->next; }
    
    // Count groups in scene (default Z=0)
    Group * group = scene->groups;
    while(group) { count++; group = group->next; }
    
    // Count figures in layers
    Layer * layer = scene->layers;
    while(layer) {
        fig = layer->figures;
        while(fig) { count++; fig = fig->next; }
        layer = layer->next;
    }

    if (count == 0) return;

    RenderableItem * items = (RenderableItem *)malloc(sizeof(RenderableItem) * count);
    int idx = 0;

    // Populate items
    // Scene figures (Z=0)
    fig = scene->figures;
    while(fig) {
        items[idx].zIndex = 0;
        items[idx].type = ITEM_FIGURE;
        items[idx].item = fig;
        idx++;
        fig = fig->next;
    }

    // Scene uses (Z=0)
    use = scene->uses;
    while(use) {
        items[idx].zIndex = 0;
        items[idx].type = ITEM_USE;
        items[idx].item = use;
        idx++;
        use = use->next;
    }
    
    // Scene groups (Z=0)
    group = scene->groups;
    while(group) {
        items[idx].zIndex = 0;
        items[idx].type = ITEM_GROUP;
        items[idx].item = group;
        idx++;
        group = group->next;
    }

    // Layer figures
    layer = scene->layers;
    while(layer) {
        fig = layer->figures;
        while(fig) {
            items[idx].zIndex = layer->zLevel;
            items[idx].type = ITEM_FIGURE;
            items[idx].item = fig;
            idx++;
            fig = fig->next;
        }
        layer = layer->next;
    }

    // 3. Sort by Z-Index
    qsort(items, count, sizeof(RenderableItem), _compareRenderables);

    // 4. Generate sorted items
    for (int i = 0; i < count; ++i) {
        if (items[i].type == ITEM_FIGURE) {
            _generateFigure(indentationLevel, (Figure *)items[i].item);
        } else if (items[i].type == ITEM_USE) {
             UseInstance * u = (UseInstance *)items[i].item;
             _output(indentationLevel, "<use href=\"#%s\"", u->symbolName);
             if (u->hasPosition) {
                 _output(0, " x=\"%d\" y=\"%d\"", u->posX, u->posY);
             }
             // UseInstance behaves like a Rectangle (x, y, width, height) for properties
             _generateProperties(indentationLevel, u->properties, RECTANGLE_FIGURE);
             _output(0, " />\n");
         } else if (items[i].type == ITEM_GROUP) {
             _generateGroup(indentationLevel, (Group *)items[i].item);
         }
    }

    free(items);
    _currentVariables = NULL;
}

/**
 * Generates the layers of the program.
 */
static void _generateLayers(const unsigned int indentationLevel, Layer * layers){
	// Not used directly as we flatten layers in _generateScene
	return;
}

static void _generateGroup(const unsigned int indentationLevel, Group * group) {
    if (!group) return;
    _output(indentationLevel, "<g");
    if (group->name) {
        _output(0, " id=\"%s\"", group->name);
    }
    _generateGroupProperties(indentationLevel, group->properties);
    _output(0, ">\n");
    
    _generateFigures(indentationLevel + 1, group->figures);
    
    _output(indentationLevel, "</g>\n");
}

static void _generateGroupProperties(const unsigned int indentationLevel, Property * properties) {
    // 1. Handle Transforms
    Property * prop = properties;
    bool hasTransform = false;
    while(prop) {
        if (prop->type == SCALE_PROPERTY || prop->type == ROTATE_PROPERTY || prop->type == TRANSLATE_PROPERTY) {
            hasTransform = true;
            break;
        }
        prop = prop->next;
    }
    
    if (hasTransform) {
        _output(0, " transform=\"");
        prop = properties;
        while(prop) {
            switch(prop->type) {
                case TRANSLATE_PROPERTY:
                    if (prop->isExpression && (prop->value.coordinates.xExpression || prop->value.coordinates.yExpression)) {
                         int tx = 0, ty = 0;
                         if (prop->value.coordinates.xExpression) {
                             ComputationResult r = computeExpression(prop->value.coordinates.xExpression, _currentVariables);
                             if(r.succeeded) tx = r.value;
                         }
                         if (prop->value.coordinates.yExpression) {
                             ComputationResult r = computeExpression(prop->value.coordinates.yExpression, _currentVariables);
                             if(r.succeeded) ty = r.value;
                         }
                         _output(0, "translate(%d, %d) ", tx, ty);
                    } else {
                        _output(0, "translate(%d, %d) ", prop->value.coordinates.x, prop->value.coordinates.y);
                    }
                    break;
                case ROTATE_PROPERTY:
                     if (prop->isExpression && prop->value.expressionValue) {
                         ComputationResult r = computeExpression(prop->value.expressionValue, _currentVariables);
                         if (r.succeeded) _output(0, "rotate(%d) ", r.value);
                     } else {
                         _output(0, "rotate(%.2f) ", prop->value.floatValue);
                     }
                     break;
                case SCALE_PROPERTY:
                     if (prop->isExpression && prop->value.expressionValue) {
                         ComputationResult r = computeExpression(prop->value.expressionValue, _currentVariables);
                         if (r.succeeded) _output(0, "scale(%d) ", r.value);
                     } else {
                         _output(0, "scale(%.2f) ", prop->value.scale.x);
                     }
                     break;
                default: break;
            }
            prop = prop->next;
        }
        _output(0, "\"");
    }
    
    // 2. Handle other properties (Opacity, etc.)
    prop = properties;
    while(prop) {
        switch(prop->type) {
            case OPACITY_PROPERTY:
                _output(0, " opacity=\"%.2f\"", prop->value.floatValue);
                break;
            // Add other group applicable properties if any
            default: break;
        }
        prop = prop->next;
    }
}

/**
 * Generates a single figure.
 */
static void _generateFigure(const unsigned int indentationLevel, Figure * figure){
    if (!figure) return;
    _output(indentationLevel, "<");
    switch(figure->type) {
        case RECTANGLE_FIGURE: _output(0, "rect"); break;
        case CIRCLE_FIGURE: _output(0, "circle"); break;
        case LINE_FIGURE: _output(0, "line"); break;
        case ELLIPSE_FIGURE: _output(0, "ellipse"); break;
        case POLYLINE_FIGURE: _output(0, "polyline"); break;
        case POLYGON_FIGURE: _output(0, "polygon"); break;
    }
    
    _generateProperties(indentationLevel, figure->properties, figure->type);
    _generateTransforms(indentationLevel, figure->transforms);
    
    _output(0, " />\n");
}

/**
 * Generates the figures of the program.
 */
static void _generateFigures(const unsigned int indentationLevel, Figure * figures){
    while(figures) {
         _generateFigure(indentationLevel, figures);
         figures = figures->next;
    }
}

static const char* _unitToString(UnitType unit) {
    switch(unit) {
        case UNIT_PX: return "px";
        case UNIT_REM: return "rem";
        case UNIT_EM: return "em";
        case UNIT_VW: return "vw";
        case UNIT_VH: return "vh";
        case UNIT_PERCENT: return "%";
        default: return "px";
    }
}

/**
 * Generates the properties of the program.
 */
static void _generateProperties(const unsigned int indentationLevel, Property * properties, FigureType figureType){
    while(properties) {
        switch(properties->type) {
            case POSITION_PROPERTY: {
                int x = properties->value.coordinates.x;
                int y = properties->value.coordinates.y;
                
                if (properties->isExpression) {
                    if (properties->value.coordinates.xExpression) {
                         ComputationResult r = computeExpression(properties->value.coordinates.xExpression, _currentVariables);
                         if (r.succeeded) x = r.value;
                    }
                    if (properties->value.coordinates.yExpression) {
                         ComputationResult r = computeExpression(properties->value.coordinates.yExpression, _currentVariables);
                         if (r.succeeded) y = r.value;
                    }
                }

                if (figureType == LINE_FIGURE) {
                    
                    break;
                }
                if (figureType == CIRCLE_FIGURE || figureType == ELLIPSE_FIGURE) {
                    _output(0, " cx=\"%d\" cy=\"%d\"", x, y);
                } else {
                    _output(0, " x=\"%d\" y=\"%d\"", x, y);
                }
                break;
            }
            case SIZE_PROPERTY:
                
                 if (figureType == ELLIPSE_FIGURE) {
                    _output(0, " rx=\"%d%s\" ry=\"%d%s\"", 
                        properties->value.dimensions.width / 2, _unitToString(properties->value.dimensions.widthUnit),
                        properties->value.dimensions.height / 2, _unitToString(properties->value.dimensions.heightUnit));
                 } else {
                     _output(0, " width=\"%d%s\" height=\"%d%s\"", 
                        properties->value.dimensions.width, _unitToString(properties->value.dimensions.widthUnit),
                        properties->value.dimensions.height, _unitToString(properties->value.dimensions.heightUnit));
                 }
                 break;
            case WIDTH_PROPERTY: {
                int w = properties->value.dimensions.width;
                if (properties->isExpression && properties->value.expressionValue) {
                     ComputationResult r = computeExpression(properties->value.expressionValue, _currentVariables);
                     if (r.succeeded) w = r.value;
                }
                if (figureType == ELLIPSE_FIGURE) {
                    _output(0, " rx=\"%d%s\"", 
                        w / 2, _unitToString(properties->value.dimensions.widthUnit));
                } else {
                    _output(0, " width=\"%d%s\"", 
                        w, _unitToString(properties->value.dimensions.widthUnit));
                }
                break;
            }
            case HEIGHT_PROPERTY: {
                int h = properties->value.dimensions.height;
                if (properties->isExpression && properties->value.expressionValue) {
                     ComputationResult r = computeExpression(properties->value.expressionValue, _currentVariables);
                     if (r.succeeded) h = r.value;
                }
                if (figureType == ELLIPSE_FIGURE) {
                    _output(0, " ry=\"%d%s\"", 
                        h / 2, _unitToString(properties->value.dimensions.heightUnit));
                } else {
                    _output(0, " height=\"%d%s\"", 
                        h, _unitToString(properties->value.dimensions.heightUnit));
                }
                break;
            }
            case FROM_PROPERTY: {
                int x1 = properties->value.coordinates.x;
                int y1 = properties->value.coordinates.y;
                if (properties->isExpression) {
                     if (properties->value.coordinates.xExpression) {
                         ComputationResult r = computeExpression(properties->value.coordinates.xExpression, _currentVariables);
                         if (r.succeeded) x1 = r.value;
                     }
                     if (properties->value.coordinates.yExpression) {
                         ComputationResult r = computeExpression(properties->value.coordinates.yExpression, _currentVariables);
                         if (r.succeeded) y1 = r.value;
                     }
                }
                _output(0, " x1=\"%d\" y1=\"%d\"", x1, y1);
                break;
            }
            case TO_PROPERTY: {
                int x2 = properties->value.coordinates.x;
                int y2 = properties->value.coordinates.y;
                if (properties->isExpression) {
                     if (properties->value.coordinates.xExpression) {
                         ComputationResult r = computeExpression(properties->value.coordinates.xExpression, _currentVariables);
                         if (r.succeeded) x2 = r.value;
                     }
                     if (properties->value.coordinates.yExpression) {
                         ComputationResult r = computeExpression(properties->value.coordinates.yExpression, _currentVariables);
                         if (r.succeeded) y2 = r.value;
                     }
                }
                _output(0, " x2=\"%d\" y2=\"%d\"", x2, y2);
                break;
            }
            case RADIUS_PROPERTY: {
                int r_val = properties->value.intValue;
                if (properties->isExpression && properties->value.expressionValue) {
                     ComputationResult r = computeExpression(properties->value.expressionValue, _currentVariables);
                     if (r.succeeded) r_val = r.value;
                }
                _output(0, " r=\"%d\"", r_val);
                break;
            }
            case FILL_PROPERTY:
                _output(0, " fill=\"");
                if (properties->value.colorValue->type == NAMED_COLOR) {
                     _output(0, "%s", properties->value.colorValue->value.name);
                } else if (properties->value.colorValue->type == HEX_COLOR_TYPE) {
                    _output(0, "%s", properties->value.colorValue->value.hex);
                } else if (properties->value.colorValue->type == RGB_COLOR_TYPE) {
                    _output(0, "rgb(%d,%d,%d)", properties->value.colorValue->value.rgb.r, properties->value.colorValue->value.rgb.g, properties->value.colorValue->value.rgb.b);
                } else if (properties->value.colorValue->type == RGBA_COLOR_TYPE) {
                    _output(0, "rgba(%d,%d,%d,%.2f)", properties->value.colorValue->value.rgba.r, properties->value.colorValue->value.rgba.g, properties->value.colorValue->value.rgba.b, properties->value.colorValue->value.rgba.a);
                }
                _output(0, "\"");
                break;
            case STROKE_PROPERTY:
                _output(0, " stroke=\"");
                if (properties->value.colorValue->type == NAMED_COLOR) {
                     _output(0, "%s", properties->value.colorValue->value.name);
                } else if (properties->value.colorValue->type == HEX_COLOR_TYPE) {
                    _output(0, "%s", properties->value.colorValue->value.hex);
                } else if (properties->value.colorValue->type == RGB_COLOR_TYPE) {
                    _output(0, "rgb(%d,%d,%d)", properties->value.colorValue->value.rgb.r, properties->value.colorValue->value.rgb.g, properties->value.colorValue->value.rgb.b);
                } else if (properties->value.colorValue->type == RGBA_COLOR_TYPE) {
                    _output(0, "rgba(%d,%d,%d,%.2f)", properties->value.colorValue->value.rgba.r, properties->value.colorValue->value.rgba.g, properties->value.colorValue->value.rgba.b, properties->value.colorValue->value.rgba.a);
                }
                _output(0, "\"");
                 break;
            case STROKE_WIDTH_PROPERTY: {
                 int sw = properties->value.intValue;
                 if (properties->isExpression && properties->value.expressionValue) {
                     ComputationResult r = computeExpression(properties->value.expressionValue, _currentVariables);
                     if (r.succeeded) sw = r.value;
                 }
                 _output(0, " stroke-width=\"%d\"", sw);
                 break;
            }
            case OPACITY_PROPERTY: {
                float op = properties->value.floatValue;
                 if (properties->isExpression && properties->value.expressionValue) {
                     ComputationResult r = computeExpression(properties->value.expressionValue, _currentVariables);
                    
                     if (r.succeeded) op = (float)r.value;
                 }
                _output(0, " opacity=\"%.2f\"", op);
                break;
            }
            default: break;
        }
        properties = properties->next;
    }
}

/**
 * Generates the symbols of the program.
 */
static void _generateSymbols(const unsigned int indentationLevel, Scene * scene){
    if (!scene->symbols) return;
    
    _output(indentationLevel, "<defs>\n");
    Symbol * sym = scene->symbols;
    while(sym) {
        _output(indentationLevel + 1, "<symbol id=\"%s\">\n", sym->name);
        // Generate figures inside symbol
        _generateFigures(indentationLevel + 2, &sym->figure);
        _output(indentationLevel + 1, "</symbol>\n");
        sym = sym->next;
    }
    _output(indentationLevel, "</defs>\n");
}

/**
 * Generates the use instances of the program.
 */
static void _generateUseInstances(const unsigned int indentationLevel, Scene * scene){
	// Handled in _generateScene
	return;
}

/**
 * Generates the transforms of the program.
 */
static void _generateTransforms(const unsigned int indentationLevel, Transform * transforms){
    if (!transforms) return;
    _output(0, " transform=\"");
    while(transforms) {
        switch(transforms->type) {
            case TRANSFORM_TRANSLATE:
                if (transforms->isExpression) {
                     int tx = 0, ty = 0;
                     if (transforms->value.translate.txExp) {
                         ComputationResult r = computeExpression(transforms->value.translate.txExp, _currentVariables);
                         if (r.succeeded) tx = r.value;
                     }
                     if (transforms->value.translate.tyExp) {
                         ComputationResult r = computeExpression(transforms->value.translate.tyExp, _currentVariables);
                         if (r.succeeded) ty = r.value;
                     }
                     _output(0, "translate(%d, %d) ", tx, ty);
                } else {
                    _output(0, "translate(%.2f, %.2f) ", transforms->value.translate.tx, transforms->value.translate.ty);
                }
                break;
            case TRANSFORM_ROTATE:
                if (transforms->isExpression) {
                     int deg = 0;
                     if (transforms->value.rotate.degreesExp) {
                         ComputationResult r = computeExpression(transforms->value.rotate.degreesExp, _currentVariables);
                         if (r.succeeded) deg = r.value;
                     }
                     _output(0, "rotate(%d) ", deg);
                } else {
                    _output(0, "rotate(%.2f) ", transforms->value.rotate.degrees);
                }
                break;
            case TRANSFORM_SCALE:
                if (transforms->isExpression) {
                     int s = 1;
                     if (transforms->value.scale.sxExp) {
                         ComputationResult r = computeExpression(transforms->value.scale.sxExp, _currentVariables);
                         if (r.succeeded) s = r.value;
                     }
                     _output(0, "scale(%d) ", s);
                } else {
                    _output(0, "scale(%.2f) ", transforms->value.scale.sx);
                }
                break;
        }
        transforms = transforms->next;
    }
    _output(0, "\"");
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    if (program == NULL || program->scene == NULL) return;
    _generateScene(1, program->scene);
}

/**
 * Creates the prologue of the generated output.
 */
static void _generatePrologue(void) {
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">\n");
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(f, effectiveFormat, arguments); // Write to file 'f' instead of stdout
	fflush(f);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void executeGenerator(CompilerState * compilerState) {
	if (!_logger) {
    _logger = createLogger("Generator");
  }

	logDebugging(_logger, "Generating final output...");

	if (!compilerState || !compilerState->abstractSyntaxtTree) {
    logError(_logger, "No abstract syntax tree available in compiler state.");
    return;
  }

	f = fopen("scene.svg", "w"); // Changed extension to .svg
  if (!f) {
    logError(_logger, "Cannot open scene.svg for writing.");
    return;
  }

	//Generate the prologue of the output
	_generatePrologue();

	//Generate the main content of the output
	_generateProgram(compilerState->abstractSyntaxtTree);

	//Generate the epilogue of the output
	_generateEpilogue();
	fclose(f);
	logDebugging(_logger, "Generation is done. File scene.svg created successfully.");
}


