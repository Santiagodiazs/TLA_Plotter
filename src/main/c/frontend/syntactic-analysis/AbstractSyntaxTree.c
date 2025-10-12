#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

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

/* PUBLIC FUNCTIONS */

void destroyConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void destroyExpression(Expression * expression) {
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

void destroyFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				destroyConstant(factor->constant);
				break;
			case EXPRESSION_FACTOR:
				destroyExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void destroyScene(Scene * scene) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (scene != NULL) {
		if (scene->name != NULL) {
			free(scene->name);
		}
		if (scene->backgroundColor != NULL) {
			free(scene->backgroundColor);
		}
		destroyFigure(scene->figures);
		free(scene);
	}
}

void destroyProgram(Program * program) {
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

void destroyFigure(Figure * figure) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (figure != NULL) {
		Figure * current = figure;
		while (current != NULL) {
			Figure * next = current->next;
			
			if (current->id != NULL) {
				free(current->id);
			}
			destroyProperty(current->properties);
			
			free(current);
			current = next;
		}
	}
}

void destroyProperty(Property * property) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (property != NULL) {
		Property * current = property;
		while (current != NULL) {
			Property * next = current->next;
			
			// Liberar valores string si es necesario
			if (current->type == FILL_PROPERTY || current->type == STROKE_PROPERTY) {
				if (current->value.stringValue != NULL) {
					free(current->value.stringValue);
				}
			}
			
			free(current);
			current = next;
		}
	}
}
