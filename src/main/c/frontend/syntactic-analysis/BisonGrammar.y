%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"
#include <stdio.h>
#include <string.h>

// Declarar logger externo para usar en la gramática
extern Logger * _logger;

// Deshabilitar buffering para ver logs inmediatamente
static void disable_buffering() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

/* ---- helper para mapear unidades de width cuando vienen como IDENTIFIER ---- */
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
%destructor { destroyConstant($$); } <constant>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyFactor($$); } <factor>
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
%token <token> SCALING  // Token temporal para testing

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
%type <constant> constant
%type <expression> expression
%type <factor> factor
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
%type <figure> figure_declaration
%type <figure> figure_with_id
%type <coordinates> coordinates_value
%type <dimensions> dimensions_value
%type <string> color_value
%type <color> parsed_color_value
%type <figure> figure_anonymous
%type <integer> figure_type
%type <property> figure_properties
%type <property> external_property_list
%type <property> external_property_item
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
%type <property> external_items_more
%type <property> external_item
%type <property> position_item
%type <property> size_item
%type <property> translate_property
%type <property> transform_item
%type <property> transform_list
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

program: scene_declaration									{ 
		disable_buffering();  // Deshabilitar buffering al inicio
		$$ = SceneProgramSemanticAction($1); 
	}
	| INTEGER													{
		$$ = NULL;
	}
	| error													{ 
		yyerrok; 
		$$ = NULL; 
	}
	;

