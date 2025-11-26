#include "AbstractSyntaxTree.h"
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule() {
  if (_logger != NULL) {
    logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
    destroyLogger(_logger);
    _logger = NULL;
  }
}

ModuleDestructor initializeAbstractSyntaxTreeModule() {
  _logger = createLogger("AbstractSyntaxTree");
  return _shutdownAbstractSyntaxTreeModule;
}

void destroyConstant(Constant *constant) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (constant != NULL) {
    free(constant);
  }
}

void destroyExpression(Expression *expression) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (expression != NULL) {
    switch (expression->type) {
    case ADDITION:
    case DIVISION:
    case MULTIPLICATION:
    case SUBTRACTION:
      destroyExpression(expression->leftExpression);
      destroyExpression(expression->rightExpression);
      break;
    case FACTOR:
      destroyFactor(expression->factor);
      break;
    }
    free(expression);
  }
}

void destroyFactor(Factor *factor) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (factor != NULL) {
    switch (factor->type) {
    case CONSTANT:
      destroyConstant(factor->constant);
      break;
    case EXPRESSION_FACTOR:
      destroyExpression(factor->expression);
      break;
    case VARIABLE_FACTOR:
      if (factor->variableName != NULL) {
        free(factor->variableName);
      }
      break;
    }
    free(factor);
  }
}

void destroyScene(Scene *scene) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (scene != NULL) {
    if (scene->name != NULL) {
      free(scene->name);
    }
    if (scene->backgroundColor != NULL) {
      free(scene->backgroundColor);
    }
    destroyFigure(scene->figures);
    destroyLayer(scene->layers);
    if (scene->symbols)
      destroySymbol(scene->symbols);
    if (scene->uses)
      destroyUseInstance(scene->uses);
    if (scene->groups)
      destroyGroup(scene->groups);
    if (scene->palette)
      destroyPalette(scene->palette);
    if (scene->variables)
      destroyVariable(scene->variables);
    free(scene);
  }
}

void destroyVariable(Variable *variable) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  while (variable) {
    Variable *next = variable->next;
    if (variable->name)
      free(variable->name);
    if (variable->value)
      destroyExpression(variable->value);
    free(variable);
    variable = next;
  }
}

void destroySymbol(Symbol *symbol) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  while (symbol) {
    Symbol *next = symbol->next;

    if (symbol->name)
      free(symbol->name);

    if (symbol->figure.id) {
      free(symbol->figure.id);
      symbol->figure.id = NULL;
    }
    if (symbol->figure.properties) {
      destroyProperty(symbol->figure.properties);
      symbol->figure.properties = NULL;
    }
    if (symbol->figure.transforms) {
      destroyTransform(symbol->figure.transforms);
      symbol->figure.transforms = NULL;
    }
    symbol->figure.next = NULL;

    free(symbol);
    symbol = next;
  }
}

void destroyUseInstance(UseInstance *useInstance) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  while (useInstance) {
    UseInstance *nxt = useInstance->next;
    if (useInstance->symbolName)
      free(useInstance->symbolName);
    if (useInstance->properties)
      destroyProperty(useInstance->properties);
    free(useInstance);
    useInstance = nxt;
  }
}

void destroyGroup(Group *group) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  while (group) {
    Group *nxt = group->next;
    if (group->name)
      free(group->name);
    if (group->figures)
      destroyFigure(group->figures);
    if (group->properties)
      destroyProperty(group->properties);
    free(group);
    group = nxt;
  }
}

void destroyProgram(Program *program) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (program != NULL) {
    switch (program->type) {
    case EXPRESSION:
      destroyExpression(program->expression);
      break;
    case SCENE_PROGRAM:
      destroyScene(program->scene);
      break;
    }
    free(program);
  }
}

void destroyFigure(Figure *figure) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (!figure)
    return;

  Figure *current = figure;
  while (current) {
    Figure *next = current->next;

    if (current->id)
      free(current->id);

    if (current->properties) {
      destroyProperty(current->properties);
      current->properties = NULL;
    }

    if (current->transforms) {
      destroyTransform(current->transforms);
      current->transforms = NULL;
    }

    free(current);
    current = next;
  }
}

