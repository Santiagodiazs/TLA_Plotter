#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();



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
typedef struct Group Group;
typedef struct GroupContent GroupContent;
typedef struct PaletteEntry PaletteEntry;
typedef struct Transform Transform;
typedef struct Variable Variable;

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
	EXPRESSION_FACTOR,
	VARIABLE_FACTOR
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
	HEIGHT_PROPERTY,
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
	NAMED_COLOR,   
	HEX_COLOR_TYPE,    
	RGB_COLOR_TYPE,     
	RGBA_COLOR_TYPE
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
    bool isExpression;
	union {
		struct { float sx, sy; Expression * sxExp; Expression * syExp; } scale;
		struct { float degrees; Expression * degreesExp; } rotate;
		struct { float tx, ty; Expression * txExp; Expression * tyExp; } translate;
	} value;
	Transform *next;
};

struct PaletteEntry {
    char *name;
    char *paletteName;
    Color *color;      
    struct PaletteEntry *next;
};

PaletteEntry * createPaletteEntry(const char *name, Color *color);
void destroyPalette(PaletteEntry *head);
Color * duplicateColor(Color *original);
PaletteEntry * duplicatePalette(PaletteEntry *original);

Transform * createTransformScale(float sx, float sy);
Transform * createTransformScaleExpression(Expression * sx, Expression * sy);
Transform * createTransformRotate(float degrees);
Transform * createTransformRotateExpression(Expression * degrees);
Transform * createTransformTranslate(float tx, float ty);
Transform * createTransformTranslateExpression(Expression * tx, Expression * ty);
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
	Figure * figures;  
	struct Layer * next; 
};

struct Variable {
    char * name;
    Expression * value;
    struct Variable * next;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
		char * variableName;
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
	char * backgroundColor; 
	Layer * layers; 
	Symbol * symbols;        
	PaletteEntry *palette;   
  UseInstance * uses; 		
  Group * groups;
  Variable * variables;
};

struct Figure {
	char * id;               
	FigureType type;
	Property * properties;    
	struct Figure * next;
	Transform *transforms;   
};

struct Property {
	PropertyType type;
    bool isExpression;
	union {
		struct {
			int x;
			int y;
			Expression * xExpression;
			Expression * yExpression;
		} coordinates;       
		struct {
			int width;
			int height;
			UnitType widthUnit;
            UnitType heightUnit;
		} dimensions;        
		struct {
			float x;
			float y;
		} scale;            
		int intValue;        
		float floatValue;    
		Color * colorValue;
        Expression * expressionValue;
	} value;
	struct Property * next;   
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
    Property * properties; 
    struct UseInstance * next;
};

struct Group {
    char * name;
    Figure * figures;
    Property * properties;  
    struct Group * next;
};

struct GroupContent {
    Figure * figures;
    Property * properties;
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
void destroyGroup(Group * group);
void destroyVariable(Variable * variable);
Color * createColor(ColorType type);
Constant * createConstant(int value);
Factor * createFactor(FactorType type);
Expression * createExpression(ExpressionType type);
Variable * createVariable(char * name, Expression * value);


Layer * createLayer(const char * name, int zLevel);
void destroyLayer(Layer * layer);

#endif
