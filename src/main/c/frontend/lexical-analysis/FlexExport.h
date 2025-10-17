#ifndef FLEX_EXPORT_HEADER
#define FLEX_EXPORT_HEADER

#include "../../support/type/FlexContext.h"
#include "../../support/type/LexicalAnalyzer.h"
#include "FlexScanner.h"
#include <stdbool.h>

// Hook para exportar funciones estáticas de Flex


FlexContext flexCurrentContext(LexicalAnalyzer * lexicalAnalyzer);
void flexEnterContext(LexicalAnalyzer * lexicalAnalyzer, FlexContext flexContext);
bool flexHasBuffer(LexicalAnalyzer * lexicalAnalyzer);
void flexLeaveContext(LexicalAnalyzer * lexicalAnalyzer);

#endif
