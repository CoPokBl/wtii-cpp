#include "Statement.h"

class ExitScopeStatement : public Statement {
public:
    ExitScopeStatement() {
        Type = StatementType::ExitScope;
    }
};