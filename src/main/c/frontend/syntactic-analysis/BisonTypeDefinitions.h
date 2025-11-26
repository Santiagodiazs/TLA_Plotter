#ifndef BISON_TYPE_DEFINITIONS_HEADER
#define BISON_TYPE_DEFINITIONS_HEADER

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"

typedef struct {
  int width;
  int height;
  UnitType widthUnit;
  UnitType heightUnit;
} Dimensions;

typedef struct {
  int x;
  int y;
} Coordinates;

#endif
