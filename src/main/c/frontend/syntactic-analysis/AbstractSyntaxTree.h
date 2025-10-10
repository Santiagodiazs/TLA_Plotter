#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();

/**
 * This type definitions allows self-referencing types (e.g., an expression
 * that is made of another expressions, such as talking about you in 3rd
 * person, but without the madness).
 */

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum ProgramType ProgramType;
typedef enum SceneType SceneType;
typedef enum FigureType FigureType;
typedef enum PropertyType PropertyType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Scene Scene;
typedef struct Figure Figure;
typedef struct Property Property;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
};

enum FactorType {
	CONSTANT,
	EXPRESSION_FACTOR
};

enum ProgramType {
	EXPRESSION,
	SCENE_PROGRAM
};

enum SceneType {
	BASIC_SCENE
};

enum FigureType {
	RECTANGLE_FIGURE,
	CIRCLE_FIGURE,
	LINE_FIGURE,
	ELLIPSE_FIGURE,
	POLYLINE_FIGURE,
	POLYGON_FIGURE
};

enum PropertyType {
	POSITION_PROPERTY,
	SIZE_PROPERTY,
	RADIUS_PROPERTY,
	FROM_PROPERTY,
	TO_PROPERTY,
	FILL_PROPERTY,
	STROKE_PROPERTY,
	STROKE_WIDTH_PROPERTY,
	OPACITY_PROPERTY
};

struct Constant {
	int value;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
	};
	FactorType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};
	ExpressionType type;
};

struct Program {
	union {
		Expression * expression;
		Scene * scene;
	};
	ProgramType type;
};

struct Scene {
	char * name;
	SceneType type;
	Figure * figures;  // Lista de figuras en la escena
};

struct Figure {
	char * id;                // ID opcional de la figura
	FigureType type;
	Property * properties;    // Lista de propiedades
	struct Figure * next;     // Para lista enlazada
};

struct Property {
	PropertyType type;
	union {
		struct {
			int x;
			int y;
		} coordinates;        // Para AT, FROM, TO
		struct {
			int width;
			int height;
		} dimensions;         // Para SIZE
		int intValue;         // Para RADIUS, STROKE_WIDTH
		float floatValue;     // Para OPACITY
		char * stringValue;   // Para FILL, STROKE (colores)
	} value;
	struct Property * next;   // Para lista enlazada
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyConstant(Constant * constant);
void destroyExpression(Expression * expression);
void destroyFactor(Factor * factor);
void destroyProgram(Program * program);
void destroyScene(Scene * scene);
void destroyFigure(Figure * figure);
void destroyProperty(Property * property);

#endif
