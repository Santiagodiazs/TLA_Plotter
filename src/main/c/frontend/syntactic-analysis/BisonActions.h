#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/CompilationStatus.h"
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

Scene * BasicSceneSemanticAction(const char * sceneName);


// ============= COLOR PARSING FUNCTIONS =============
Color * ParseNamedColor(const char * name);
Color * ParsePaletteColor(const char * palette_name, const char * color_name);
Color * ParseHexColor(const char * hex);
Color * ParseRgbColor(const char * rgb);
Color * ParseRgbaColor(const char * rgba);
Color * ParseColorFunction(const char * colorFunc);
Color * LookupPaletteColor(const char *name);

// ============= DSL SEMANTIC ACTIONS =============
Figure * CreateFigureSemanticAction(FigureType type, const char * id, Property * properties);
Property * CreatePropertySemanticAction(PropertyType type);
Property * SetPropertyCoordinatesSemanticAction(Property * property, int x, int y);
Property * SetPropertyIntValueSemanticAction(Property * property, int value);
Property * SetPropertyFloatValueSemanticAction(Property * property, float value);
Property * SetPropertyColorSemanticAction(Property * property, Color * color);
Symbol * CreateSymbolMove(const char * name, Figure * figure);
UseInstance * CreateUseInstance(const char *symbolName);
Property * SetPropertyTranslateSemanticAction(Property * property, int x, int y);
void ApplyTransformPropertiesToFigure(Figure *figure, Property **propertiesHead);
void AppendTransform(Figure *figure, Transform *t);
PaletteEntry * CreatePaletteEntrySemanticAction(char *name, Color *color);
// REMOVED: AddPropertyToListSemanticAction - No longer needed with right recursion

// ============= SCENE SEMANTIC ACTIONS =============
Scene * BasicSceneSemanticAction(const char * name);
Scene * AddFigureToSceneSemanticAction(Scene * scene, Figure * figure);
Scene * AddSymbolToSceneSemanticAction(Scene *scene, Symbol *sym);
Scene * AddUseToSceneSemanticAction(Scene *scene, UseInstance *use);
Scene * SceneWithPaletteBlock(PaletteEntry *entries);
Scene * SceneWithNamedPaletteBlock(const char *name, PaletteEntry *entries);

// ============= NEW SCENE HELPER FUNCTIONS =============
Scene * SceneFromFigure(Figure * figure);
Scene * SceneWithBackground(Color * color);
Scene * SceneWithLayerBlock(const char * name, int zLevel, Scene * blockContent);
Scene * MergeSceneContent(Scene * acc, Scene * item);


// ============= PROGRAM SEMANTIC ACTIONS =============
Program * SceneProgramSemanticAction(Scene * scene);

Property * SetPropertyDimensionsWithUnitSemanticAction(Property * property,float width,  UnitType wUnit,float height, UnitType hUnit);

// ============= GROUP FUNCTIONS =============
Group * CreateGroup(const char *name, GroupContent *content);
GroupContent * GroupFromFigure(Figure *figure);
GroupContent * GroupFromProperty(Property *property);
GroupContent * MergeGroupContent(GroupContent *acc, GroupContent *item);
Scene * AddGroupToSceneSemanticAction(Scene *scene, Group *group);

// ============= USE PROPERTIES FUNCTIONS =============
Property * MergeProperties(Property *acc, Property *item);

#endif
