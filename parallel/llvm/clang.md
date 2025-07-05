许多类似C 语言的方言或语言扩展也开始使用Clang 托管它们的实现，例如：Clang 为OpenCL、OpenMP 和CUDA C/C++ 提供官方支持。除了常规的前端工作外，Clang 一直在发展，将其功能划分为库和模块，这样开发人员可以使用它们来创建各种与源代码处理相关的工具，例如：代码重构、代码格式化和语法高亮显示。

编译
cmake -G Ninja -DLLVM_ENABLE_PROJECTS="clang;clang-toolsextra" …

如果已经有了未启用Clang 的LLVM 构建，可以在CMakeCache.txt 中编辑CMake参数LLVM_ENABLE_PROJECTS，而不需要再次使用CMake 命令

LLVM 将其大部分任务安排在管道(即PassManager) 中顺序执行。与LLVM 不同，Clang 在组织子组件的方式上更加多样化。


编译源代码是一个复杂的过程。首先，它由多个阶段组成
• 前端: 解析和语义检查
• 中端: 程序分析和优化
• 后端: 原生代码生成
• 汇编: 运行汇编器
• 链接: 运行链接器


观察驱动程序的方法是在正常使用clang 时添加-### 命令行标志。
clang++ -### -std=c++11 -Wall ./hello_world.cpp -o hello_world

要体验AST，可以使用下面的命令(从源代码中) 打印出一个AST
clang -Xclang -ast-dump -fsyntax-only foo.c


Clang 项目不仅包含clang 可执行文件，还为开发人员提供接口来扩展其工具，并可以将其功能导出为库
Clang 中目前有三种工具和扩展选项:Clang 插件、libTooling 和Clang 工具。

FrontendAction ,Clang 的前端组件都封装在这里面。通常情况下，不会直接从FrontendAction 派生，但是理解FrontendAction 在Clang 中的内部角色和接口，在使用工具或插件开发，给你带来更多的思考。

Clang 插件可以动态注册新的FrontendAction(更确切地说是ASTFrontendAction)，可以在 clang 的主动作之前或之后处理AST，甚至替换。

通过使用简单的命令行选项，插件可以很容易地加载到clang 中
clang -fplugin=/path/to/MyPlugin.so … foo.cpp

LibTooling 是一个库，提供了在Clang 技术之上构建独立工具的特性。可以像在项目中使用普通库一样使用它，而不需要依赖于clang 可执行文件。此外，API 设计的更高层，这样就不需要处理很多Clang 的内部细节，这使得它对非Clang 开发人员更友好。
语言服务器是libTooling 最著名的用例

1 int main(int argc, char** argv) {
2 CommonOptionsParser OptionsParser(argc, argv,…);
3 ClangTool Tool(OptionsParser.getCompilations(), {"foo.cpp"});
4 return Tool.run(newFrontendActionFactory<MyCustomAction>().
5 get());
6 }


https://clang.llvm.org/docs/Tooling.html


SourceLocation 和SourceManager
1. SourceLocation 类用于表示代码在文件中的位置
2. 由于SourceLocation 是指针，所以只有当它与刚才提到的数据缓冲区一起使用才有意义，数据缓冲区是由本文中的第二个主要角色SourceManager 管理的。

1 void foo(SourceManager &SM, SourceLocation SLoc) {
2 auto Line = SM.getSpellingLineNumber(SLoc),
3 Column = SM.getSpellingColumnNumber(SLoc);
4 …
5 }

Clang 的预处理器和词法分析器执行的角色和主要操作，分别由Preprocessor 和Lexer 类表示

1 Token GetNextToken(Preprocessor &PP) {
2 Token Tok;
3 PP.Lex(Tok);
4 return Tok;
5 }


查看token 信息
clang -fsyntax-only -Xclang -dump-tokens foo.cc

从token类型中检索标识符信息
1 IdentifierInfo *II = Tok.getIdentifierInfo();

处理宏
MacroDirective:该类是给定宏标识符的#define 或#undef 语句j 进行逻辑表示
MacroDefinition:该类表示当前时间点的宏标识符的定义
MacroInfo:该类包含宏体，包括宏体中的标记和宏定义的宏参数(如果有的话)

