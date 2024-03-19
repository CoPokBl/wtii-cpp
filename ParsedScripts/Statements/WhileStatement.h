#include <utility>
#include <vector>

class WhileStatement : public Statement {
public:
    StatementType Type = StatementType::While;
    Value* Condition = nullptr;
    std::vector<Statement*> Statements;

    WhileStatement() = default;

    WhileStatement(Value* condition, std::vector<Statement*> statements) {
        Condition = condition;
        Statements = std::move(statements);
    }
};