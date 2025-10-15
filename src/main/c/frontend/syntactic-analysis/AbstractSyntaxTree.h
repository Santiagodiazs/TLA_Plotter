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
typedef enum ColorType ColorType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Scene Scene;
typedef struct Figure Figure;
typedef struct Property Property;
typedef struct Color Color;
typedef struct Layer Layer;
typedef struct Symbol Symbol;
typedef struct UseInstance UseInstance;
typedef struct PaletteEntry PaletteEntry;
typedef struct Transform Transform;

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
	WIDTH_PROPERTY,
	RADIUS_PROPERTY,
	FROM_PROPERTY,
	TO_PROPERTY,
	FILL_PROPERTY,
	STROKE_PROPERTY,
	STROKE_WIDTH_PROPERTY,
	OPACITY_PROPERTY,
	SCALE_PROPERTY,
	ROTATE_PROPERTY,
	TRANSLATE_PROPERTY
};

enum ColorType {
	NAMED_COLOR,    // "red", "blue", etc.
	HEX_COLOR_TYPE,      // "#FF0000", "#F00"
	RGB_COLOR_TYPE,      // "rgb(255,0,0)"
	RGBA_COLOR_TYPE      // "rgba(255,0,0,1.0)"
};

typedef enum {
	TRANSFORM_SCALE,
	TRANSFORM_ROTATE,
	TRANSFORM_TRANSLATE
} TransformType;

typedef enum {
    UNIT_PX,
    UNIT_REM,
    UNIT_EM,
    UNIT_VW,
    UNIT_VH,
    UNIT_PERCENT
} UnitType;

struct Transform {
	TransformType type;
	union {
		struct { float sx, sy; } scale;
		struct { float degrees; } rotate;
		struct { float tx, ty; } translate;
	} value;
	Transform *next;
};

struct PaletteEntry {
    char *name;        // nombre lógico (ej. "trunk")
    Color *color;      // color resuelto asociado
    struct PaletteEntry *next;
};

PaletteEntry * createPaletteEntry(const char *name, Color *color);
void destroyPalette(PaletteEntry *head);
Color * duplicateColor(Color *original);
PaletteEntry * duplicatePalette(PaletteEntry *original);

Transform * createTransformScale(float sx, float sy);
Transform * createTransformRotate(float degrees);
Transform * createTransformTranslate(float tx, float ty);
void destroyTransform(Transform *t);

struct Constant {
	int value;
};

struct Color {
	ColorType type;
	union {
		char * name;          
		char * hex;           
		struct {
			int r, g, b;      
		} rgb;
		struct {
			int r, g, b;      
			float a;
		} rgba;
	} value;
};

struct Layer {
	char * name;
	int zLevel;
	Figure * figures;  // Lista de figuras en este layer
	struct Layer * next; // Lista enlazada de layers
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
	Figure * figures;  
	char * backgroundColor; // opcional
	Layer * layers; // Lista de layers
	Symbol * symbols;          // lista de símbolos declarados
	PaletteEntry *palette;   // lista de entradas de paleta
  UseInstance * uses; 			// lista de uses en esta escena
};

struct Figure {
	char * id;               
	FigureType type;
	Property * properties;    
	struct Figure * next;
	Transform *transforms;   // lista de transforms (scale/rotate/translate)
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
			UnitType widthUnit;
            UnitType heightUnit;
		} dimensions;         // Para SIZE
		struct {
			float x;
			float y;
		} scale;              // Para SCALE
		int intValue;         // Para RADIUS, STROKE_WIDTH
		float floatValue;     // Para OPACITY, ROTATE (angle)
		Color * colorValue;   // Para FILL, STROKE (colores)
	} value;
	struct Property * next;   // Para lista enlazada
};

struct Symbol {
    char * name;
    Figure figure;
    struct Symbol * next;
};

struct UseInstance {
    Symbol * symbol;
    char * symbolName;
    int hasPosition;
    int posX, posY;
    struct UseInstance * next;
};

void destroyConstant(Constant * constant);
void destroyExpression(Expression * expression);
void destroyFactor(Factor * factor);
void destroyProgram(Program * program);
void destroyScene(Scene * scene);
void destroyFigure(Figure * figure);
void destroyProperty(Property * property);
void destroyColor(Color * color);
void destroySymbol(Symbol * symbol);
void destroyUseInstance(UseInstance * useInstance);
Color * createColor(ColorType type);

// Layer functions
Layer * createLayer(const char * name, int zLevel);
void destroyLayer(Layer * layer);

#endif
