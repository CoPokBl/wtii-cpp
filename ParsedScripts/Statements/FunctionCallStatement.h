#include <utility>
#include <vector>
#include <string>
#include "../Values/Value.h"

class FunctionCallStatement : public Statement {
public:
    StatementType Type = StatementType::FunctionCall;
    std::vector<std::string> Path;
    std::vector<Value*> Arguments;

    FunctionCallStatement(std::vector<std::string> path, std::vector<Value*> args) {
        Path = std::move(path);
        Arguments = std::move(args);
    }

    FunctionCallStatement() = default;
};