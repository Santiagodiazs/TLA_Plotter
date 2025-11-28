%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"
#include "BisonTypeDefinitions.h"
#include <stdio.h>
#include <string.h>

extern Logger * _logger;

static void disable_buffering() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {
    
}

%}

%code requires {
#include "BisonTypeDefinitions.h"
}

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.trace
%define parse.error verbose
%start program
%locations

%union {
	/** Terminals. */

	signed int integer;
	TokenLabel token;
	char * string;
	float decimal;
	Dimensions dimensions;
	Coordinates coordinates;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
	Scene * scene;
	Figure * figure;
	Property * property;
	Color * color;
	PaletteEntry * palette_entry;
	GroupContent * group_content;
	Group * group;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { free($$); } <string>
%destructor { destroyConstant($$); } <constant>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyFactor($$); } <factor>
%destructor { destroyScene($$); } <scene>
%destructor { destroyFigure($$); } <figure>
%destructor { destroyProperty($$); } <property>
%destructor { destroyColor($$); } <color>
%destructor { destroyPalette($$); } <palette_entry>


/** Terminals. */
%token <integer> INTEGER
%token <token> ADD
%token <token> ASSIGN
%token <token> CLOSE_BRACE
%token <token> CLOSE_COMMENT
%token <token> CLOSE_PARENTHESIS
%token <token> DIV
%token <token> MUL
%token <token> OPEN_BRACE
%token <token> OPEN_COMMENT
%token <token> OPEN_PARENTHESIS
%token <token> SUB

%token <token> IGNORED
%token <token> UNKNOWN

// Units
%token <token> PX
%token <token> REM
%token <token> EM
%token <token> VW
%token <token> VH
%token <token> PERCENT

// Color Functions
%token <token> RGB
%token <token> RGBA
%token <token> COLOR_FUNC

// Dimensions
%token <dimensions> DIMENSIONS


// DSL Tokens - Scene and Structure
%token <token> SCENE
%token <token> DRAW
%token <token> WITH
%token <token> LAYER
%token <token> GROUP
%token <token> SYMBOL
%token <token> USE
%token <token> PALETTE
%token <token> BACKGROUND

// DSL Tokens - Figures
%token <token> RECTANGLE
%token <token> CIRCLE
%token <token> LINE
%token <token> ELLIPSE
%token <token> POLYLINE
%token <token> POLYGON

// DSL Tokens - Properties
%token <token> AT
%token <token> FROM
%token <token> TO
%token <token> SIZE
%token <token> RADIUS
%token <token> WIDTH
%token <token> HEIGHT
%token <token> FILL
%token <token> STROKE
%token <token> STROKE_WIDTH
%token <token> OPACITY
%token <token> Z
%token <token> ID

// DSL Tokens - Transformations
%token <token> TRANSLATE
%token <token> ROTATE
%token <token> SCALE

// DSL Tokens - Colors
%token <string> HEX_COLOR
// REMOVED: RGB_COLOR, RGBA_COLOR, COLOR (now handled by rules)

// DSL Tokens - Data Types
%token <string> IDENTIFIER
%token <string> STRING
%token <decimal> DECIMAL
// REMOVED: DIMENSIONS (now handled by rules)


// DSL Tokens - Delimiters
%token <token> SEMICOLON
%token <token> COMMA
%token <token> COLON
%token <token> DOT

/** Non-terminals. */
%type <program> program
%type <scene> scene_declaration

