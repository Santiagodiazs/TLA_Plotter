#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef struct SymbolTable SymbolTable;

/**
 * Creates a new symbol table.
 */
SymbolTable * createSymbolTable();

/**
 * Destroys a symbol table.
 */
void destroySymbolTable(SymbolTable * symbolTable);

/**
 * Adds a palette to the symbol table.
 * Returns true if successful, false if the palette already exists.
 */
bool addPalette(SymbolTable * symbolTable, char * name, int color);

/**
 * Gets a palette color by name.
 * Returns the color value or -1 if not found.
 */
int getPalette(SymbolTable * symbolTable, char * name);

/**
 * Adds a symbol definition to the symbol table.
 * Returns true if successful, false if the symbol already exists.
 */
bool addSymbol(SymbolTable * symbolTable, char * name, Symbol * symbolNode);

/**
 * Gets a symbol definition by name.
 * Returns the Symbol node or NULL if not found.
 */
Symbol * getSymbol(SymbolTable * symbolTable, char * name);

/**
 * Adds a layer to the symbol table.
 * Returns true if successful, false if the layer already exists.
 */
bool addLayer(SymbolTable * symbolTable, char * name, int zIndex);

/**
 * Gets a layer's Z-index by name.
 * Returns the Z-index or -1 if not found.
 */
int getLayerZIndex(SymbolTable * symbolTable, char * name);

/**
 * Checks if a layer with the given Z-index already exists.
 */
bool hasLayerWithZIndex(SymbolTable * symbolTable, int zIndex);

#endif