expression: expression[left] ADD expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor												{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS		{ $$ = ExpressionFactorSemanticAction($2); }
	| constant												{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER											{ $$ = IntegerConstantSemanticAction($1); }
	;


scene_declaration: SCENE IDENTIFIER	{ 
		$$ = BasicSceneSemanticAction($2); 
		free($2); // Liberar el string del IDENTIFIER
	}
	| SCENE IDENTIFIER OPEN_BRACE scene_content CLOSE_BRACE	{ 
		$$ = BasicSceneSemanticAction($2);
		if ($4 != NULL) {
			$$ = $4;  // Use the scene with figures
			if ($$ != NULL && $$->name == NULL) {
				$$->name = malloc(strlen($2) + 1);
				if ($$->name != NULL) {
					strcpy($$->name, $2);
				}
			}
		}
		free($2); // Liberar el string del IDENTIFIER
	}
	| SCENE IDENTIFIER SIZE DIMENSIONS OPEN_BRACE scene_content CLOSE_BRACE {
		printf("DEBUG: scene with size parsed - %s size %dx%d\n", $2, $4.width, $4.height);
		$$ = BasicSceneSemanticAction($2);
		if ($6 != NULL) {
			$$ = $6;  // Use the scene with figures
			if ($$ != NULL && $$->name == NULL) {
				$$->name = malloc(strlen($2) + 1);
				if ($$->name != NULL) {
					strcpy($$->name, $2);
				}
			}
		}
		// TODO: Agregar acción semántica para tamaño de escena
	}
	;

scene_content: /* empty */				{
		$$ = NULL;
	}
	| scene_content scene_item		{
		$$ = MergeSceneContent($1, $2);
	}
	;

scene_item: draw_statement		{
		$$ = SceneFromFigure($1);
	}
	| BACKGROUND parsed_color_value SEMICOLON {
		$$ = SceneWithBackground($2);
	}
	| LAYER layer_name Z INTEGER OPEN_BRACE scene_content CLOSE_BRACE {
        $$ = SceneWithLayerBlock($2, $4, $6);
        free($2);
    }
    | PALETTE OPEN_BRACE palette_list CLOSE_BRACE {
        $$ = SceneWithPaletteBlock($3);
    }
    | PALETTE IDENTIFIER OPEN_BRACE palette_list CLOSE_BRACE {
        $$ = SceneWithNamedPaletteBlock($2, $4);
    }
	| symbol_declaration {
    $$ = $1;
		}
	| use_statement {
    $$ = $1;
		}
	| group_statement {
    $$ = $1;
		}
	;

symbol_declaration
  : SYMBOL IDENTIFIER OPEN_BRACE draw_list CLOSE_BRACE
    {
      printf("[DEBUG] symbol_declaration(list): name='%s' head=%p\n", $2, (void*)$4);
      Symbol *sym = CreateSymbolMove($2, $4);   /* $4 es la CABEZA de la lista de Figure */
      $$ = BasicSceneSemanticAction(NULL);
      $$ = AddSymbolToSceneSemanticAction($$, sym);
      printf("[DEBUG] symbol_declaration: attached (sym=%p)\n", (void*)sym);
      free($2); /* si CreateSymbolMove hace strdup del nombre */
    }
  ;

use_statement
  : USE IDENTIFIER SEMICOLON {
      printf("[DEBUG] use_statement: use '%s'\n", $2);
      UseInstance *u = CreateUseInstance($2);
      $$ = BasicSceneSemanticAction(NULL);
      $$ = AddUseToSceneSemanticAction($$, u);
      printf("[DEBUG] use_statement: attached (use=%p)\n", (void*)u);
    }
  | USE IDENTIFIER AT coordinates_value SEMICOLON {
      printf("[DEBUG] use_statement: use '%s' at (%d,%d)\n", $2, $4.x, $4.y);
      UseInstance *u = CreateUseInstance($2);
      u->hasPosition = 1; u->posX = $4.x; u->posY = $4.y;
      $$ = BasicSceneSemanticAction(NULL);
      $$ = AddUseToSceneSemanticAction($$, u);
      printf("[DEBUG] use_statement: attached with position (use=%p)\n", (void*)u);
    }
  | USE IDENTIFIER OPEN_BRACE use_properties CLOSE_BRACE {
      printf("[DEBUG] use_statement: use '%s' with properties\n", $2);
      UseInstance *u = CreateUseInstance($2);
      u->properties = $3;
      $$ = BasicSceneSemanticAction(NULL);
      $$ = AddUseToSceneSemanticAction($$, u);
      printf("[DEBUG] use_statement: attached with properties (use=%p)\n", (void*)u);
    }
  ;

use_properties: /* empty */ { $$ = NULL; }
  | use_properties use_property { $$ = MergeProperties($1, $2); }
  ;

use_property: AT coordinates_value SEMICOLON {
    $$ = CreatePropertySemanticAction(POSITION_PROPERTY);
    $$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y);
  }
  | scale_property { $$ = $1; }
  | rotate_property { $$ = $1; }
  | translate_property { $$ = $1; }
  ;

group_statement
  : GROUP IDENTIFIER OPEN_BRACE group_content CLOSE_BRACE {
      printf("[DEBUG] group_statement: group '%s'\n", $2);
      Group *g = CreateGroup($2, $4);
      $$ = BasicSceneSemanticAction(NULL);
      $$ = AddGroupToSceneSemanticAction($$, g);
      printf("[DEBUG] group_statement: attached (group=%p)\n", (void*)g);
    }
  ;

group_content: /* empty */ { $$ = NULL; }
  | group_content group_item { $$ = MergeGroupContent($1, $2); }
  ;

group_item: draw_statement { $$ = GroupFromFigure($1); }
  | scale_property { $$ = GroupFromProperty($1); }
  | rotate_property { $$ = GroupFromProperty($1); }
  | translate_property { $$ = GroupFromProperty($1); }
  ;

layer_name: IDENTIFIER { $$ = $1; }
	| BACKGROUND { $$ = strdup("background"); }
	;

figure_declaration: figure_with_id		{ $$ = $1; }
	| figure_anonymous				{ $$ = $1; }
	;

draw_statement: DRAW figure_type IDENTIFIER draw_tail	{
		printf("DEBUG: Draw statement with ID '%s'\n", $3);
		printf("[DEBUG] draw_statement: About to call CreateFigureSemanticAction\n");
		printf("[DEBUG] draw_statement: type=%d, id='%s', tail=%p\n", $2, $3, $4);
		$$ = CreateFigureSemanticAction($2, $3, $4);
		free($3); 
		printf("[DEBUG] draw_statement: CreateFigureSemanticAction completed\n");
		printf("[DEBUG] draw_statement: Figure created successfully\n");
	}
	| DRAW figure_type OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("DEBUG: Draw statement anonymous figure\n");
		$$ = CreateFigureSemanticAction($2, NULL, $4);
	}
	;

