# Compiler Development Plan: From Parse Tree to Assembly

## Current State Analysis

### ✅ Implemented Components
- **Lexer**: Tokenizes input with rich token types (including C-like keywords)
- **Parser**: Builds AST for expressions (binary, unary, literals, grouping)
- **Visitor Pattern**: For AST traversal with `visitor.hpp`
- **AST Printer**: For debugging parse trees
- **Expression Support**: Binary operations, unary operations, literals, grouping

### 🎯 Target Language Features
Based on `test/hello_world.ko`:
```c
int main() {
    print("Hello World");
    return 0;
}
```

## Implementation Roadmap

### Phase 1: Statement Support (Essential Foundation)
**Priority: HIGH** - Currently only handles expressions, need statements for real programs

#### 1.1 Extend Parser for Statements
- **Expression statements**: `expr;`
- **Variable declarations**: `int x = 5;`
- **Block statements**: `{ ... }`
- **Return statements**: `return expr;`
- **Function declarations**: `int main() { ... }`

#### 1.2 Update AST Classes
```cpp
// Add to parser.hpp:
class Statement { virtual ~Statement() = 0; };
class ExpressionStatement : public Statement;
class VarDeclaration : public Statement;
class Block : public Statement;
class ReturnStatement : public Statement;
class FunctionDeclaration : public Statement;
```

#### 1.3 Extend Visitor Pattern
- Create `StatementVisitor` interface
- Update existing visitors to handle statements
- Add statement printing capabilities

### Phase 2: Semantic Analysis (Critical for Correctness)
**Priority: HIGH** - Required before any code generation

#### 2.1 Symbol Table Implementation
```cpp
struct Symbol {
    std::string name;
    std::string type;
    int scope_level;
    bool is_initialized;
    // Memory location info for codegen
};
```

#### 2.2 Scope Management
- **Scope stack**: Track nested scopes (functions, blocks)
- **Variable resolution**: Lookup variables in appropriate scope
- **Shadowing handling**: Inner scope variables hide outer scope

#### 2.3 Type System
- **Type checking**: Ensure binary operations use compatible types
- **Function signatures**: Validate parameter types and return types
- **Type inference**: Determine expression result types
- **Error reporting**: Clear semantic error messages

#### 2.4 Declaration Analysis
- **Variable declaration**: Ensure variables declared before use
- **Function declaration**: Track function signatures
- **Duplicate detection**: Prevent redeclaration in same scope

### Phase 3: Extended Language Features
**Priority: MEDIUM** - Needed for practical programs

#### 3.1 Function Call Support
- **Call expression**: `function(arg1, arg2)`
- **Parameter passing**: Type checking and evaluation order
- **Built-in functions**: `print()` and other system functions
- **Return value handling**: Expression evaluation

#### 3.2 Control Flow Statements
- **If statements**: `if (condition) { ... } else { ... }`
- **While loops**: `while (condition) { ... }`
- **For loops**: `for (init; condition; update) { ... }` (future)

#### 3.3 Variable Assignment
- **Assignment expressions**: `x = expr`
- **Compound assignments**: `x += expr` (future)
- **Increment/decrement**: `++x`, `x--` (future)

### Phase 4: Code Generation Infrastructure
**Priority: HIGH** - Foundation for assembly generation

#### 4.1 Target Architecture Definition
- **Choose target**: x86-64, ARM64, or custom VM
- **Instruction set**: Define supported assembly instructions
- **Calling conventions**: Function call/return mechanisms
- **Memory model**: Stack layout, heap management

#### 4.2 Register Management
- **Register allocation**: Simple algorithm (linear scan or graph coloring)
- **Spilling strategy**: Handle when more variables than registers
- **Calling convention**: Preserve registers across function calls

#### 4.3 Memory Layout
- **Stack frame**: Local variables, parameters, return addresses
- **Global variables**: Static memory allocation
- **String literals**: Read-only data section

### Phase 5: Code Generation Implementation
**Priority: HIGH** - The actual assembly generation

#### 5.1 Expression Code Generation
- **Arithmetic operations**: Convert AST to instruction sequences
- **Register assignment**: Allocate registers for temporaries
- **Constant folding**: Optimize compile-time constants

#### 5.2 Statement Code Generation
- **Variable declarations**: Stack allocation
- **Function calls**: Parameter setup, call instruction, cleanup
- **Control flow**: Labels and jumps for if/while statements
- **Function prologue/epilogue**: Stack frame setup/teardown

#### 5.3 Assembly Output
- **Instruction emission**: Generate actual assembly code
- **Label management**: Unique labels for jumps
- **Data section**: String literals and global variables
- **Entry point**: Program initialization

### Phase 6: Optimization (Optional)
**Priority: LOW** - Performance improvements

#### 6.1 Intermediate Representation (IR)
- **Three-address code**: Simplified instruction format
- **Basic blocks**: Straight-line code sequences
- **Control flow graph**: Represents program structure

#### 6.2 Optimizations
- **Constant folding**: Evaluate constants at compile time
- **Dead code elimination**: Remove unused code
- **Common subexpression**: Avoid duplicate calculations

## Implementation Priority Order

1. **Start Here**: Extend parser to handle statements (especially function declarations)
2. **Next**: Implement basic symbol table for variable tracking
3. **Then**: Add type checking for expressions and assignments
4. **After**: Function call parsing and semantic analysis
5. **Finally**: Choose target architecture and implement code generation

## Testing Strategy

### Test Files Needed
- `test/statements/` - Variable declarations, assignments, blocks
- `test/functions/` - Function definitions and calls
- `test/semantic/` - Type errors, undeclared variables
- `test/codegen/` - Assembly output verification

### Test Categories
- **Parser tests**: Verify AST structure for new constructs
- **Semantic tests**: Check error detection and symbol table
- **Integration tests**: Full programs that compile and run
- **Code generation tests**: Verify correct assembly output

## Architecture Decisions

### Code Organization
```
src/
├── lexer.hpp/.cpp          # ✅ Done
├── parser.hpp/.cpp         # ✅ Expressions only
├── statements.hpp/.cpp     # 📝 TODO: Statement AST nodes
├── semantic.hpp/.cpp       # 📝 TODO: Symbol table & type checking
├── codegen.hpp/.cpp        # 📝 TODO: Assembly generation
├── visitors/               # 📝 TODO: Separate visitor implementations
│   ├── ast_printer.hpp/.cpp
│   ├── semantic_analyzer.hpp/.cpp
│   └── code_generator.hpp/.cpp
└── main.cpp               # ✅ Done (with --print flags)
```
