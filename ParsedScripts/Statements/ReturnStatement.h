#include "../Values/Value.h"

class ReturnStatement : public Statement {
public:
    StatementType Type = StatementType::Return;
    Value* RetValue;

    ReturnStatement() = default;

    explicit ReturnStatement(class Value* value) {
        RetValue = value;
    }
};