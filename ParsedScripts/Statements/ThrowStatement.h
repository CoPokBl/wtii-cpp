#include "Statement.h"
#include "../Values/Value.h"

class ThrowStatement : public Statement {
public:
    StatementType Type = StatementType::Throw;
    Value* Exception{};

    ThrowStatement() = default;

    explicit ThrowStatement(Value* exception) {
        Exception = exception;
    }
};