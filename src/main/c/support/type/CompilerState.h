#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include <stdbool.h>
#include "../../backend/semantic-analysis/SymbolTable.h"

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	/**
	 * The root node of the AST.
	 */
	void * abstractSyntaxtTree;

	/**
	 * The symbol table for semantic analysis and code generation.
	 */
	SymbolTable * symbolTable;

	/**
	 * Validation status.
	 */
	bool isValid;
	char * errorMessage;

} CompilerState;

#endif
