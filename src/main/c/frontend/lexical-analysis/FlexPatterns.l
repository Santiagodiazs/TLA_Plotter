%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{



extern CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
extern CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context);
extern CompilationStatus EnterOnelineCommentLexemeAction(FlexContext context);
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus LeaveMultilineCommentLexemeAction();
extern CompilationStatus LeaveOnelineCommentLexemeAction();
extern CompilationStatus EnterStringContextLexemeAction(FlexContext context);
extern CompilationStatus LeaveStringContextLexemeAction();
extern CompilationStatus SemicolonAndLeaveStringContextLexemeAction();
extern CompilationStatus ParenthesisLexemeAction(TokenLabel label);
extern CompilationStatus UnknownLexemeAction();

// Acciones específicas del DSL 
extern CompilationStatus KeywordLexemeAction(TokenLabel label);
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus StringLexemeAction();
extern CompilationStatus DecimalLexemeAction();
extern CompilationStatus DimensionsLexemeAction();

extern CompilationStatus ColorLexemeAction(TokenLabel label);
extern CompilationStatus BraceLexemeAction(TokenLabel label);
extern CompilationStatus SemicolonLexemeAction();
extern CompilationStatus CommaLexemeAction();
extern CompilationStatus ColonLexemeAction();
extern CompilationStatus DotLexemeAction();

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x ONELINE_COMMENT
%s STRING

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+

/* Patrones específicos del DSL */ 
/* Patrones específicos del DSL */ 
identifier          [a-zA-Z_][a-zA-Z0-9_]*
integer             [0-9]+
decimal             [0-9]+\.[0-9]+

WS                  [ \t\r\n\f\v]*
INT                 [0-9]+
FLOAT               ([0-9]+(\.[0-9]+)?|\.[0-9]+)
hex_color           #([0-9A-Fa-f]{6}|[0-9A-Fa-f]{3})
string              \"[^\"]*\"
NUM                 ({decimal}|{integer})

%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

"/*"								{ return EnterMultilineCommentLexemeAction(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"				{ return LeaveMultilineCommentLexemeAction(); }
<MULTILINE_COMMENT>[^*]+			{ return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>.				{ return IgnoredLexemeAction(); }

"//"								{ return EnterOnelineCommentLexemeAction(ONELINE_COMMENT); }
<ONELINE_COMMENT>"\n"				{ return LeaveOnelineCommentLexemeAction(); }
<ONELINE_COMMENT>[^\n]+			{ return IgnoredLexemeAction(); }

<STRING>";"							{ return SemicolonAndLeaveStringContextLexemeAction(); }
<STRING>[^;]							{ return IgnoredLexemeAction(); }
"-"									{ return ArithmeticOperatorLexemeAction(SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(ADD); }

"("									{ return ParenthesisLexemeAction(OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(CLOSE_PARENTHESIS); }

"scene"								{ return KeywordLexemeAction(SCENE); }
"draw"								{ return KeywordLexemeAction(DRAW); }
"with"								{ return KeywordLexemeAction(WITH); }
"layer"								{ return KeywordLexemeAction(LAYER); }
"group"								{ return KeywordLexemeAction(GROUP); }
"symbol"							{ return KeywordLexemeAction(SYMBOL); }
"use"								{ return KeywordLexemeAction(USE); }
"palette"							{ return KeywordLexemeAction(PALETTE); }
"background"						{ return KeywordLexemeAction(BACKGROUND); }

"rectangle"							{ return KeywordLexemeAction(RECTANGLE); }
"circle"							{ return KeywordLexemeAction(CIRCLE); }
"line"								{ return KeywordLexemeAction(LINE); }
"ellipse"							{ return KeywordLexemeAction(ELLIPSE); }
"polyline"							{ return KeywordLexemeAction(POLYLINE); }
"polygon"							{ return KeywordLexemeAction(POLYGON); }

"at"								{ return KeywordLexemeAction(AT); }
"from"								{ return KeywordLexemeAction(FROM); }
"to"								{ return KeywordLexemeAction(TO); }
"size"								{ return KeywordLexemeAction(SIZE); }
"radius"							{ return KeywordLexemeAction(RADIUS); }
"width"								{ return KeywordLexemeAction(WIDTH); }
"height"							{ return KeywordLexemeAction(HEIGHT); }
"fill"								{ return KeywordLexemeAction(FILL); }
"stroke"							{ return KeywordLexemeAction(STROKE); }
"strokeWidth"						{ return KeywordLexemeAction(STROKE_WIDTH); }
"stroke_width"						{ return KeywordLexemeAction(STROKE_WIDTH); }
"opacity"							{ return KeywordLexemeAction(OPACITY); }
"z"									{ return KeywordLexemeAction(Z); }
"id"								{ return KeywordLexemeAction(ID); }

"translate"							{ return KeywordLexemeAction(TRANSLATE); }
"rotate"							{ return KeywordLexemeAction(ROTATE); }
"scale"								{ return KeywordLexemeAction(SCALE); }

"color"								{ return KeywordLexemeAction(COLOR_FUNC); }
"rgb"								{ return KeywordLexemeAction(RGB); }
"rgba"								{ return KeywordLexemeAction(RGBA); }

"px"                                { return KeywordLexemeAction(PX); }
"rem"                               { return KeywordLexemeAction(REM); }
"em"                                { return KeywordLexemeAction(EM); }
"vw"                                { return KeywordLexemeAction(VW); }
"vh"                                { return KeywordLexemeAction(VH); }
"%"                                 { return KeywordLexemeAction(PERCENT); }

{hex_color}							{ return ColorLexemeAction(HEX_COLOR); }

[0-9]+[xX][0-9]+                    { return DimensionsLexemeAction(); }



{string}							{ return StringLexemeAction(); }
{decimal}							{ return DecimalLexemeAction(); }
{integer}							{ return IntegerLexemeAction(); }

"}"									{ return BraceLexemeAction(CLOSE_BRACE); }
"{"									{ return BraceLexemeAction(OPEN_BRACE); }
"="									{ return KeywordLexemeAction(ASSIGN); }
";"									{ return SemicolonLexemeAction(); }
","									{ return CommaLexemeAction(); }
":"									{ return EnterStringContextLexemeAction(STRING); }
"."									{ return DotLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }

{identifier}						{ return IdentifierLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

.									{ return UnknownLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"