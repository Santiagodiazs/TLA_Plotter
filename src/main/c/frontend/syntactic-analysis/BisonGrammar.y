%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

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
%token <token> HEX_COLOR
%token <token> RGB_COLOR
%token <token> RGBA_COLOR

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
%type <token> scene_content
%type <token> figure_declaration
%type <token> figure_with_id
%type <token> figure_anonymous
%type <token> figure_type
%type <token> figure_properties
%type <token> size_property
%type <token> position_property
%type <token> fill_property
%type <token> stroke_property

%type <token> draw_statement

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

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
	}
	;

scene_content: /* empty */
	| scene_content figure_declaration
	| scene_content draw_statement
	;

figure_declaration: figure_with_id
	| figure_anonymous
	;

draw_statement: DRAW figure_type IDENTIFIER OPEN_BRACE figure_properties CLOSE_BRACE
	| DRAW figure_type OPEN_BRACE figure_properties CLOSE_BRACE
	;

figure_with_id: figure_type IDENTIFIER OPEN_BRACE figure_properties CLOSE_BRACE	{
		$$ = NULL;
	}
	;

figure_anonymous: figure_type OPEN_BRACE figure_properties CLOSE_BRACE	{
		$$ = NULL;
	}
	;

figure_type: RECTANGLE
	| CIRCLE
	| LINE
	| ELLIPSE
	| POLYLINE
	| POLYGON
	;

figure_properties: /* empty */
	| figure_properties size_property
	| figure_properties position_property
	| figure_properties fill_property
	| figure_properties stroke_property
	| figure_properties radius_property
	| figure_properties from_to_property
	| figure_properties stroke_width_property
	| figure_properties opacity_property
	;

size_property: SIZE dimensions_value
	| SIZE
	;

position_property: AT coordinates_value {
		printf("DEBUG: position_property with coordinates parsed\n");
	}
	| AT {
		printf("DEBUG: position_property without coordinates parsed\n");
	}
	;

fill_property: FILL color_value
	| FILL
	;

stroke_property: STROKE color_value
	| STROKE
	;

radius_property: RADIUS INTEGER
	;

from_to_property: FROM coordinates_value
	| TO coordinates_value
	;

stroke_width_property: STROKE_WIDTH INTEGER
	;

opacity_property: OPACITY DECIMAL
	;

dimensions_value: DIMENSIONS {
		printf("DEBUG: dimensions_value parsed successfully\n");
	}
	;

coordinates_value: OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS {
		printf("DEBUG: coordinates_value parsed successfully\n");
	}
	;

color_value: HEX_COLOR
	| RGB_COLOR
	| RGBA_COLOR
	| IDENTIFIER
	;

%%
