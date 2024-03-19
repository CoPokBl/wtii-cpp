#include "Statement.h"

class EnterScopeStatement : public Statement {
public:
    EnterScopeStatement() {
        Type = StatementType::EnterScope;
    }
};