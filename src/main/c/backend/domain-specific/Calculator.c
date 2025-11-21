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
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult divide(const int dividend, const int divisor) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult multiply(const int multiplicand, const int multiplier) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult subtract(const int minuend, const int subtract) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult computeConstant(Constant * constant) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult computeExpression(Expression * expression) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult computeFactor(Factor * factor) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

ComputationResult executeCalculator(CompilerState * compilerState) {
    return (ComputationResult){.succeeded = true, .value = 0};
}

