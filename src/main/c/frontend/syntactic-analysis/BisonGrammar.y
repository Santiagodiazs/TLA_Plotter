%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"
#include <stdio.h>
#include <string.h>

extern Logger * _logger;

static void disable_buffering() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

static UnitType parseUnit(const char* s) {
    if (!s) return UNIT_PX; /* default razonable */
    if (strcmp(s,"px")==0)  return UNIT_PX;
    if (strcmp(s,"rem")==0) return UNIT_REM;
    if (strcmp(s,"em")==0)  return UNIT_EM;
    if (strcmp(s,"vw")==0)  return UNIT_VW;
    if (strcmp(s,"vh")==0)  return UNIT_VH;
    /* fallback: px */
    return UNIT_PX;
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
    fprintf(stderr, "SYNTAX ERROR: %s\n", message);
    exit(1);
}

%}

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
	struct {
		int width;
		int height;
		UnitType widthUnit;
        UnitType heightUnit;
	} dimensions;

	struct {
		int x;
		int y;
	} coordinates;

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
%destructor { destroyColor($$); } <color>

/** Terminals. */
%token <integer> INTEGER
%token <token> ADD
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
%token <token> COLOR
%token <string> HEX_COLOR
%token <string> RGB_COLOR
%token <string> RGBA_COLOR

// DSL Tokens - Data Types
%token <string> IDENTIFIER
%token <string> STRING
%token <decimal> DECIMAL
%token <dimensions> DIMENSIONS
%token <coordinates> COORDINATES

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
	| error	{ yyerrok; $$ = NULL; }
	;


scene_declaration: SCENE IDENTIFIER	{ $$ = BasicSceneSemanticAction($2); }
	| SCENE IDENTIFIER OPEN_BRACE scene_content CLOSE_BRACE	{ $$ = BasicSceneSemanticAction($2); if ($4 != NULL) { $$ = $4; if ($$ != NULL && $$->name == NULL) { $$->name = malloc(strlen($2) + 1); if ($$->name != NULL) { strcpy($$->name, $2); } } } }
	| SCENE IDENTIFIER SIZE DIMENSIONS OPEN_BRACE scene_content CLOSE_BRACE	{ $$ = BasicSceneSemanticAction($2); if ($6 != NULL) { $$ = $6; if ($$ != NULL && $$->name == NULL) { $$->name = malloc(strlen($2) + 1); if ($$->name != NULL) { strcpy($$->name, $2); } } } }
	;

scene_content: /* empty */	{ $$ = NULL; }
	| scene_content scene_item	{ $$ = MergeSceneContent($1, $2); }
	;

scene_item: draw_statement	{ $$ = SceneFromFigure($1); }
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

use_property: AT coordinates_value SEMICOLON	{ $$ = CreatePropertySemanticAction(POSITION_PROPERTY); $$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y); }
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

layer_name: IDENTIFIER	{ $$ = $1; }
	| BACKGROUND	{ $$ = strdup("background"); }
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

position_item: AT coordinates_value	{ Property* prop = CreatePropertySemanticAction(POSITION_PROPERTY); $$ = SetPropertyCoordinatesSemanticAction(prop, $2.x, $2.y); }
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

width_property: WIDTH DIMENSIONS SEMICOLON	{ $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, $2.width, $2.widthUnit, 0, $2.widthUnit); }
	| WIDTH INTEGER IDENTIFIER SEMICOLON	{ UnitType u = parseUnit($3); $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, $2, u, 0, u); }
	| WIDTH DECIMAL IDENTIFIER SEMICOLON	{ UnitType u = parseUnit($3); $$ = CreatePropertySemanticAction(WIDTH_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, (int)$2, u, 0, u); }
    ;

height_property: HEIGHT DIMENSIONS SEMICOLON	{ $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, $2.heightUnit, $2.height, $2.heightUnit); }
	| HEIGHT INTEGER IDENTIFIER SEMICOLON	{ UnitType u = parseUnit($3); $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, u, $2, u); }
	| HEIGHT DECIMAL IDENTIFIER SEMICOLON	{ UnitType u = parseUnit($3); $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY); $$ = SetPropertyDimensionsWithUnitSemanticAction($$, 0, u, (int)$2, u); }
    ;

position_property: AT coordinates_value SEMICOLON	{ $$ = CreatePropertySemanticAction(POSITION_PROPERTY); $$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y); }
	;

fill_property: FILL parsed_color_value SEMICOLON	{ $$ = CreatePropertySemanticAction(FILL_PROPERTY); $$ = SetPropertyColorSemanticAction($$, $2); }
	;

stroke_property: STROKE parsed_color_value SEMICOLON	{ $$ = CreatePropertySemanticAction(STROKE_PROPERTY); $$ = SetPropertyColorSemanticAction($$, $2); }
	| STROKE SEMICOLON	{ $$ = CreatePropertySemanticAction(STROKE_PROPERTY); Color * defaultColor = ParseNamedColor("black"); $$ = SetPropertyColorSemanticAction($$, defaultColor); }
	;

	radius_property: RADIUS INTEGER SEMICOLON	{ $$ = CreatePropertySemanticAction(RADIUS_PROPERTY); $$ = SetPropertyIntValueSemanticAction($$, $2); }
	;

	from_property: FROM coordinates_value SEMICOLON	{ $$ = CreatePropertySemanticAction(FROM_PROPERTY); $$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y); }
	;

	to_property: TO coordinates_value SEMICOLON	{ $$ = CreatePropertySemanticAction(TO_PROPERTY); $$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y); }
	;

stroke_width_property
    : STROKE_WIDTH INTEGER IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        $$ = CreatePropertySemanticAction(STROKE_WIDTH_PROPERTY);
        $$ = SetPropertyIntValueSemanticAction($$, $2);
    }
    | STROKE_WIDTH DECIMAL IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        $$ = CreatePropertySemanticAction(STROKE_WIDTH_PROPERTY);
        $$ = SetPropertyIntValueSemanticAction($$, (int)$2);
    }
    ;

	opacity_property: OPACITY DECIMAL SEMICOLON	{ $$ = CreatePropertySemanticAction(OPACITY_PROPERTY); $$ = SetPropertyFloatValueSemanticAction($$, $2); }
	;

	scale_property: SCALE OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON	{ $$ = CreatePropertySemanticAction(SCALE_PROPERTY); $$ = SetPropertyFloatValueSemanticAction($$, (float)$3); }
	;


	rotate_property: ROTATE OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON	{ $$ = CreatePropertySemanticAction(ROTATE_PROPERTY); $$ = SetPropertyFloatValueSemanticAction($$, (float)$3); }
	;

	dimensions_value: DIMENSIONS	{ $$.width = $1.width; $$.height = $1.height; $$.widthUnit = $1.widthUnit; $$.heightUnit = $1.heightUnit; }
	;

	coordinates_value: COORDINATES	{ $$.x = $1.x; $$.y = $1.y; }
	;

	translate_property: TRANSLATE coordinates_value SEMICOLON	{ $$ = CreatePropertySemanticAction(TRANSLATE_PROPERTY); $$ = SetPropertyTranslateSemanticAction($$, $2.x, $2.y); }
	;

	parsed_color_value: IDENTIFIER	{ $$ = ParseNamedColor($1); }
	| IDENTIFIER DOT IDENTIFIER	{ $$ = ParsePaletteColor($1, $3); }
	| RGB_COLOR	{ $$ = ParseRgbColor($1); }
	| HEX_COLOR	{ $$ = ParseHexColor($1); }
	| RGBA_COLOR	{ $$ = ParseRgbaColor($1); }
	;

%%