draw_tail: OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("[DEBUG] draw_tail: internal properties only\n");
		$$ = $2;
	}
	| external_items draw_tail_after_external	{
		printf("[DEBUG] draw_tail: external + tail\n");
		$$ = $2;
	}
	| WITH OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("[DEBUG] draw_tail: WITH syntax\n");
		$$ = $3;
	}
	;

draw_tail_after_external: OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("[DEBUG] draw_tail_after_external: external + internal properties\n");
		$$ = $2;
	}
	| WITH OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("[DEBUG] draw_tail_after_external: external + WITH syntax\n");
		$$ = $3;
	}
	;

draw_list
  : draw_statement                          { $$ = $1; }
  | draw_list draw_statement                {
      printf("[DEBUG] draw_list: linking %p -> %p\n", (void*)$1, (void*)$2);
      if ($1) {
        Figure *tail = $1;
        while (tail->next) tail = tail->next;
        tail->next = $2;
        $$ = $1;
      } else {
        $$ = $2;
      }
    }
  ;

external_items: external_item external_items_more	{
		printf("[DEBUG] external_items: head + more\n");
		printf("[DBG] link head %p -> %p\n", (void*)$1, (void*)$2);
		$1->next = $2;
		$$ = $1;
	}
	| external_item external_item	{
		printf("[DEBUG] external_items: two items in sequence\n");
		$1->next = $2;
		$$ = $1;
	}
	| external_item SEMICOLON external_item	{
		printf("[DEBUG] external_items: item ; item\n");
		$1->next = $3;
		$$ = $1;
	}
	| external_item SEMICOLON external_item external_item	{
		printf("[DEBUG] external_items: item ; item item\n");
		$1->next = $3;
		$3->next = $4;
		$$ = $1;
	}
	| external_item	{
		printf("[DEBUG] external_items: single item\n");
		$$ = $1;
	}
	;

external_items_more: SEMICOLON external_item external_items_more	{
		printf("[DEBUG] external_items_more: semicolon + item + more\n");
		printf("[DBG] link mid %p -> %p\n", (void*)$2, (void*)$3);
		$2->next = $3;
		$$ = $2;
	}
	| /* empty */	{
		printf("[DEBUG] external_items_more: empty\n");
		$$ = NULL;
	}
	;

external_item: position_item				{
		printf("[DEBUG] external_item: position_item\n");
		$$ = $1;
	}
	| size_item						{
		printf("[DEBUG] external_item: size_item\n");
		$$ = $1;
	}
	| scale_property					{
		printf("[DEBUG] external_item: scale_property\n");
		$$ = $1;
	}
	| rotate_property					{
		printf("[DEBUG] external_item: rotate_property\n");
		$$ = $1;
	}
	| translate_property					{
		printf("[DEBUG] external_item: translate_property\n");
		$$ = $1;
	}
	;

position_item: AT coordinates_value			{
		printf("[DEBUG] position_item: AT coordinates\n");
		Property* prop = CreatePropertySemanticAction(POSITION_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction(prop, $2.x, $2.y);
	}
	;

size_item: SIZE dimensions_value				{
		printf("[DEBUG] size_item: SIZE dimensions\n");
		Property* prop = CreatePropertySemanticAction(SIZE_PROPERTY);
		$$ = SetPropertyDimensionsWithUnitSemanticAction(prop,
                 $2.width,  $2.widthUnit,
                 $2.height, $2.heightUnit);
	}
	;

figure_with_id: figure_type IDENTIFIER OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("DEBUG: Creating figure with ID '%s' of type %d\n", $2, $1);
		$$ = CreateFigureSemanticAction($1, $2, $4);
	}
	;

