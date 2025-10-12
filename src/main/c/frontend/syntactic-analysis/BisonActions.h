#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
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

Constant * IntegerConstantSemanticAction(const int value);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Program * ExpressionProgramSemanticAction(Expression * expression);

Scene * BasicSceneSemanticAction(const char * sceneName);
Program * SceneProgramSemanticAction(Scene * scene);

// ============= DSL SEMANTIC ACTIONS =============
Figure * CreateFigureSemanticAction(FigureType type, const char * id, Property * properties);
Property * CreatePropertySemanticAction(PropertyType type);
Property * SetPropertyCoordinatesSemanticAction(Property * property, int x, int y);
Property * SetPropertyDimensionsSemanticAction(Property * property, int width, int height);
Property * SetPropertyScaleSemanticAction(Property * property, float x, float y);
Property * SetPropertyIntValueSemanticAction(Property * property, int value);
Property * SetPropertyFloatValueSemanticAction(Property * property, float value);
Property * SetPropertyStringValueSemanticAction(Property * property, const char * value);
// REMOVED: AddPropertyToListSemanticAction - No longer needed with right recursion
Scene * AddFigureToSceneSemanticAction(Scene * scene, Figure * figure);

#endif
