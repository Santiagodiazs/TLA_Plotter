%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

// Declarar logger externo para usar en la gramática
extern Logger * _logger;

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

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program
%type <scene> scene_declaration

// Figure-related non-terminals
%type <scene> scene_content
%type <scene> scene_item
%type <figure> draw_statement
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

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV
%left SEMICOLON

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: scene_declaration									{ 
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
		printf("[DEBUG] scene_content: empty\n");
		$$ = NULL; 
	}
	| scene_content scene_item		{ 
		printf("[DEBUG] scene_content: merging scene_item\n");
		$$ = MergeSceneContent($1, $2); 
	}
	;

scene_item: draw_statement		{ 
		printf("[DEBUG] scene_item: draw_statement\n");
		$$ = SceneFromFigure($1); 
	}
	| BACKGROUND parsed_color_value SEMICOLON {
		printf("[DEBUG] scene_item: background\n");
		$$ = SceneWithBackground($2);
	}
	| LAYER layer_name Z INTEGER SEMICOLON {
		printf("[DEBUG] scene_item: layer declaration\n");
		$$ = SceneWithLayerDecl($2, $4);
		free($2); // Liberar el string del layer_name
	}
	| LAYER layer_name OPEN_BRACE scene_content CLOSE_BRACE {
		printf("[DEBUG] scene_item: layer block\n");
		$$ = SceneWithLayerBlock($2, $4);
		free($2); // Liberar el nombre
	}
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

external_items: external_item external_items_more	{
		printf("[DEBUG] external_items: head + more\n");
		printf("[DBG] link head %p -> %p\n", (void*)$1, (void*)$2);
		$1->next = $2;
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
		$$ = SetPropertyDimensionsSemanticAction(prop, $2.width, $2.height);
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
	;

size_property: SIZE dimensions_value SEMICOLON {
		printf("DEBUG: size_property with dimensions parsed\n");
		$$ = CreatePropertySemanticAction(SIZE_PROPERTY);
		$$ = SetPropertyDimensionsSemanticAction($$, $2.width, $2.height);
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
		printf("DEBUG: from_property parsed\n");
		$$ = CreatePropertySemanticAction(FROM_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y);
	}
	;

to_property: TO coordinates_value SEMICOLON	{
		printf("DEBUG: to_property parsed\n");
		$$ = CreatePropertySemanticAction(TO_PROPERTY);
		$$ = SetPropertyCoordinatesSemanticAction($$, $2.x, $2.y);
	}
	;

stroke_width_property: STROKE_WIDTH INTEGER SEMICOLON	{
		printf("DEBUG: stroke_width_property parsed: stroke-width(%d)\n", $2);
		$$ = CreatePropertySemanticAction(STROKE_WIDTH_PROPERTY);
		$$ = SetPropertyIntValueSemanticAction($$, $2);
	}
	;

opacity_property: OPACITY DECIMAL SEMICOLON	{
		printf("DEBUG: opacity_property parsed: opacity(%f)\n", $2);
		$$ = CreatePropertySemanticAction(OPACITY_PROPERTY);
		$$ = SetPropertyFloatValueSemanticAction($$, $2);
	}
	;

scale_property: SCALE OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS SEMICOLON {
		printf("DEBUG: scale_property parsed: scale(%d, %d)\n", $3, $5);
		$$ = CreatePropertySemanticAction(SCALE_PROPERTY);
		$$ = SetPropertyScaleSemanticAction($$, (float)$3, (float)$5);
	}
	;

rotate_property: ROTATE OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON {
		printf("DEBUG: rotate_property parsed: rotate(%d)\n", $3);
		$$ = CreatePropertySemanticAction(ROTATE_PROPERTY);
		$$ = SetPropertyFloatValueSemanticAction($$, (float)$3);
	}
	;

dimensions_value: DIMENSIONS {
		printf("DEBUG: dimensions_value parsed successfully\n");
		$$.width = $1.width;
		$$.height = $1.height;
	}
	;

coordinates_value: COORDINATES {
		printf("DEBUG: coordinates_value parsed successfully: (%d,%d)\n", $1.x, $1.y);
		$$.x = $1.x;
		$$.y = $1.y;
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
