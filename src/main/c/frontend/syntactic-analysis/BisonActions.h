#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilationStatus.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonParser.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeBisonActionsModule();

/**
 * Bison semantic actions.
 */

Scene *BasicSceneSemanticAction(char *sceneName);

// ============= COLOR PARSING FUNCTIONS =============
Color *ParseNamedColor(char *name);
Color *ParsePaletteColor(char *palette_name, char *color_name);
Color *ParseHexColor(char *hex);
Color *CreateRGBColor(int r, int g, int b);
Color *CreateRGBAColor(int r, int g, int b, float a);
Color *LookupPaletteColor(const char *name);

// ============= DSL SEMANTIC ACTIONS =============
Figure *CreateFigureSemanticAction(FigureType type, char *id,
                                   Property *properties);
Property *CreatePropertySemanticAction(PropertyType type);
Property *SetPropertyCoordinatesSemanticAction(Property *property,
                                               Expression *x, Expression *y);
Property *SetPropertyIntValueSemanticAction(Property *property, int value);
Property *SetPropertyFloatValueSemanticAction(Property *property, float value);
Property *SetPropertyColorSemanticAction(Property *property, Color *color);
Symbol *CreateSymbolMove(char *name, Figure *figure);
UseInstance *CreateUseInstance(char *symbolName);
Property *SetPropertyTranslateSemanticAction(Property *property, Expression *x,
                                             Expression *y);
void ApplyTransformPropertiesToFigure(Figure *figure,
                                      Property **propertiesHead);
void AppendTransform(Figure *figure, Transform *t);
PaletteEntry *CreatePaletteEntrySemanticAction(char *name, Color *color);

// ============= SCENE SEMANTIC ACTIONS =============
Scene *BasicSceneSemanticAction(char *name);
Scene *AddFigureToSceneSemanticAction(Scene *scene, Figure *figure);
Scene *AddSymbolToSceneSemanticAction(Scene *scene, Symbol *sym);
Scene *AddUseToSceneSemanticAction(Scene *scene, UseInstance *use);
Scene *AddVariableToSceneSemanticAction(Scene *scene, char *name,
                                        Expression *value);
Scene *SceneWithVariable(char *name, Expression *value);
Scene *SceneWithPaletteBlock(PaletteEntry *entries);
Scene *SceneWithNamedPaletteBlock(char *name, PaletteEntry *entries);

// ============= NEW SCENE HELPER FUNCTIONS =============
Scene *SceneFromFigure(Figure *figure);
Scene *SceneWithBackground(Color *color);
Scene *SceneWithLayerBlock(char *name, int zLevel, Scene *blockContent);
Scene *MergeSceneContent(Scene *acc, Scene *item);

// ============= PROGRAM SEMANTIC ACTIONS =============
Program *SceneProgramSemanticAction(Scene *scene);

Property *SetPropertyDimensionsWithUnitSemanticAction(Property *property,
                                                      float width,
                                                      UnitType wUnit,
                                                      float height,
                                                      UnitType hUnit);

Scene *CreateSceneWithContent(char *name, Scene *content);
Scene *CreateSceneWithContentAndSize(char *name, int width, int height,
                                     UnitType wUnit, UnitType hUnit,
                                     Scene *content);
char *CreateLayerName(char *name);
char *CreateBackgroundLayerName();
Property *CreateStrokeWidthProperty(Expression *expr, UnitType unit);
Property *CreateStrokeWidthPropertyInt(int value, UnitType unit);

// ============= GROUP FUNCTIONS =============
Group *CreateGroup(char *name, GroupContent *content);
GroupContent *GroupFromFigure(Figure *figure);
GroupContent *GroupFromProperty(Property *property);
GroupContent *MergeGroupContent(GroupContent *acc, GroupContent *item);
Scene *AddGroupToSceneSemanticAction(Scene *scene, Group *group);

// ============= USE PROPERTIES FUNCTIONS =============
Property *MergeProperties(Property *acc, Property *item);

// ============= EXPRESSION SEMANTIC ACTIONS =============
Constant *CreateConstantSemanticAction(int value);
Factor *CreateConstantFactorSemanticAction(Constant *constant);
Factor *CreateExpressionFactorSemanticAction(Expression *expression);
Factor *CreateVariableFactorSemanticAction(char *name);
Expression *CreateArithmeticExpressionSemanticAction(ExpressionType type,
                                                     Expression *left,
                                                     Expression *right);
Expression *CreateFactorExpressionSemanticAction(Factor *factor);
Property *SetPropertyExpressionSemanticAction(Property *property,
                                              Expression *expression);

#endif
