#include <string>
#include <utility>
#include <vector>

class TryCatchStatement : public Statement {
public:
    StatementType Type = StatementType::TryCatch;
    std::vector<Statement*> Statements;
    std::vector<Statement*> CatchStatements;
    std::string ExceptionName;

    TryCatchStatement() = default;

    TryCatchStatement(std::vector<Statement*> statements, std::vector<Statement*> catchStatements, std::string exceptionName) {
        Statements = std::move(statements);
        CatchStatements = std::move(catchStatements);
        ExceptionName = std::move(exceptionName);
    }
};