#include <utility>
#include <vector>

class IfStatement : public Statement {
public:
    StatementType Type = StatementType::If;
    Value* Condition;
    std::vector<Statement*> Statements;
    std::vector<Statement*> ElseStatements;

    IfStatement(Value* condition, std::vector<Statement*> statements, std::vector<Statement*> elseStatements) {
        Condition = condition;
        Statements = std::move(statements);
        ElseStatements = std::move(elseStatements);
    }

    IfStatement() = default;
};