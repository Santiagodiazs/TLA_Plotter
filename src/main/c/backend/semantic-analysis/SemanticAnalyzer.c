#include "SemanticAnalyzer.h"
#include "../../backend/semantic-analysis/SymbolTable.h"
#include "../../backend/domain-specific/Calculator.h"
#include <string.h>
#include <stdio.h>

/* Private Functions */

static void _validateScene(Scene * scene, CompilerState * state);
static void _validateLayers(Layer * layers, CompilerState * state);
static void _validatePalettes(PaletteEntry * palette, CompilerState * state);
static void _validateSymbols(Symbol * symbols, CompilerState * state);
static void _validateFigures(Figure * figures, CompilerState * state);
static void _validateProperties(Property * properties, CompilerState * state);
static void _validateUseInstances(UseInstance * uses, CompilerState * state);
static void _validateColor(Color * color, CompilerState * state);

/* Public Functions */

void validateAST(CompilerState * compilerState) {
    if (!compilerState || !compilerState->abstractSyntaxtTree) {
        compilerState->isValid = false;
        compilerState->errorMessage = "No AST to validate.";
        return;
    }

    Program * program = (Program *)compilerState->abstractSyntaxtTree;
    if (program->type != SCENE_PROGRAM) {
        compilerState->isValid = true;
        return;
    }

    // Initialize Symbol Table
    if (compilerState->symbolTable) {
        destroySymbolTable(compilerState->symbolTable);
    }
    compilerState->symbolTable = createSymbolTable();
    compilerState->isValid = true; // Assume valid until proven otherwise
    compilerState->errorMessage = NULL;

    _validateScene(program->scene, compilerState);
}

/* Private Functions Implementation */

static void _validateScene(Scene * scene, CompilerState * state) {
    if (!state->isValid) return;

    // 1. Validate Palettes (Register them)
    _validatePalettes(scene->palette, state);
    if (!state->isValid) return;

    // 2. Validate Layers (Check Z-Index collisions)
    _validateLayers(scene->layers, state);
    if (!state->isValid) return;

    // 3. Validate Symbols (Register them)
    _validateSymbols(scene->symbols, state);
    if (!state->isValid) return;

    // 4. Validate Figures (Check properties and colors)
    _validateFigures(scene->figures, state);
    if (!state->isValid) return;

    // 5. Validate Use Instances (Check symbol existence)
    _validateUseInstances(scene->uses, state);
    if (!state->isValid) return;
    
    // 6. Validate Groups (Recursively validate figures inside groups)
    Group * group = scene->groups;
    while(group) {
        _validateFigures(group->figures, state);
        if (!state->isValid) return;
        group = group->next;
    }
}

static void _validatePalettes(PaletteEntry * palette, CompilerState * state) {
    while (palette) {
        // If palette has a name (it's a named palette block entry)
        if (palette->paletteName) {
            addPalette(state->symbolTable, palette->paletteName);
            
            if (!addPaletteColor(state->symbolTable, palette->paletteName, palette->name, palette->color)) {
            }
        } else {
            
        }
        palette = palette->next;
    }
}

static void _validateLayers(Layer * layers, CompilerState * state) {
    while (layers) {
        if (hasLayerWithZIndex(state->symbolTable, layers->zLevel)) {
            state->isValid = false;
            state->errorMessage = "Duplicate Layer Z-Index detected.";
            return;
        }
        addLayer(state->symbolTable, layers->name, layers->zLevel);
        
        // Validate figures inside the layer
        _validateFigures(layers->figures, state);
        if (!state->isValid) return;

        layers = layers->next;
    }
}

static void _validateSymbols(Symbol * symbols, CompilerState * state) {
    while (symbols) {
        if (!addSymbol(state->symbolTable, symbols->name, symbols)) {
            state->isValid = false;
            state->errorMessage = "Duplicate Symbol name defined.";
            return;
        }
        
        _validateFigures(&symbols->figure, state); 
        if (!state->isValid) return;

        symbols = symbols->next;
    }
}

static void _validateFigureProperties(Figure * figure, CompilerState * state);

static void _validateFigures(Figure * figures, CompilerState * state) {
    while (figures) {
        _validateProperties(figures->properties, state);
        if (!state->isValid) return;
        
        _validateFigureProperties(figures, state);
        if (!state->isValid) return;
        
        figures = figures->next;
    }
}