figure_anonymous: figure_type OPEN_BRACE figure_properties CLOSE_BRACE	{
		printf("DEBUG: Creating anonymous figure of type %d\n", $1);
		$$ = CreateFigureSemanticAction($1, NULL, $3);
	}
	;

figure_type: RECTANGLE	{ $$ = RECTANGLE_FIGURE; }
	| CIRCLE		{ $$ = CIRCLE_FIGURE; }
	| LINE			{ $$ = LINE_FIGURE; }
	| ELLIPSE		{ $$ = ELLIPSE_FIGURE; }
	| POLYLINE		{ $$ = POLYLINE_FIGURE; }
	| POLYGON		{ $$ = POLYGON_FIGURE; }
	;

figure_properties: /* empty */				{
		printf("[DEBUG] figure_properties: empty\n");
		$$ = NULL;
	}
	| property_list						{
		printf("[DEBUG] figure_properties: property_list=%p\n", $1);
		$$ = $1;
	}
	;

	palette_item
        : IDENTIFIER parsed_color_value SEMICOLON {
            printf("DEBUG: palette_item %s\n", $1);
            $$ = CreatePaletteEntrySemanticAction($1, $2);
        }
        ;

    palette_list
        : palette_item                      { $$ = $1; }
        | palette_item palette_list         { $1->next = $2; $$ = $1; }
        ;


external_property_list: external_property_item		{
		printf("[DEBUG] external_property_list: single property\n");
		$$ = $1;
	}
	| external_property_item external_property_list	{
		printf("[DEBUG] external_property_list: linking property to list\n");
		$1->next = $2;
		$$ = $1;
	}
	;

external_property_item: position_property		{
		printf("[DEBUG] external_property_item: position_property\n");
		$$ = $1;
	}
	| size_property					{
		printf("[DEBUG] external_property_item: size_property\n");
		$$ = $1;
	}
	;

property_list: property_item				{
		printf("[DEBUG] property_list: single property\n");
		$$ = $1;
	}
	| property_item property_list			{
		printf("[DEBUG] property_list: linking property to list\n");
		$1->next = $2;
		$$ = $1;
	}
	;

transform_item
        : scale_property
        | rotate_property
        | translate_property
        ;

transform_list
        : transform_item                    { $$ = $1; }
        | transform_item transform_list     { $1->next = $2; $$ = $1; }
        ;

property_item: size_property				{
		printf("[DEBUG] property_item: size_property\n");
		$$ = $1;
	}
	| position_property					{
		printf("[DEBUG] property_item: position_property\n");
		$$ = $1;
	}
	| fill_property						{
		printf("[DEBUG] property_item: fill_property\n");
		$$ = $1;
	}
	| stroke_property					{
		printf("[DEBUG] property_item: stroke_property\n");
		$$ = $1;
	}
	| radius_property					{
		printf("[DEBUG] property_item: radius_property\n");
		$$ = $1;
	}
	| from_property					{
		printf("[DEBUG] property_item: from_property\n");
		$$ = $1;
	}
	| to_property					{
		printf("[DEBUG] property_item: to_property\n");
		$$ = $1;
	}
	| stroke_width_property				{
		printf("[DEBUG] property_item: stroke_width_property\n");
		$$ = $1;
	}
	| opacity_property					{
		printf("[DEBUG] property_item: opacity_property\n");
		$$ = $1;
	}
	| scale_property					{ 
		printf("[DEBUG] property_item: scale_property\n");
		$$ = $1; 
	}
	| rotate_property					{ 
		printf("[DEBUG] property_item: rotate_property\n");
		$$ = $1; 
	}
    | translate_property					{ 
		printf("[DEBUG] property_item: translate_property\n");
		$$ = $1; 
	}
	/* nuevo: width */
	| width_property					{ $$ = $1; }
	| height_property					{ $$ = $1; }
	;