// Figure-related non-terminals
%type <palette_entry> palette_item
%type <palette_entry> palette_list
%type <scene> scene_content
%type <scene> scene_item
%type <scene> symbol_declaration
%type <scene> use_statement
%type <scene> group_statement
%type <property> use_properties
%type <property> use_property
%type <group_content> group_content
%type <group_content> group_item
%type <figure> draw_statement
%type <figure> draw_list
%type <string> layer_name
%type <coordinates> coordinates_value
%type <property> coordinates_property_value
%type <dimensions> dimensions_value
%type <color> parsed_color_value
%type <integer> figure_type
%type <property> figure_properties
%type <property> property_list
%type <property> property_item
%type <property> size_property
%type <property> position_property
%type <property> fill_property
%type <property> stroke_property
%type <property> scale_property
%type <property> rotate_property
%type <property> radius_property
%type <property> from_property
%type <property> to_property
%type <property> stroke_width_property
%type <property> opacity_property
%type <property> draw_tail
%type <property> draw_tail_after_external
%type <property> external_items
%type <property> external_item
%type <property> position_item
%type <property> size_item
%type <property> translate_property
%type <property> width_property
%type <property> height_property
%type <constant> constant
%type <factor> factor
%type <expression> expression
%type <integer> unit
%type <color> rgb_color
%type <color> rgba_color
%type <color> color_func


/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV
%left SEMICOLON

// Precedencia para transformaciones
%left SCALE
%left ROTATE
%left TRANSLATE

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: scene_declaration	{ disable_buffering(); $$ = SceneProgramSemanticAction($1); }
	| INTEGER	{ $$ = NULL; }
	
	;


scene_declaration: SCENE IDENTIFIER	{ $$ = BasicSceneSemanticAction($2); }
	| SCENE IDENTIFIER OPEN_BRACE scene_content CLOSE_BRACE	{ $$ = CreateSceneWithContent($2, $4); }
	| SCENE IDENTIFIER SIZE dimensions_value OPEN_BRACE scene_content CLOSE_BRACE	{ $$ = CreateSceneWithContentAndSize($2, $4.width, $4.height, $4.widthUnit, $4.heightUnit, $6); }
	;

scene_content: /* empty */	{ $$ = NULL; }
	| scene_content scene_item	{ $$ = MergeSceneContent($1, $2); }
	;

scene_item: draw_statement	{ $$ = SceneFromFigure($1); }
	| IDENTIFIER ASSIGN expression SEMICOLON	{ $$ = SceneWithVariable($1, $3); }
	| BACKGROUND parsed_color_value SEMICOLON	{ $$ = SceneWithBackground($2); }
	| LAYER layer_name Z INTEGER OPEN_BRACE scene_content CLOSE_BRACE	{ $$ = SceneWithLayerBlock($2, $4, $6); }
    | PALETTE OPEN_BRACE palette_list CLOSE_BRACE	{ $$ = SceneWithPaletteBlock($3); }
    | PALETTE IDENTIFIER OPEN_BRACE palette_list CLOSE_BRACE	{ $$ = SceneWithNamedPaletteBlock($2, $4); }
	| symbol_declaration	{ $$ = $1; }
	| use_statement	{ $$ = $1; }
	| group_statement	{ $$ = $1; }
	;

symbol_declaration: SYMBOL IDENTIFIER OPEN_BRACE draw_list CLOSE_BRACE	{ Symbol *sym = CreateSymbolMove($2, $4); $$ = BasicSceneSemanticAction(NULL); $$ = AddSymbolToSceneSemanticAction($$, sym); }
  ;

use_statement: USE IDENTIFIER SEMICOLON	{ UseInstance *u = CreateUseInstance($2); $$ = BasicSceneSemanticAction(NULL); $$ = AddUseToSceneSemanticAction($$, u); }
	| USE IDENTIFIER AT coordinates_value SEMICOLON	{ UseInstance *u = CreateUseInstance($2); u->hasPosition = 1; u->posX = $4.x; u->posY = $4.y; $$ = BasicSceneSemanticAction(NULL); $$ = AddUseToSceneSemanticAction($$, u); }
	| USE IDENTIFIER OPEN_BRACE use_properties CLOSE_BRACE	{ UseInstance *u = CreateUseInstance($2); u->properties = $4; $$ = BasicSceneSemanticAction(NULL); $$ = AddUseToSceneSemanticAction($$, u); }
  ;