static void _validateFigureProperties(Figure * figure, CompilerState * state) {
    bool hasWidth = false;
    bool hasHeight = false;
    bool hasSize = false;
    bool hasRadius = false;
    bool hasPoints = false; 
    
    Property * prop = figure->properties;
    while (prop) {
        // Check property-figure type compatibility
        switch (prop->type) {
            case WIDTH_PROPERTY:
                hasWidth = true;
                if (figure->type == CIRCLE_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'width' is not valid for circle figures.";
                    return;
                } else if (figure->type == LINE_FIGURE || figure->type == POLYLINE_FIGURE || figure->type == POLYGON_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'width' is not valid for line/polyline/polygon figures.";
                    return;
                }
                break;
            case HEIGHT_PROPERTY:
                hasHeight = true;
                if (figure->type == CIRCLE_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'height' is not valid for circle figures.";
                    return;
                } else if (figure->type == LINE_FIGURE || figure->type == POLYLINE_FIGURE || figure->type == POLYGON_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'height' is not valid for line/polyline/polygon figures.";
                    return;
                }
                break;
            case SIZE_PROPERTY:
                hasSize = true;
                if (figure->type == CIRCLE_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'size' is not valid for circle figures.";
                    return;
                } else if (figure->type == LINE_FIGURE || figure->type == POLYLINE_FIGURE || figure->type == POLYGON_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'size' is not valid for line/polyline/polygon figures.";
                    return;
                }
                break;
            case RADIUS_PROPERTY:
                hasRadius = true;
                if (figure->type == RECTANGLE_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'radius' is not valid for rectangle figures.";
                    return;
                } else if (figure->type == LINE_FIGURE || figure->type == POLYLINE_FIGURE || figure->type == POLYGON_FIGURE) {
                    state->isValid = false;
                    state->errorMessage = "Property 'radius' is not valid for line/polyline/polygon figures.";
                    return;
                }
                break;
            default:
                break;
        }
        prop = prop->next;
    }
    
    // Check for conflicting size declarations
    if (hasSize && (hasWidth || hasHeight)) {
        state->isValid = false;
        state->errorMessage = "Cannot use both 'size' and 'width'/'height' properties.";
        return;
    }
    
    // Check required properties and width/height pairing
    switch (figure->type) {
        case RECTANGLE_FIGURE:
            if (!hasSize && !hasWidth && !hasHeight) {
                state->isValid = false;
                state->errorMessage = "Rectangle must have 'size' or both 'width' and 'height'.";
            } else if (!hasSize && (hasWidth != hasHeight)) {
                state->isValid = false;
                state->errorMessage = "Must specify both 'width' and 'height' when not using 'size'.";
            }
            break;
        case ELLIPSE_FIGURE:
             if (!hasSize && !hasWidth && !hasHeight) {
                state->isValid = false;
                state->errorMessage = "Ellipse must have 'size' or both 'width' and 'height' (rx/ry).";
            } else if (!hasSize && (hasWidth != hasHeight)) {
                state->isValid = false;
                state->errorMessage = "Must specify both 'width' and 'height' when not using 'size'.";
            }
            break;
        case CIRCLE_FIGURE:
            if (!hasRadius) {
                state->isValid = false;
                state->errorMessage = "Circle must have 'radius'.";
            }
            break;
        default:
            break;
    }
}

