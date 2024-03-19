#ifndef STATEMENT_TYPE_DEFINED
#define STATEMENT_TYPE_DEFINED

enum class StatementType {
    VariableSet,       // 0
    VariableInit,      // 1
    FunctionCall,      // 2
    MethodDefinition,  // 3
    If,                // 4
    Return,            // 5
    While,             // 6
    EnterScope,        // 7
    ExitScope,         // 8
    Throw,             // 9
    TryCatch,          // 10
    LoadLib            // 11
};

class Statement {
public:
    StatementType Type;

    // As this is an abstract class in C#, you might want to include some virtual functions here
    // that will be overridden in the derived classes.
    virtual ~Statement() = default; // Virtual destructor
};

#endif // STATEMENT_TYPE_DEFINED