use_properties: /* empty */	{ $$ = NULL; }
	| use_properties use_property	{ $$ = MergeProperties($1, $2); }
  ;

use_property: AT coordinates_property_value SEMICOLON	{ $$ = $2; }
	| scale_property	{ $$ = $1; }
	| rotate_property	{ $$ = $1; }
	| translate_property	{ $$ = $1; }
  ;

group_statement: GROUP IDENTIFIER OPEN_BRACE group_content CLOSE_BRACE	{ Group *g = CreateGroup($2, $4); $$ = BasicSceneSemanticAction(NULL); $$ = AddGroupToSceneSemanticAction($$, g); }
  ;

group_content: /* empty */	{ $$ = NULL; }
	| group_content group_item	{ $$ = MergeGroupContent($1, $2); }
  ;

group_item: draw_statement	{ $$ = GroupFromFigure($1); }
	| scale_property	{ $$ = GroupFromProperty($1); }
	| rotate_property	{ $$ = GroupFromProperty($1); }
	| translate_property	{ $$ = GroupFromProperty($1); }
  ;

layer_name: IDENTIFIER	{ $$ = CreateLayerName($1); }
	| BACKGROUND	{ $$ = CreateBackgroundLayerName(); }
	;



draw_statement: DRAW figure_type IDENTIFIER draw_tail	{ $$ = CreateFigureSemanticAction($2, $3, $4); }
	| DRAW figure_type OPEN_BRACE figure_properties CLOSE_BRACE	{ $$ = CreateFigureSemanticAction($2, NULL, $4); }
	;

draw_tail: OPEN_BRACE figure_properties CLOSE_BRACE	{ $$ = $2; }
	| external_items draw_tail_after_external	{ if ($1 && $2) { Property *last = $1; while (last && last->next) last = last->next; if (last) last->next = $2; $$ = $1; } else if ($1) { $$ = $1; } else { $$ = $2; } }
	| WITH OPEN_BRACE figure_properties CLOSE_BRACE	{ $$ = $3; }
	;

draw_tail_after_external: OPEN_BRACE figure_properties CLOSE_BRACE	{ $$ = $2; }
	| WITH OPEN_BRACE figure_properties CLOSE_BRACE	{ $$ = $3; }
	;

draw_list: draw_statement	{ $$ = $1; }
	| draw_list draw_statement	{ if ($1) { Figure *tail = $1; while (tail->next) tail = tail->next; tail->next = $2; $$ = $1; } else { $$ = $2; } }
  ;

external_items: external_item	{ $$ = $1; }
	| external_items external_item	{ Property *last = $1; while (last->next) last = last->next; last->next = $2; $$ = $1; }
	;

external_item: position_item	{ $$ = $1; }
	| size_item	{ $$ = $1; }
	| scale_property	{ $$ = $1; }
	| rotate_property	{ $$ = $1; }
	| translate_property	{ $$ = $1; }
	;

position_item: AT coordinates_property_value	{ $$ = $2; }
	;

