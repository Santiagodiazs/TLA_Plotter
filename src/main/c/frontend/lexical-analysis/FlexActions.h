#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../support/configuration/Environment.h"
#include "../../support/language/String.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/LexicalAnalyzer.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/Token.h"
#include "../../support/type/TokenLabel.h"
#include "../Frontend.h"
#include "../syntactic-analysis/BisonParser.h"

/** Initialize module's internal state. */
ModuleDestructor initializeFlexActionsModule();

CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
CompilationStatus EnterImportExpressionLexemeAction(FlexContext context);
CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context);
CompilationStatus EOFLexemeAction();
CompilationStatus IgnoredLexemeAction();
CompilationStatus IntegerLexemeAction();
CompilationStatus LeaveImportExpressionLexemeAction();
CompilationStatus LeaveMultilineCommentLexemeAction();
CompilationStatus EnterOnelineCommentLexemeAction(FlexContext context);
CompilationStatus LeaveOnelineCommentLexemeAction();
CompilationStatus EnterStringContextLexemeAction(FlexContext context);
CompilationStatus LeaveStringContextLexemeAction();
CompilationStatus SemicolonAndLeaveStringContextLexemeAction();
CompilationStatus ParenthesisLexemeAction(TokenLabel label);
CompilationStatus SubexpressionLexemeAction();
CompilationStatus UnknownLexemeAction();

// Acciones específicas del DSL
CompilationStatus KeywordLexemeAction(TokenLabel label);
CompilationStatus IdentifierLexemeAction();
CompilationStatus StringLexemeAction();
CompilationStatus DecimalLexemeAction();
CompilationStatus DimensionsLexemeAction();

CompilationStatus ColorLexemeAction(TokenLabel label);
CompilationStatus BraceLexemeAction(TokenLabel label);
CompilationStatus SemicolonLexemeAction();
CompilationStatus CommaLexemeAction();
CompilationStatus ColonLexemeAction();
CompilationStatus DotLexemeAction();

#endif
