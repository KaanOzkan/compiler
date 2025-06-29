# Minimal Compiler Plan

### 🎯 Goals

1 - Compile `test/asm/hello_world.ko` into working assembly:
```c
int main() {
    print("Hello World");
    return 0;
}
```

## What We Actually Need (Minimal Path)

### Step 1: Parse Statements (Currently Missing)
**Why needed**: We can only parse expressions like `1+2`, but hello_world has statements

**Add to parser.hpp**:
```cpp
class Statement {
public:
    virtual ~Statement() {}
    virtual std::string accept(class Visitor *) = 0;
};

class FunctionDecl : public Statement {
    std::string name;           // "main"
    std::string return_type;    // "int"  
    std::unique_ptr<Block> body;
};

class Block : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;
};

class FunctionCall : public Expression {
    std::string name;           // "print"
    std::vector<std::unique_ptr<Expression>> args;
};

class ReturnStmt : public Statement {
    std::unique_ptr<Expression> value;  // 0
};
```

**Extend parser.cpp**:
- `parse_function_declaration()` - for `int main() { ... }`
- `parse_block()` - for `{ ... }`  
- `parse_function_call()` - for `print(...)`
- `parse_return_statement()` - for `return 0;`

### Step 2: Basic Symbol Table (Minimal)
**Why needed**: Track that `main` exists and `print` is built-in

```cpp
struct Symbol {
    std::string name;
    std::string type;
    bool is_builtin;
};

class SymbolTable {
    std::unordered_map<std::string, Symbol> symbols;
public:
    void add_builtin(std::string name, std::string type);
    void add_function(std::string name, std::string return_type);
    Symbol* lookup(std::string name);
};
```

**Add built-ins**:
- `print(string) -> void`
- `main() -> int`

### Step 3: Simple Code Generator 
**Why needed**: Turn IR into runnable assembly

**Choose target**: arm (simple syscalls)

## Implementation Order

### Phase 1: Parser Extensions (1-2 days)
- [ ] Add Statement base class
- [ ] Parse function declarations  
- [ ] Parse function calls
- [ ] Parse return statements
- [ ] Parse block statements
- [ ] Test: `./run.out test/asm/hello_world.ko --print=parser` shows full AST

### Phase 2: Minimal Semantic Analysis (1 day)
- [ ] Create basic symbol table
- [ ] Add built-in functions (print)
- [ ] Validate function calls exist
- [ ] Test: No "undefined function" errors

### Phase 3: Code Generation (2-3 days)
- [ ] Choose ARM target
- [ ] Generate function prologue/epilogue
- [ ] Generate function calls (built-ins)
- [ ] Generate return statements
- [ ] Generate string literals
- [ ] Test: Assembly compiles with `gcc` and runs

## What We're Skipping

❌ **Complex type system** - hello_world only uses `int` and `string`
❌ **Variable declarations** - hello_world has no variables
❌ **Control flow** - no if/while/for in hello_world
❌ **Scope management** - only one function
❌ **Optimizations** - learning exercise, not performance
❌ **Error recovery** - just fail fast for now
❌ **Multiple files** - single file compilation
❌ **Complex expressions** - hello_world only has literals
