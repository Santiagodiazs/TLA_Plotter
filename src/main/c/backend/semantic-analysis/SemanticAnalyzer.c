#include "SemanticAnalyzer.h"
#include "../../backend/semantic-analysis/SymbolTable.h"
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
        // If it's just an expression (calculator), we skip semantic analysis for now or handle it differently
        // But the plan says we are doing SVG generation for Scenes.
        // Let's assume valid for expression programs or just ignore.
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
        if (!addPalette(state->symbolTable, palette->name, 0)) { // We don't strictly need the color value for validation, just existence
             // Actually, we might want to store the color if we were doing more complex checks, but for now just name existence.
             // Wait, addPalette returns false if it already exists.
             state->isValid = false;
             state->errorMessage = "Duplicate palette name defined.";
             return;
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
        // Validate figures inside the symbol
        // Note: Symbol struct has 'Figure figure' (not pointer) in AST?
        // Let's check AST definition.
        // struct Symbol { char * name; Figure figure; struct Symbol * next; };
        // It seems 'figure' is a single Figure struct, not a pointer?
        // Wait, Figure struct has 'struct Figure * next'.
        // So 'Figure figure' is the head of the list? Or just one figure?
        // Usually it's a list pointer.
        // If it is 'Figure figure', then it is a struct instance.
        // We can pass the address of it if _validateFigures expects a pointer.
        // But _validateFigures iterates using ->next.
        // So passing &symbols->figure is correct if it represents the head of the list.
        
        _validateFigures(&symbols->figure, state); 
        if (!state->isValid) return;

        symbols = symbols->next;
    }
}

static void _validateFigures(Figure * figures, CompilerState * state) {
    while (figures) {
        _validateProperties(figures->properties, state);
        if (!state->isValid) return;
        figures = figures->next;
    }
}

static void _validateProperties(Property * properties, CompilerState * state) {
    while (properties) {
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
                         // Assuming intValue for now as it's a single value
                         if (properties->value.intValue < 0) {
                             state->isValid = false;
                             state->errorMessage = "Width must be non-negative.";
                             return;
                         }
                     } else if (properties->type == HEIGHT_PROPERTY) {
                         if (properties->value.intValue < 0) {
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
        
        
        if (strchr(color->value.name, '.') != NULL) {
            if (getPalette(state->symbolTable, color->value.name) == -1) {
                 state->isValid = false;
                 state->errorMessage = "Undefined palette color reference.";
                 return;
            }
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