1 void printMacroBody(IdentifierInfo *MacroII, Preprocessor &PP)
2 {
3 MacroDefinition Def = PP.getMacroDefinition(MacroII);
4 MacroInfo *Info = Def.getMacroInfo();
5 for(Token Tok : Info->tokens()) {
6 std::cout << Tok.getName() << "\n";
7 }
8 }

Clang 的预处理框架也提供通过插件插入自定义逻辑的方法

clang … -fplugin=/path/to/MacroGuardPlugin.so foo.c

./bin/clang -isysroot $(xcrun --show-sdk-path) -fplugin=./lib/MacroGuardPlugin.dylib foo.c

AST
1. 内存表示
在Clang 中，AST 的内存表示为一个层次结构，类似于C 族语言程序的语法结构。
TranslationUnitDecl: 该类表示一个输入源文件，也称为翻译单元(Translation Unit) 全局变量、类和函数 作为子声明，其中每个顶级声明都有自己的子树，子树递归地定义AST 的其余部分。
ASTContext: 顾名思义，这个类跟踪所有AST 节点和来自输入源文件的其他元数据
AST 的主体(AST 节点) 还可以进一步分为三个主要类别: 声明、语句和表达式。这些类别中的节点分别由派生自Decl、Stmt 和Expr 类的子类表示。

2. 让我们了解如何在Clang AST 中建模类型。
Clang AST 类型系统的核心是clang::Type 类。输入代码中的每种类型——包括int 这样的基本类型和struct/class 这样的用户定义类型——都由一个单例类型(更具体地说，是Type 的子类) 对象表示。
QualType 表示限定类型。它充当Type 的包装器，以表示 const <type>、volatile <type> 和restrict <type>*。

1 // If `T` is representing 'int'…
2 QualType toConstVolatileTy(Type *T) {
3 return QualType(T, Qualifier::Const | Qualifier::Volatile);
4 } // Then the returned QualType represents `volatile const int`

ASTMatcher 是一个实用工具，可以通过干净、简洁和高效的领域特定语言(DSL) 编写AST模式匹配逻辑。
MatchFinder 是模式匹配过程中常用的驱动程序，基本用法非常简单:

1 using namespace ast_matchers;
2 …
3 MatchFinder Finder;
4 // Add AST matching patterns to `MatchFinder`
5 Finder.addMatch(traverse(TK_AsIs, pattern1), Callback1);
6 Finder.addMatch(traverse(TK_AsIs, pattern2), Callback2);
7 …
8 // Match a given AST. `Tree` has the type of `ASTContext&`
9 // If there is a match in either of the above patterns,
10 // functions in Callback1 or Callback2 will be invoked
11 // accordingly
12 Finder.matchAST(Tree);
13
14 // …Or match a specific AST node. `FD` has the type of
15 // `FunctionDecl&`
16 Finder.match(FD, Tree);

TK_IgnoreUnlessSpelledInSource vs TK_AsIs
TK_IgnoreUnlessSpelledInSource 用于匹配AST 中的标识符，而不管它们是否在源文件中拼写。
TK_AsIs 用于匹配AST 中的标识符，而不管它们是否在源文件中拼写。

DSL 语法
https://clang.llvm.org/docs/LibASTMatchersReference.html
调用bind(…) 来将匹配的Expr AST 节点与exit_condition 的名称相关联。


Clang 的诊断框架由三个主要部分组成:
• 诊断ID
• 诊断引擎
• 诊断用户(客户端)
诊断消息由唯一的诊断ID(与消息模板及其诊断级别相关联) 和数据组成。
诊断消息发送到诊断引擎(由DiagnosticsEngine 类表示) 之后，引擎将消息格式化为文本内容，并将它们发送给诊断用户(在代码库中也称为客户端)。


Clang 使用TableGen 语法来声明所有类型的编译器标志——包括驱动标志和前端标志。
所有这些标志声明都放在clang/include/clang/Driver/Options.td 中
许多驱动标志转换发生的地方是在driver::tools::Clang 类中。更具体地说，这发生在它的Clang::ConstructJob 方法中，该方法位于clang/lib/Driver/ToolChains/Clang.cpp 文件中。