// New rule for property coordinates that returns a Property*
coordinates_property_value: OPEN_PARENTHESIS expression COMMA expression CLOSE_PARENTHESIS {
		Property* prop = CreatePropertySemanticAction(POSITION_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction(prop, $2, $4);
	}
	;

size_item: SIZE dimensions_value	{ Property* prop = CreatePropertySemanticAction(SIZE_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction(prop, $2.width, $2.widthUnit, $2.height, $2.heightUnit); }
	;



figure_type: RECTANGLE	{ $$ = RECTANGLE_FIGURE; }
	| CIRCLE	{ $$ = CIRCLE_FIGURE; }
	| LINE	{ $$ = LINE_FIGURE; }
	| ELLIPSE	{ $$ = ELLIPSE_FIGURE; }
	| POLYLINE	{ $$ = POLYLINE_FIGURE; }
	| POLYGON	{ $$ = POLYGON_FIGURE; }
	;

figure_properties: /* empty */	{ $$ = NULL; }
	| property_list	{ $$ = $1; }
	;

palette_item: IDENTIFIER parsed_color_value SEMICOLON	{ $$ = CreatePaletteEntrySemanticAction($1, $2); }
        ;

palette_list: palette_item	{ $$ = $1; }
	| palette_item palette_list	{ $1->next = $2; $$ = $1; }
        ;




property_list: property_item	{ $$ = $1; }
	| property_item property_list	{ $1->next = $2; $$ = $1; }
	;



property_item: size_property	{ $$ = $1; }
	| position_property	{ $$ = $1; }
	| fill_property	{ $$ = $1; }
	| stroke_property	{ $$ = $1; }
	| radius_property	{ $$ = $1; }
	| from_property	{ $$ = $1; }
	| to_property	{ $$ = $1; }
	| stroke_width_property	{ $$ = $1; }
	| opacity_property	{ $$ = $1; }
	| scale_property	{ $$ = $1; }
	| rotate_property	{ $$ = $1; }
	| translate_property	{ $$ = $1; }
	/* nuevo: width */
	| width_property	{ $$ = $1; }
	| height_property	{ $$ = $1; }
	;

size_property: SIZE dimensions_value SEMICOLON	{ $$ = CreatePropertySemanticAction(SIZE_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, $2.width, $2.widthUnit, $2.height, $2.heightUnit); }
    ;

constant: INTEGER { $$ = CreateConstantSemanticAction($1); }
	;

factor: constant { $$ = CreateConstantFactorSemanticAction($1); }
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS { $$ = CreateExpressionFactorSemanticAction($2); }
	| IDENTIFIER { $$ = CreateVariableFactorSemanticAction($1); }
	;

expression: factor { $$ = CreateFactorExpressionSemanticAction($1); }
	| expression ADD expression { $$ = CreateArithmeticExpressionSemanticAction(ADDITION, $1, $3); }
	| expression SUB expression { $$ = CreateArithmeticExpressionSemanticAction(SUBTRACTION, $1, $3); }
	| expression MUL expression { $$ = CreateArithmeticExpressionSemanticAction(MULTIPLICATION, $1, $3); }
	| expression DIV expression { $$ = CreateArithmeticExpressionSemanticAction(DIVISION, $1, $3); }
	;

width_property: WIDTH dimensions_value SEMICOLON	{ $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, $2.width, $2.widthUnit, 0, $2.widthUnit); }
	| WIDTH expression unit SEMICOLON	{ $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, $3, 0, $3); $$ = SetPropertyExpressionSemanticAction($$, $2); }
	| WIDTH expression SEMICOLON	{ $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, UNIT_PX, 0, UNIT_PX); $$ = SetPropertyExpressionSemanticAction($$, $2); }
	| WIDTH DECIMAL unit SEMICOLON	{ $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, (int)$2, $3, 0, $3); }
    ;

height_property: HEIGHT dimensions_value SEMICOLON	{ $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, $2.heightUnit, $2.height, $2.heightUnit); }
	| HEIGHT expression unit SEMICOLON	{ $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, $3, 0, $3); $$ = SetPropertyExpressionSemanticAction($$, $2); }
	| HEIGHT expression SEMICOLON	{ $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, UNIT_PX, 0, UNIT_PX); $$ = SetPropertyExpressionSemanticAction($$, $2); }
	| HEIGHT DECIMAL unit SEMICOLON	{ $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, $3, (int)$2, $3); }
    ;

position_property: AT coordinates_property_value SEMICOLON	{ $$ = $2; }
	;

fill_property: FILL parsed_color_value SEMICOLON	{ $$ = CreatePropertySemanticAction(FILL_PROPERTY); $$ = SetPropertyColorSemanticAction($$, $2); }
	;

stroke_property: STROKE parsed_color_value SEMICOLON	{ $$ = CreatePropertySemanticAction(STROKE_PROPERTY); $$ = SetPropertyColorSemanticAction($$, $2); }
	| STROKE SEMICOLON	{ $$ = CreatePropertySemanticAction(STROKE_PROPERTY); Color * defaultColor = ParseNamedColor("black"); $$ = SetPropertyColorSemanticAction($$, defaultColor); }
	;