void destroyProperty(Property *property) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (property != NULL) {
    Property *current = property;
    while (current != NULL) {
      Property *next = current->next;

      if (current->isExpression && current->value.expressionValue) {
        destroyExpression(current->value.expressionValue);
      } else if (current->type == POSITION_PROPERTY ||
                 current->type == FROM_PROPERTY ||
                 current->type == TO_PROPERTY) {
        if (current->value.coordinates.xExpression)
          destroyExpression(current->value.coordinates.xExpression);
        if (current->value.coordinates.yExpression)
          destroyExpression(current->value.coordinates.yExpression);
      } else if (current->type == FILL_PROPERTY ||
                 current->type == STROKE_PROPERTY) {
        if (current->value.colorValue != NULL) {
          destroyColor(current->value.colorValue);
        }
      }

      free(current);
      current = next;
    }
  }
}

Color *createColor(ColorType type) {
  logDebugging(_logger, "Creating color of type %d", type);
  Color *color = calloc(1, sizeof(Color));
  if (color != NULL) {
    color->type = type;
  }
  return color;
}

Constant *createConstant(int value) {
  logDebugging(_logger, "Creating constant with value %d", value);
  Constant *constant = calloc(1, sizeof(Constant));
  if (constant != NULL) {
    constant->value = value;
  }
  return constant;
}

Factor *createFactor(FactorType type) {
  logDebugging(_logger, "Creating factor of type %d", type);
  Factor *factor = calloc(1, sizeof(Factor));
  if (factor != NULL) {
    factor->type = type;
  }
  return factor;
}

Expression *createExpression(ExpressionType type) {
  logDebugging(_logger, "Creating expression of type %d", type);
  Expression *expression = calloc(1, sizeof(Expression));
  if (expression != NULL) {
    expression->type = type;
  }
  return expression;
}

Variable *createVariable(char *name, Expression *value) {
  logDebugging(_logger, "Creating variable %s", name ? name : "NULL");
  Variable *variable = calloc(1, sizeof(Variable));
  if (variable != NULL) {
    variable->name = name ? strdup(name) : NULL;
    variable->value = value;
    variable->next = NULL;
  }
  return variable;
}

void destroyColor(Color *color) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (color != NULL) {
    switch (color->type) {
    case NAMED_COLOR:
      if (color->value.name != NULL) {
        free(color->value.name);
      }
      break;
    case HEX_COLOR_TYPE:
      if (color->value.hex != NULL) {
        free(color->value.hex);
      }
      break;
    case RGB_COLOR_TYPE:
    case RGBA_COLOR_TYPE:

      break;
    }
    free(color);
  }
}

Layer *createLayer(const char *name, int zLevel) {
  logDebugging(_logger, "Creating layer with name '%s' and z-level %d",
               name ? name : "NULL", zLevel);
  Layer *layer = calloc(1, sizeof(Layer));
  if (layer != NULL) {
    layer->zLevel = zLevel;
    layer->figures = NULL;
    layer->next = NULL;
    layer->name = name ? strdup(name) : NULL;
  }
  return layer;
}

void destroyLayer(Layer *layer) {
  logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
  if (layer != NULL) {
    Layer *current = layer;
    while (current != NULL) {
      Layer *next = current->next;

      if (current->name != NULL) {
        free(current->name);
      }

      if (current->figures != NULL) {
        destroyFigure(current->figures);
      }

      free(current);
      current = next;
    }
  }
}

Transform *createTransformScale(float sx, float sy) {
  Transform *t = calloc(1, sizeof(Transform));
  if (!t)
    return NULL;
  t->type = TRANSFORM_SCALE;
  t->value.scale.sx = sx;
  t->value.scale.sy = sy;
  return t;
}

Transform *createTransformScaleExpression(Expression *sx, Expression *sy) {
  Transform *t = calloc(1, sizeof(Transform));
  if (!t)
    return NULL;
  t->type = TRANSFORM_SCALE;
  t->isExpression = true;
  t->value.scale.sxExp = sx;
  t->value.scale.syExp = sy;
  return t;
}

Transform *createTransformRotate(float degrees) {
  Transform *t = calloc(1, sizeof(Transform));
  if (!t)
    return NULL;
  t->type = TRANSFORM_ROTATE;
  t->value.rotate.degrees = degrees;
  return t;
}

