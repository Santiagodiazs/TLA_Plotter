%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

extern CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
extern CompilationStatus EnterImportExpressionLexemeAction(FlexContext context);
extern CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context);
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus LeaveImportExpressionLexemeAction();
extern CompilationStatus LeaveMultilineCommentLexemeAction();
extern CompilationStatus ParenthesisLexemeAction(TokenLabel label);
extern CompilationStatus SubexpressionLexemeAction();
extern CompilationStatus UnknownLexemeAction();

// Acciones específicas del DSL 
extern CompilationStatus KeywordLexemeAction(TokenLabel label);
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus StringLexemeAction();
extern CompilationStatus DecimalLexemeAction();
extern CompilationStatus DimensionsLexemeAction();
extern CompilationStatus CoordinatesLexemeAction();
extern CompilationStatus ColorLexemeAction(TokenLabel label);
extern CompilationStatus BraceLexemeAction(TokenLabel label);
extern CompilationStatus SemicolonLexemeAction();
extern CompilationStatus CommaLexemeAction();
extern CompilationStatus ColonLexemeAction();

%}

/* Habilitar funciones de manipulación de condiciones de inicio */
%option stack

%x IMPORT_EXPRESSION
%x MULTILINE_COMMENT

/* Patrones reutilizables para el DSL */ 
whitespace			[[:space:]]+
identifier			[a-zA-Z_][a-zA-Z0-9_]*
integer				[0-9]+
decimal				[0-9]+(\.[0-9]+)?
dimensions			[0-9]+[[:space:]]*[xX][[:space:]]*[0-9]+
coordinates			\([0-9]+,[0-9]+\)


WS					[ \t\r\n\f\v]*
INT					[0-9]+
FLOAT				([0-9]+(\.[0-9]+)?|\.[0-9]+)
hex_color			#([0-9A-Fa-f]{6}|[0-9A-Fa-f]{3})
rgb_color			rgb{WS}\({WS}{INT}{WS},{WS}{INT}{WS},{WS}{INT}{WS}\)
rgba_color			rgba{WS}\({WS}{INT}{WS},{WS}{INT}{WS},{WS}{INT}{WS},{WS}{FLOAT}{WS}\)
color_func			color{WS}\({WS}{INT}{WS},{WS}{INT}{WS},{WS}{INT}{WS}\)
string				\"[^\"]*\"

%%

%{

%}

"import"							{ return EnterImportExpressionLexemeAction(IMPORT_EXPRESSION); }
<IMPORT_EXPRESSION>"from"			{ return SubexpressionLexemeAction(); }
<IMPORT_EXPRESSION>";"				{ return LeaveImportExpressionLexemeAction(); }
<IMPORT_EXPRESSION>[^;]+			{ return SubexpressionLexemeAction(); }

"/*"								{ return EnterMultilineCommentLexemeAction(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"				{ return LeaveMultilineCommentLexemeAction(); }
<MULTILINE_COMMENT>[^*]+			{ return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>.				{ return IgnoredLexemeAction(); }

"-"									{ return ArithmeticOperatorLexemeAction(SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(ADD); }

"("									{ return ParenthesisLexemeAction(OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(CLOSE_PARENTHESIS); }

[[:digit:]]+						{ return IntegerLexemeAction(); }

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
"opacity"							{ return KeywordLexemeAction(OPACITY); }
"z"									{ return KeywordLexemeAction(Z); }
"id"								{ return KeywordLexemeAction(ID); }

"translate"							{ return KeywordLexemeAction(TRANSLATE); }
"rotate"							{ return KeywordLexemeAction(ROTATE); }
"scale"								{ return KeywordLexemeAction(SCALE); }

"color"								{ return KeywordLexemeAction(COLOR); }

{hex_color}							{ return ColorLexemeAction(HEX_COLOR); }
{rgba_color}						{ return ColorLexemeAction(RGBA_COLOR); }
{rgb_color}							{ return ColorLexemeAction(RGB_COLOR); }
{color_func}						{ return ColorLexemeAction(COLOR); }

{identifier}						{ return IdentifierLexemeAction(); }
{string}							{ return StringLexemeAction(); }
{decimal}							{ return DecimalLexemeAction(); }
{dimensions}						{ return DimensionsLexemeAction(); }
{integer}							{ return IntegerLexemeAction(); }
{coordinates}						{ return CoordinatesLexemeAction(); }

"}"									{ return BraceLexemeAction(CLOSE_BRACE); }
"{"									{ return BraceLexemeAction(OPEN_BRACE); }
";"									{ return SemicolonLexemeAction(); }
","									{ return CommaLexemeAction(); }
":"									{ return ColonLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

.									{ return UnknownLexemeAction(); }

%%