radius_property: RADIUS expression SEMICOLON	{ $$ = CreatePropertySemanticAction(RADIUS_PROPERTY); $$ = SetPropertyExpressionSemanticAction($$, $2); }
	;

from_property: FROM coordinates_property_value SEMICOLON	{ $$ = $2; $$->type = FROM_PROPERTY; }
	;

to_property: TO coordinates_property_value SEMICOLON	{ $$ = $2; $$->type = TO_PROPERTY; }
	;

stroke_width_property
    : STROKE_WIDTH expression unit SEMICOLON {
        $$ = CreateStrokeWidthProperty($2, $3);
    }
    | STROKE_WIDTH expression SEMICOLON {
        $$ = CreateStrokeWidthProperty($2, UNIT_PX);
    }
    | STROKE_WIDTH DECIMAL unit SEMICOLON {
        $$ = CreateStrokeWidthPropertyInt((int)$2, $3);
    }
    ;

opacity_property: OPACITY DECIMAL SEMICOLON	{ $$ = CreatePropertySemanticAction(OPACITY_PROPERTY); $$ = SetPropertyFloatValueSemanticAction($$, $2); }
	;

scale_property: SCALE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS SEMICOLON	{ $$ = CreatePropertySemanticAction(SCALE_PROPERTY); $$ = SetPropertyExpressionSemanticAction($$, $3); }
	;


rotate_property: ROTATE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS SEMICOLON	{ $$ = CreatePropertySemanticAction(ROTATE_PROPERTY); $$ = SetPropertyExpressionSemanticAction($$, $3); }
	;

dimensions_value: DIMENSIONS	{ $$ = $1; }
	| INTEGER IDENTIFIER INTEGER	{ 
		if (strcasecmp($2, "x") != 0) {
			yyerror(&@2, "Expected 'x' or 'X' between dimensions");
			YYERROR;
		}
		$$.width = $1; $$.height = $3; $$.widthUnit = UNIT_PX; $$.heightUnit = UNIT_PX; 
		free($2);
	}
	;

coordinates_value: OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS	{ $$.x = $2; $$.y = $4; }
	;

translate_property: TRANSLATE coordinates_property_value SEMICOLON	{ $$ = $2; $$->type = TRANSLATE_PROPERTY; }
	;

parsed_color_value: IDENTIFIER	{ $$ = ParseNamedColor($1); }
	| IDENTIFIER DOT IDENTIFIER	{ $$ = ParsePaletteColor($1, $3); }
	| rgb_color	{ $$ = $1; }
	| HEX_COLOR	{ $$ = ParseHexColor($1); }
	| rgba_color	{ $$ = $1; }
	| color_func	{ $$ = $1; }
	;

rgb_color: RGB OPEN_PARENTHESIS INTEGER COMMA INTEGER COMMA INTEGER CLOSE_PARENTHESIS { $$ = CreateRGBColor($3, $5, $7); }
    ;

rgba_color: RGBA OPEN_PARENTHESIS INTEGER COMMA INTEGER COMMA INTEGER COMMA DECIMAL CLOSE_PARENTHESIS { $$ = CreateRGBAColor($3, $5, $7, $9); }
    ;

color_func: COLOR_FUNC OPEN_PARENTHESIS INTEGER COMMA INTEGER COMMA INTEGER CLOSE_PARENTHESIS { $$ = CreateRGBColor($3, $5, $7); }
    ;

unit: PX { $$ = UNIT_PX; }
    | REM { $$ = UNIT_REM; }
    | EM { $$ = UNIT_EM; }
    | VW { $$ = UNIT_VW; }
    | VH { $$ = UNIT_VH; }
    | PERCENT { $$ = UNIT_PERCENT; }
    ;

%%