Transform *createTransformRotateExpression(Expression *degrees) {
  Transform *t = calloc(1, sizeof(Transform));
  if (!t)
    return NULL;
  t->type = TRANSFORM_ROTATE;
  t->isExpression = true;
  t->value.rotate.degreesExp = degrees;
  return t;
}

Transform *createTransformTranslate(float tx, float ty) {
  Transform *t = calloc(1, sizeof(Transform));
  if (!t)
    return NULL;
  t->type = TRANSFORM_TRANSLATE;
  t->value.translate.tx = tx;
  t->value.translate.ty = ty;
  return t;
}

Transform *createTransformTranslateExpression(Expression *tx, Expression *ty) {
  Transform *t = calloc(1, sizeof(Transform));
  if (!t)
    return NULL;
  t->type = TRANSFORM_TRANSLATE;
  t->isExpression = true;
  t->value.translate.txExp = tx;
  t->value.translate.tyExp = ty;
  return t;
}

void destroyTransform(Transform *t) {
  while (t) {
    Transform *next = t->next;
    if (t->isExpression) {
      switch (t->type) {
      case TRANSFORM_SCALE:
        if (t->value.scale.sxExp)
          destroyExpression(t->value.scale.sxExp);
        if (t->value.scale.syExp)
          destroyExpression(t->value.scale.syExp);
        break;
      case TRANSFORM_ROTATE:
        if (t->value.rotate.degreesExp)
          destroyExpression(t->value.rotate.degreesExp);
        break;
      case TRANSFORM_TRANSLATE:
        if (t->value.translate.txExp)
          destroyExpression(t->value.translate.txExp);
        if (t->value.translate.tyExp)
          destroyExpression(t->value.translate.tyExp);
        break;
      }
    }
    free(t);
    t = next;
  }
}

PaletteEntry *createPaletteEntry(const char *name, Color *color) {
  logDebugging(_logger, "Creating palette entry '%s'", name ? name : "(null)");
  PaletteEntry *entry = calloc(1, sizeof(PaletteEntry));
  if (!entry)
    return NULL;
  entry->name = name ? strdup(name) : NULL;
  entry->paletteName = NULL;
  entry->color = color;
  entry->next = NULL;
  return entry;
}

void destroyPalette(PaletteEntry *head) {
  logDebugging(_logger, "Destroying palette");
  while (head) {
    PaletteEntry *next = head->next;
    if (head->name)
      free(head->name);
    if (head->paletteName)
      free(head->paletteName);
    if (head->color)
      destroyColor(head->color);
    free(head);
    head = next;
  }
}

Color *duplicateColor(Color *original) {
  if (!original)
    return NULL;

  Color *copy = createColor(original->type);
  if (!copy)
    return NULL;

  switch (original->type) {
  case NAMED_COLOR:
    copy->value.name =
        original->value.name ? strdup(original->value.name) : NULL;
    break;
  case HEX_COLOR_TYPE:
    copy->value.hex = original->value.hex ? strdup(original->value.hex) : NULL;
    break;
  case RGB_COLOR_TYPE:
    copy->value.rgb.r = original->value.rgb.r;
    copy->value.rgb.g = original->value.rgb.g;
    copy->value.rgb.b = original->value.rgb.b;
    break;
  case RGBA_COLOR_TYPE:
    copy->value.rgba.r = original->value.rgba.r;
    copy->value.rgba.g = original->value.rgba.g;
    copy->value.rgba.b = original->value.rgba.b;
    copy->value.rgba.a = original->value.rgba.a;
    break;
  }
  return copy;
}

PaletteEntry *duplicatePalette(PaletteEntry *original) {
  if (!original)
    return NULL;

  PaletteEntry *head = NULL;
  PaletteEntry *tail = NULL;

  for (PaletteEntry *it = original; it; it = it->next) {
    Color *colorCopy = duplicateColor(it->color);
    PaletteEntry *entry = createPaletteEntry(it->name, colorCopy);
    if (it->paletteName)
      entry->paletteName = strdup(it->paletteName);

    if (!head) {
      head = tail = entry;
    } else {
      tail->next = entry;
      tail = entry;
    }
  }

  return head;
}
