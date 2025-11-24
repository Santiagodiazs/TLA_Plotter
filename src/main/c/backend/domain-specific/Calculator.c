#include "Calculator.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownCalculatorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Calculator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeCalculatorModule() {
	_logger = createLogger("Calculator");
	return _shutdownCalculatorModule;
}

/** PUBLIC FUNCTIONS */

ComputationResult add(const int leftAddend, const int rightAddend) {
    return (ComputationResult){.succeeded = true, .value = leftAddend + rightAddend};
}

ComputationResult divide(const int dividend, const int divisor) {
    if (divisor == 0) {
        logError(_logger, "Division by zero");
        return (ComputationResult){.succeeded = false, .value = 0};
    }
    return (ComputationResult){.succeeded = true, .value = dividend / divisor};
}

ComputationResult multiply(const int multiplicand, const int multiplier) {
    return (ComputationResult){.succeeded = true, .value = multiplicand * multiplier};
}

ComputationResult subtract(const int minuend, const int subtract) {
    return (ComputationResult){.succeeded = true, .value = minuend - subtract};
}

ComputationResult computeConstant(Constant * constant) {
    if (!constant) return (ComputationResult){.succeeded = false, .value = 0};
    return (ComputationResult){.succeeded = true, .value = constant->value};
}

ComputationResult computeExpression(Expression * expression, Variable * variables) {
    if (!expression) return (ComputationResult){.succeeded = false, .value = 0};

    switch (expression->type) {
        case ADDITION: {
            ComputationResult left = computeExpression(expression->leftExpression, variables);
            ComputationResult right = computeExpression(expression->rightExpression, variables);
            if (left.succeeded && right.succeeded) {
                return add(left.value, right.value);
            }
            break;
        }
        case SUBTRACTION: {
            ComputationResult left = computeExpression(expression->leftExpression, variables);
            ComputationResult right = computeExpression(expression->rightExpression, variables);
            if (left.succeeded && right.succeeded) {
                return subtract(left.value, right.value);
            }
            break;
        }
        case MULTIPLICATION: {
            ComputationResult left = computeExpression(expression->leftExpression, variables);
            ComputationResult right = computeExpression(expression->rightExpression, variables);
            if (left.succeeded && right.succeeded) {
                return multiply(left.value, right.value);
            }
            break;
        }
        case DIVISION: {
            ComputationResult left = computeExpression(expression->leftExpression, variables);
            ComputationResult right = computeExpression(expression->rightExpression, variables);
            if (left.succeeded && right.succeeded) {
                return divide(left.value, right.value);
            }
            break;
        }
        case FACTOR:
            return computeFactor(expression->factor, variables);
        default:
            break;
    }
    return (ComputationResult){.succeeded = false, .value = 0};
}

ComputationResult computeFactor(Factor * factor, Variable * variables) {
    if (!factor) return (ComputationResult){.succeeded = false, .value = 0};

    switch (factor->type) {
        case CONSTANT:
            return computeConstant(factor->constant);
        case EXPRESSION_FACTOR:
            return computeExpression(factor->expression, variables);
        case VARIABLE_FACTOR:
            if (factor->variableName) {
                Variable * current = variables;
                while (current) {
                    if (strcmp(current->name, factor->variableName) == 0) {
                        return computeExpression(current->value, variables);
                    }
                    current = current->next;
                }
                logError(_logger, "Variable not found: %s", factor->variableName);
            }
            break;
        default:
            break;
    }
    return (ComputationResult){.succeeded = false, .value = 0};
}

ComputationResult executeCalculator(CompilerState * compilerState) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