size_property: SIZE dimensions_value SEMICOLON {
        printf("DEBUG: size_property with dimensions+units parsed\n");
        $$ = CreatePropertySemanticAction(SIZE_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 $2.width,  $2.widthUnit,
                 $2.height, $2.heightUnit);
    }
    ;

/* ---- NUEVO: width con unidades obligatorias ----
   Soporta:
   - WIDTH DIMENSIONS;             (toma width y su unit del token DIMENSIONS)
   - WIDTH INTEGER IDENTIFIER;     (p.ej. WIDTH 320 px;)
   - WIDTH DECIMAL IDENTIFIER;     (p.ej. WIDTH 24.5 rem;)
*/
width_property
    : WIDTH DIMENSIONS SEMICOLON {
        printf("DEBUG: width_property via DIMENSIONS: %d (unit=%d)\n", $2.width, (int)$2.widthUnit);
        $$ = CreatePropertySemanticAction(WIDTH_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 $2.width,  $2.widthUnit,
                 0,         $2.widthUnit /* altura ignorada */);
    }
    | WIDTH INTEGER IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        printf("DEBUG: width_property: %d %s (unit=%d)\n", $2, $3, (int)u);
        $$ = CreatePropertySemanticAction(WIDTH_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 $2, u,
                 0,  u);
        free($3);
    }
    | WIDTH DECIMAL IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        printf("DEBUG: width_property: %f %s (unit=%d)\n", $2, $3, (int)u);
        $$ = CreatePropertySemanticAction(WIDTH_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 (int)$2, u,   /* si necesitás preservar decimales, cambiá a un setter float */
                 0,      u);
        free($3);
    }
    ;

height_property
    : HEIGHT DIMENSIONS SEMICOLON {
        printf("DEBUG: height_property via DIMENSIONS: %d (unit=%d)\n", $2.height, (int)$2.heightUnit);
        $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 0,         $2.heightUnit, /* ancho ignorado */
                 $2.height, $2.heightUnit);
    }
    | HEIGHT INTEGER IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        printf("DEBUG: height_property: %d %s (unit=%d)\n", $2, $3, (int)u);
        $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 0,  u,  
                 $2, u);
        free($3);
    }
    | HEIGHT DECIMAL IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        printf("DEBUG: height_property: %f %s (unit=%d)\n", $2, $3, (int)u);
        $$ = CreatePropertySemanticAction(HEIGHT_PROPERTY);
        $$ = SetPropertyDimensionsWithUnitSemanticAction($$,
                 0,      u,   /* ancho ignorado */
                 (int)$2, u);
        free($3);
    }
    ;

position_property: AT coordinates_value SEMICOLON {
		printf("DEBUG: position_property with coordinates parsed\n");
		$$ = CreatePropertySemanticAction(POSITION_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y);
	}
	;

fill_property: FILL parsed_color_value SEMICOLON {
		printf("DEBUG: fill_property with parsed color\n");
		$$ = CreatePropertySemanticAction(FILL_PROPERTY);
		$$ = SetPropertyColorSemanticAction($$, $2);
	}
	;

stroke_property: STROKE parsed_color_value SEMICOLON	{
		printf("DEBUG: stroke_property with parsed color\n");
		$$ = CreatePropertySemanticAction(STROKE_PROPERTY);
		$$ = SetPropertyColorSemanticAction($$, $2);
	}
	| STROKE SEMICOLON	{
		printf("DEBUG: stroke_property without color parsed\n");
		$$ = CreatePropertySemanticAction(STROKE_PROPERTY);
		Color * defaultColor = ParseNamedColor("black");
		$$ = SetPropertyColorSemanticAction($$, defaultColor);
	}
	;

