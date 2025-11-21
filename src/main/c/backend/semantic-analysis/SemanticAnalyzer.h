#ifndef SEMANTIC_ANALYZER_HEADER
#define SEMANTIC_ANALYZER_HEADER

#include "../../support/type/CompilerState.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Validates the AST ensuring all semantic rules are met.
 * Updates the CompilerState with the validation result.
 */
void validateAST(CompilerState * compilerState);

#endif
