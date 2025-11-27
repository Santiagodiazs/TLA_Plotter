#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Internal Structures */

typedef struct PaletteColorNode {
    char * name;
    Color * color;
    struct PaletteColorNode * next;
} PaletteColorNode;

typedef struct PaletteNode {
    char * name;
    PaletteColorNode * colors;
    struct PaletteNode * next;
} PaletteNode;

typedef struct SymbolNode {
    char * name;
    Symbol * symbol;
    struct SymbolNode * next;
} SymbolNode;

typedef struct LayerNode {
    char * name;
    int zIndex;
    struct LayerNode * next;
} LayerNode;

struct SymbolTable {
    PaletteNode * palettes;
    SymbolNode * symbols;
    LayerNode * layers;
};

/* Public Functions */

SymbolTable * createSymbolTable() {
    SymbolTable * table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (table) {
        table->palettes = NULL;
        table->symbols = NULL;
        table->layers = NULL;
    }
    return table;
}

void destroySymbolTable(SymbolTable * symbolTable) {
    if (!symbolTable) return;

    // Free Palettes
    PaletteNode * currentPalette = symbolTable->palettes;
    while (currentPalette) {
        PaletteNode * next = currentPalette->next;
        free(currentPalette->name);
        
        // Free Colors
        PaletteColorNode * currentColor = currentPalette->colors;
        while (currentColor) {
            PaletteColorNode * nextColor = currentColor->next;
            free(currentColor->name);
            destroyColor(currentColor->color);
            free(currentColor);
            currentColor = nextColor;
        }
        
        free(currentPalette);
        currentPalette = next;
    }

    // Free Symbols
    SymbolNode * currentSymbol = symbolTable->symbols;
    while (currentSymbol) {
        SymbolNode * next = currentSymbol->next;
        free(currentSymbol->name);
        free(currentSymbol);
        currentSymbol = next;
    }

    // Free Layers
    LayerNode * currentLayer = symbolTable->layers;
    while (currentLayer) {
        LayerNode * next = currentLayer->next;
        free(currentLayer->name);
        free(currentLayer);
        currentLayer = next;
    }

    free(symbolTable);
}

bool addPalette(SymbolTable * symbolTable, char * name) {
    if (!symbolTable || !name) return false;

    // Check for duplicates
    PaletteNode * current = symbolTable->palettes;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return false; // Already exists
        }
        current = current->next;
    }

    // Add new palette
    PaletteNode * newNode = (PaletteNode *)malloc(sizeof(PaletteNode));
    newNode->name = strdup(name);
    newNode->colors = NULL;
    newNode->next = symbolTable->palettes;
    symbolTable->palettes = newNode;
    return true;
}

bool addPaletteColor(SymbolTable * symbolTable, char * paletteName, char * colorName, Color * color) {
    if (!symbolTable || !paletteName || !colorName || !color) return false;

    // Find palette
    PaletteNode * palette = symbolTable->palettes;
    while (palette) {
        if (strcmp(palette->name, paletteName) == 0) {
            break;
        }
        palette = palette->next;
    }
    
    if (!palette) return false; // Palette not found

    // Check for duplicate color in palette
    PaletteColorNode * currentColor = palette->colors;
    while (currentColor) {
        if (strcmp(currentColor->name, colorName) == 0) {
            return false; // Already exists
        }
        currentColor = currentColor->next;
    }

    // Add new color
    PaletteColorNode * newColor = (PaletteColorNode *)malloc(sizeof(PaletteColorNode));
    newColor->name = strdup(colorName);
    newColor->color = duplicateColor(color); // Store a copy
    newColor->next = palette->colors;
    palette->colors = newColor;
    return true;
}

Color * getPaletteColor(SymbolTable * symbolTable, char * paletteName, char * colorName) {
    if (!symbolTable || !paletteName || !colorName) return NULL;

    PaletteNode * palette = symbolTable->palettes;
    while (palette) {
        if (strcmp(palette->name, paletteName) == 0) {
            // Find color
            PaletteColorNode * colorNode = palette->colors;
            while (colorNode) {
                if (strcmp(colorNode->name, colorName) == 0) {
                    return colorNode->color;
                }
                colorNode = colorNode->next;
            }
            return NULL; // Color not found in palette
        }
        palette = palette->next;
    }
    return NULL; // Palette not found
}

bool addSymbol(SymbolTable * symbolTable, char * name, Symbol * symbolNode) {
    if (!symbolTable || !name) return false;

    // Check for duplicates
    SymbolNode * current = symbolTable->symbols;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return false; // Already exists
        }
        current = current->next;
    }

    // Add new symbol
    SymbolNode * newNode = (SymbolNode *)malloc(sizeof(SymbolNode));
    newNode->name = strdup(name);
    newNode->symbol = symbolNode;
    newNode->next = symbolTable->symbols;
    symbolTable->symbols = newNode;
    return true;
}

Symbol * getSymbol(SymbolTable * symbolTable, char * name) {
    if (!symbolTable || !name) return NULL;

    SymbolNode * current = symbolTable->symbols;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->symbol;
        }
        current = current->next;
    }
    return NULL;
}

bool addLayer(SymbolTable * symbolTable, char * name, int zIndex) {
    if (!symbolTable || !name) return false;

    // Check for duplicates
    LayerNode * current = symbolTable->layers;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return false; // Already exists
        }
        current = current->next;
    }

    // Add new layer
    LayerNode * newNode = (LayerNode *)malloc(sizeof(LayerNode));
    newNode->name = strdup(name);
    newNode->zIndex = zIndex;
    newNode->next = symbolTable->layers;
    symbolTable->layers = newNode;
    return true;
}

int getLayerZIndex(SymbolTable * symbolTable, char * name) {
    if (!symbolTable || !name) return -1;

    LayerNode * current = symbolTable->layers;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->zIndex;
        }
        current = current->next;
    }
    return -1;
}

bool hasLayerWithZIndex(SymbolTable * symbolTable, int zIndex) {
    if (!symbolTable) return false;

    LayerNode * current = symbolTable->layers;
    while (current) {
        if (current->zIndex == zIndex) {
            return true;
        }
        current = current->next;
    }
    return false;
}