static void _validateProperties(Property * properties, CompilerState * state) {
    while (properties) {
        // Evaluate expression if present
        if (properties->isExpression && properties->value.expressionValue) {
            ComputationResult result = computeExpression(properties->value.expressionValue);
            if (!result.succeeded) {
                state->isValid = false;
                state->errorMessage = "Failed to evaluate expression (e.g., division by zero).";
                return;
            }
            
            destroyExpression(properties->value.expressionValue);
            properties->value.expressionValue = NULL;
            properties->isExpression = false;
            
            // Assign result to appropriate value field based on property type
            switch (properties->type) {
                case WIDTH_PROPERTY:
                case HEIGHT_PROPERTY:
                    if (properties->type == WIDTH_PROPERTY) {
                        properties->value.dimensions.width = result.value;
                    } else {
                        properties->value.dimensions.height = result.value;
                    }
                    break;
                case RADIUS_PROPERTY:
                case STROKE_WIDTH_PROPERTY:
                    properties->value.intValue = result.value;
                    break;
                case SCALE_PROPERTY:
                case ROTATE_PROPERTY:
                case OPACITY_PROPERTY:
                    properties->value.floatValue = (float)result.value;
                    break;
                default:
                    // Should not happen for other properties if grammar is correct
                    break;
            }
        }

        switch (properties->type) {
            case OPACITY_PROPERTY:
                if (properties->value.floatValue < 0.0f || properties->value.floatValue > 1.0f) {
                    state->isValid = false;
                    state->errorMessage = "Opacity must be between 0.0 and 1.0.";
                    return;
                }
                break;
            case WIDTH_PROPERTY:
            case HEIGHT_PROPERTY:
            case RADIUS_PROPERTY:
            case SIZE_PROPERTY: 
 
                 if (properties->type == WIDTH_PROPERTY || properties->type == HEIGHT_PROPERTY) {
                    
                     
                     if (properties->type == SIZE_PROPERTY) {
                         if (properties->value.dimensions.width < 0 || properties->value.dimensions.height < 0) {
                             state->isValid = false;
                             state->errorMessage = "Dimensions must be non-negative.";
                             return;
                         }
                     } else if (properties->type == WIDTH_PROPERTY) {
                         if (properties->value.dimensions.width < 0) {
                             state->isValid = false;
                             state->errorMessage = "Width must be non-negative.";
                             return;
                         }
                     } else if (properties->type == HEIGHT_PROPERTY) {
                         if (properties->value.dimensions.height < 0) {
                             state->isValid = false;
                             state->errorMessage = "Height must be non-negative.";
                             return;
                         }
                     } else if (properties->type == RADIUS_PROPERTY) {
                         if (properties->value.intValue < 0) {
                             state->isValid = false;
                             state->errorMessage = "Radius must be non-negative.";
                             return;
                         }
                     }
                 }
                 break;
            case FILL_PROPERTY:
            case STROKE_PROPERTY:
                if (properties->value.colorValue) {
                    _validateColor(properties->value.colorValue, state);
                    if (!state->isValid) return;
                }
                break;
            default:
                break;
        }
        properties = properties->next;
    }
}

static void _validateColor(Color * color, CompilerState * state) {
    if (color->type == NAMED_COLOR) {
        char * dot = strchr(color->value.name, '.');
        if (dot != NULL) {
            // Parse "Palette.Color"
            int paletteNameLen = dot - color->value.name;
            char * paletteName = malloc(paletteNameLen + 1);
            strncpy(paletteName, color->value.name, paletteNameLen);
            paletteName[paletteNameLen] = '\0';
            
            char * colorName = dot + 1;
            
            if (getPaletteColor(state->symbolTable, paletteName, colorName) == NULL) {
                 state->isValid = false;
                 state->errorMessage = "Undefined palette color reference.";
                 free(paletteName);
                 return;
            }
            free(paletteName);
        }
    } else if (color->type == RGB_COLOR_TYPE) {
        if (color->value.rgb.r < 0 || color->value.rgb.r > 255 ||
            color->value.rgb.g < 0 || color->value.rgb.g > 255 ||
            color->value.rgb.b < 0 || color->value.rgb.b > 255) {
            state->isValid = false;
            state->errorMessage = "RGB values must be between 0 and 255.";
            return;
        }
    } else if (color->type == RGBA_COLOR_TYPE) {
         if (color->value.rgba.r < 0 || color->value.rgba.r > 255 ||
            color->value.rgba.g < 0 || color->value.rgba.g > 255 ||
            color->value.rgba.b < 0 || color->value.rgba.b > 255 ||
            color->value.rgba.a < 0.0f || color->value.rgba.a > 1.0f) {
            state->isValid = false;
            state->errorMessage = "RGBA values invalid.";
            return;
        }
    }
}

static void _validateUseInstances(UseInstance * uses, CompilerState * state) {
    while (uses) {
        if (getSymbol(state->symbolTable, uses->symbolName) == NULL) {
            state->isValid = false;
            state->errorMessage = "Undefined symbol used.";
            return;
        }
        uses = uses->next;
    }
}