radius_property: RADIUS INTEGER SEMICOLON	{
		printf("DEBUG: radius_property parsed: radius(%d)\n", $2);
		$$ = CreatePropertySemanticAction(RADIUS_PROPERTY);
		$$ = SetPropertyIntValueSemanticAction($$, $2);
	}
	;

from_property: FROM coordinates_value SEMICOLON	{
		$$ = CreatePropertySemanticAction(FROM_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y);
	}
	;

to_property: TO coordinates_value SEMICOLON	{
		$$ = CreatePropertySemanticAction(TO_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y);
	}
	;

stroke_width_property
    : STROKE_WIDTH INTEGER IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        printf("DEBUG: stroke_width_property: %d %s (unit=%d)\n", $2, $3, (int)u);
        $$ = CreatePropertySemanticAction(STROKE_WIDTH_PROPERTY);
        $$ = SetPropertyIntValueSemanticAction($$, $2);
        free($3);
    }
    | STROKE_WIDTH DECIMAL IDENTIFIER SEMICOLON {
        UnitType u = parseUnit($3);
        printf("DEBUG: stroke_width_property: %f %s (unit=%d)\n", $2, $3, (int)u);
        $$ = CreatePropertySemanticAction(STROKE_WIDTH_PROPERTY);
        $$ = SetPropertyIntValueSemanticAction($$, (int)$2);
        free($3);
    }
    ;

opacity_property: OPACITY DECIMAL SEMICOLON	{
		printf("DEBUG: opacity_property parsed: opacity(%f)\n", $2);
		$$ = CreatePropertySemanticAction(OPACITY_PROPERTY);
		$$ = SetPropertyFloatValueSemanticAction($$, $2);
	}
	;

scale_property: SCALE OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON {
		$$ = CreatePropertySemanticAction(SCALE_PROPERTY);
        $$ = SetPropertyFloatValueSemanticAction($$, (float)$3);
	}
	;


rotate_property: ROTATE OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON {
		$$ = CreatePropertySemanticAction(ROTATE_PROPERTY);
		$$ = SetPropertyFloatValueSemanticAction($$, (float)$3);
	}
	;

dimensions_value: DIMENSIONS {
		printf("DEBUG: dimensions_value parsed successfully\n");
		$$.width = $1.width;
		$$.height = $1.height;
		$$.widthUnit  = $1.widthUnit;
        $$.heightUnit = $1.heightUnit;
	}
	;

coordinates_value: COORDINATES {
		printf("DEBUG: coordinates_value parsed successfully: (%d,%d)\n", $1.x, $1.y);
		$$.x = $1.x;
		$$.y = $1.y;
	}
	;

translate_property: TRANSLATE coordinates_value SEMICOLON {
		$$ = CreatePropertySemanticAction(TRANSLATE_PROPERTY);
        $$ = SetPropertyTranslateSemanticAction($$, $2.x, $2.y);
	}
	;

color_value: IDENTIFIER {
		$$ = $1;
	}
	| RGB_COLOR {
		$$ = $1;
	}
	| HEX_COLOR {
		$$ = $1;
	}
	| RGBA_COLOR {
		$$ = $1;
	}
	;

parsed_color_value: IDENTIFIER {
		printf("DEBUG: parsing named color: %s\n", $1);
		$$ = ParseNamedColor($1);
	}
	| IDENTIFIER DOT IDENTIFIER {
		printf("DEBUG: parsing palette color: %s.%s\n", $1, $3);
		$$ = ParsePaletteColor($1, $3);
	}
	| RGB_COLOR {
		printf("DEBUG: parsing RGB color: %s\n", $1);
		$$ = ParseRgbColor($1);
	}
	| HEX_COLOR {
		printf("DEBUG: parsing HEX color: %s\n", $1);
		$$ = ParseHexColor($1);
	}
	| RGBA_COLOR {
		printf("DEBUG: parsing RGBA color: %s\n", $1);
		$$ = ParseRgbaColor($1);
	}
	;

%%
