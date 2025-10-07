#ifndef FLEX_EXPORT_HEADER
#define FLEX_EXPORT_HEADER

#include "../../support/type/FlexContext.h"
#include "../../support/type/LexicalAnalyzer.h"
#include "FlexScanner.h"
#include <stdbool.h>

// Hook para exportar funciones estáticas de Flex


FlexContext flexCurrentContext(LexicalAnalyzer * lexicalAnalyzer) {
	// struct yyguts_t * yyg = (struct yyguts_t *) lexicalAnalyzer->scanner;
	// return YYSTATE;
	
	// TODO: Implementar obtención del estado actual
	return 0;
}

void flexEnterContext(LexicalAnalyzer * lexicalAnalyzer, FlexContext flexContext) {
	// struct yyguts_t * yyg = (struct yyguts_t *) lexicalAnalyzer->scanner;
	// yy_push_state(flexContext);
	
	// TODO: Implementar cambio de contexto
}

bool flexHasBuffer(LexicalAnalyzer * lexicalAnalyzer) {
	// struct yyguts_t * yyg = (struct yyguts_t *) lexicalAnalyzer->scanner;
	// return YY_CURRENT_BUFFER != NULL;
	
	// TODO: Implementar verificación de buffer
	return true;
}

void flexLeaveContext(LexicalAnalyzer * lexicalAnalyzer) {
	// struct yyguts_t * yyg = (struct yyguts_t *) lexicalAnalyzer->scanner;
	// yy_pop_state();
	
	// TODO: Implementar salida de contexto
}

#endif
