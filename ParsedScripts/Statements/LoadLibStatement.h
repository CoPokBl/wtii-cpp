#include <string>
#include <utility>

class LoadLibStatement : public Statement {
public:
    StatementType Type = StatementType::LoadLib;
    std::string Lib;

    explicit LoadLibStatement(std::string lib) {
        Lib = std::move(lib);
    }

    LoadLibStatement() = default;
};