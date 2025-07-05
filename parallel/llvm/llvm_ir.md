在LLVM IR 上执行编译过程后，LLVM 所做的大部分工作是使输入程序运行更快或体积更小

虽然查看LLVM IR 最直接和可视化的方式是通过文本表示，但LLVM 提供了包含一组强大
的现代C++ API 的库来与IR 进行交互。这些API 可以检查LLVM IR 在内存中的表示，并帮助
我们操作，这将有效地改变正在编译的目标程序

我们将使用Graphviz 提供的命令行工具——dot——来可视化函数的控制流。

LLVM IR 以分层的方式组织。这个层次结构中的层次——从顶部开始计算——是模块、函数、
基本块和指令。
一个模块代表一个翻译单元——通常是一个源文件。每个模块可以包含多个函数(或全局变
量)。每个函数都包含一个基本块列表，每个基本块包含一个指令列表。
基本块表示一个只有一个入口和一个出口的指令列表。换句话说，如果执行一个基本块，
控制流需要保证遍历块中的每一条指令。

我们可以使用下面的clang 命令来生成文本形式的LLVM IR:
$ clang -emit-llvm -S foo.c

在文本形式中，指令通常以以下格式呈现:
<result> = <operator / op-code> <type>, [operand1, operand2, …]


以下是LLVM 中最重要的一些内容:
• 控制流图(CFG): 这是一种组织成基本块的图结构，以显示控制流关系。图中的顶点代表基
本块，而边代表单个控制流传输路径。
• 循环: 这表示我们熟悉的循环，它由多个基本块组成，至少有一条后边——一条返回父顶点或
祖先顶点的控制流边。我们将在本章的最后一节中更详细地进行讨论。
• 调用图: 与CFG 类似，调用图也显示了控制流传输，但是顶点变成了单独的函数，而边变成
了函数调用关系。

代码部分
1. 迭代指令
1 // `F` has the type of `Function&`
2 for (BasicBlock &BB : F) {
3 for (Instruction &I : BB) {
4 // Work on `I`
5 }
6 }
其次，可以利用一个名为inst_iterator 的程序。下面是一个例子:
1 #include "llvm/IR/InstIterator.h"
2 …
3 // `F` has the type of `Function&`
4 for (Instruction &I : instructions(F)) {
5 // Work on `I`
6 }

观察指令
在很多情况下，我们希望对一个基本块或函数中的不同类型的指令应用有不同的处理方法。例
如，我们有以下代码:
1 for (Instruction &I : instructions(F)) {
2 switch (I.getOpcode()) {
3 case Instruction::BinaryOperator:
4 // this instruction is a binary operator like `add` or `sub`
5 break;
6 case Instruction::Return:
7 // this is a return instruction
8 break;
9 …
10 }
11 }

LLVM 提供了一种更好的方法来实现这种访问模式——Instvisitor

2. 迭代基本块

生成cfg文件
$ opt -passes="dot-cfg" -disable-output test_foo.ll
$ dot -Tpng .foo.cfg.dot -o test_foo.cfg.png

拓扑排序遍历
1 #include "llvm/ADT/PostOrderIterator.h"
2 #include "llvm/IR/CFG.h"
3 // `F` has the type of `Function*`
4 for (BasicBlock *BB : post_order(F)) {
5 BB->printAsOperand(errs());
6 errs() << "\n";
7 }

dfs /bfs 遍历
1 #include "llvm/ADT/DepthFirstIterator.h"
2 #include "llvm/IR/CFG.h"
3 // `F` has the type of `Function*`
4 for (BasicBlock *BB : depth_first(F)) {
5 BB->printAsOperand(errs());
6 errs() << "\n";
7 }

SSC 遍历
SCC 表示一个子图，其中每个外围节点都可以从其他节点到达。在CFG 的上下文中，用循
环遍历CFG 是很有用的。
使用LLVM 中的scc_iterator，我们可以遍历CFG 中强连接的基本块。有了这些信息，可
以快速地发现循环的控制流，这对于一些分析和程序转换任务至关重要，例如：需要知道后面的边
和循环的基本块，以便沿着控制流的边准确地传播分支概率数据。
1 #include "llvm/ADT/SCCIterator.h"
2 #include "llvm/IR/CFG.h"
3 // `F` has the type of `Function*`
4 for (auto SCCI = scc_begin(&F); !SCCI.isAtEnd(); ++SCCI) {
5 const std::vector<BasicBlock*> &SCC = *SCCI;
6 for (auto *BB : SCC) {
7 BB->printAsOperand(errs());
8 errs() << "\n";
9 }
10 errs() << "====\n";
11 }

3. 迭代调用图

1 #include "llvm/Analysis/CallGraph.h"
2 struct SimpleIPO : public PassInfoMixin<SimpleIPO> {
3 PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
4 {
5 CallGraph CG(M);
6 for (auto &Node : CG) {
7 // Print function name
8 if (Node.first)
9 errs() << Node.first->getName() << "\n";
10 }
11 return PreservedAnalysis::all();
12 }
13 };

基本块的迭代，调用图也适用
GraphTraits 是一个类，旨在为LLVM-CFG 中的各种不同的图和调用图提供抽象接口，允许
其他LLVM 组件(如我们在前一节中看到的分析、转换或迭代器实用程序) 独立于底层图构建它们
的工作。与要求LLVM 中的每个图都继承GraphTraits，并实现所需的函数不同，GraphTraits
采用了一种完全不同的方法，即使用模板特化。
1 // After the original declaration of struct Distance…
2 template<>
3 struct Distance<SimplePoint> {
4 SimplePoint compute(SimplePoint &A, SimplePoint &B) {
5 return std::sqrt(std::pow(A.X – B.X, 2),…);
6 }
7 };
8 …
9 SimplePoint A, B;
10 Distance<SimplePoint>::compute(A, B); // Success

1 // `F` has the type of `Function*`
2 BasicBlock *TailBB = find_tail(F);
3 // `CG` has the type of `CallGraph*`
4 CallGraphNode *TailCGN = find_tail(CG);



值和指令
在LLVM 中，值是有独特的构造——不仅表示存储在变量中的值，还模拟了从常量、全局变
量、单个指令，甚至基本块等广泛的概念。换句话说，它是LLVM IR 的基础。
为了学习如何在LLVM IR 中使用值，我们必须理解这个系统背后的重要理论，它规定了LLVM
指令的行为和格式——单一静态赋值(SSA) 形式。

1. 在SSA 中，一个变量(在IR 中) 只会赋值一次。
2. 我们可以把注意力集中在指令所产生的数据上，从而清楚地了解每条指令的来源——也就是结
果值可以到达的区域。此外，我们可以很容易地找到任意变量/值的原点。换句话说，SSA 显示程序中的数据流，以便编译器更容易地跟踪、分析和修改指令。

因为LLVM IR 中的每条指令只能产生一个结果值，Instruction
对象——Instruction 是在LLVM IR 中表示一条指令的C++ 类——也可以表示它的结果值。更
具体地说，LLVM IR 中值的概念是由一个C++ 类Value 表示的。Instruction 是它的子类。这
意味着给定一个Instruction 对象，当然，我们可以将它强制转换为一个Value 对象。这个特定
的Value 对象实际上是该Instruction 的结果:
1 // let's say `I` represents an instruction `x = a + b`
2 Instruction *I = …;
3 Value *V = I; // `V` effectively represents the value `x`

1 Instruction *BinI = BinaryOperator::Create(Instruction::Add,…);
2 Instruction *RetI = ReturnInst::Create(…, BinI, …);

除了Instruction 之外，Constant(C++ 中用于不同类型常量的类)、GlobalVariable(C++
中用于全局变量的类) 和BasicBlock 都是Value 的子类。这意味着它们也是以SSA 的形式组织
的，并且可以将它们用作指令的操作数。

但“值”的概念在LLVM 中是如何体现的呢？
下面的图表显示了两个回答这个问题的重要C++ 类——User 和Use
多个Use 实例可能指向一个User，这意味着User 使用了多个Value 实例。可以使用User
提供的value_op_iterator 来检索每个Value 实例，例如:
1 // `Usr` has the type of `User*`
2 for (Value *V : Usr->operand_values()) {
3 // Working with `V`
4 }
一个Value 实例可以被多个不同的User 实例使用。类似地，我们可以使用下面的片段来遍
历它们:
1 // `V` has the type of `Value*`
2 for (User *Usr : V->users()) {
3 // Working with `Usr`
4 }

Value::replaceAllUsesWith
User::replaceUsesOfWith(From,To)


指令操作
• 不同指令类型之间的强制转换
实际上，dynamic_cast 具有我们正在寻找的确切功能——更正式地说，是运行时类型信息
(RTTI) 特性——但它在运行时性能方面也有很高的开销。糟糕的是，C++ 中RTTI 的默认实现
相当复杂，使得生成的程序难以优化。因此，LLVM 默认关闭RTTI 功能。由于这个原因，LLVM
提出了自己的运行时类型转换系统，这个系统更加简单和高效
LLVM 的类型转换框架为动态类型转换提供了三个功能:
    • isa<T>(val)
    • cast<T>(val)
    • dyn_cast<T>(val)
1 // `I` has the type of `Instruction*`
2 if (isa<BinaryOperator>(I)) {
3 BinaryOperator *BinOp = cast<BinaryOperator>(I);
4 }
最后一个函数dyn_cast<T>，是isa<T> 和cast<T> 的组合。如果适用，可以执行类型转换。
否则，返回null。下面是一个例子:
1 // `I` has the type of `Instruction*`
2 if (BinaryOperator *BinOp = dyn_cast<BinaryOperator>(I)) {
3 // Work with `BinOp`
4 }

• 插入新指令
1 Instruction *BinI = BinaryOperator::Create(…);
2 Instruction *RetI = ReturnInst::Create(…, BinI, …);
正如方法名Create 所示，我们可以推断这两行创建了一个BinaryOperator 和一个returnst
指令。LLVM 鼓励使用这些工厂方法，而不是通过new 关键字或malloc 函数手动分配指令对象。将
指令插入到BasicBlock 中时，LLVM 将为你管理指令对象的内存。
    1. Create 方法插入指令
    2. Instruction 类提供的insertBefore/insertAfter 方法来插入新指令
    3. 我们也可以使用IRBuilder 类，他是一个强大的工具，可以自动化一些指令创建和插入步骤，
    实现了一个构建器设计模式。当开发人员调用它的一个创建方法时，可以逐个地插入新的指
    令:
    1 // `BB` has the type of `BasicBlock*`
    2 IRBuilder<> Builder(BB /*the insertion point*/);
    3 // insert a new addition instruction at the end of `BB`
    4 auto *AddI = Builder.CreateAdd(LHS, RHS);
    5 // Create a new `ReturnInst`, which returns the result
    6 // of `AddI`, and insert after `AddI`
    7 Builer.CreateRet(AddI);

• 更换指令
要替换LLVM 中的指令，需要创建一个新的Instruction(作为替换指令)，并将所有SSA 定
义和用法从原来的Instruction 变更到替换的指令。
3 // Using `replaceAllUsesWith` to update users of `Mul`
4 Mul.replaceAllUsesWith(Shl);
5 Mul.eraseFromParent(); // remove the original

• 批量处理指令
换句话说，在迭代Instruction 实例时更改它们非常困难。 --> 缓存起来，一起更改


循环
简而言之，循环优化是生成更快、更高效程序的关键。这在高性能和科学计算社区中尤为重要。
在LLVM 中，循环由Loop 类表示。
LoopAnalysis 是一个LLVM 分析类，为我们提供了一个LoopInfo 实例，包含了一个Function
中的所有Loop 实例。我们可以遍历一个LoopInfo 实例，以获得一个单独的Loop 实例。

在这个讨论之后，还有其他一些属性可以让我们更容易地编写循环转换。如果循环实例有这些漂亮的属性，我们通常称它为规范循
环。LLVM 中的优化流水在将一个循环发送到任何循环转换之前，会尝试将其“更改”成这种规范形式。
目前，LLVM 有两种Loop 形式: 简化和旋转。

要获得一个简化的循环，可以在原始循环上运行LoopSimplfyPass
要获得一个旋转的循环，可以在原始循环上运行LoopRotationPass

有不同种类的LLVM Pass 在不同的IR 单元上工作——函数和模块的 Pass。这两种Pass 的运行方法有一个类似的函数签名——LLVM Pass 的主要入口点——如下所示:
1 PreservedAnalyses run(<IR unit class> &Unit,
2 <IR unit>AnalysisManager &AM);
所有run 方法都有两个参数—对IR 单元实例和AnalysisManager 实例的引用。
相反，循环Pass 有一个稍微复杂一点的run 方法签名，如下所示:
1 PreservedAnalyses run(Loop &LP, LoopAnalysisManager &LAM,
2 LoopStandardAnalysisResults &LAR,
3 LPMUpdater &U);
    • 第三个参数，LoopStandardAnalysisResults 提供了一些分析数据实例，例如：AAResults(别
    名分析数据)、DominatorTree 和LoopInfo。这些分析可以在许多循环优化中使用。然而，其
    中的大多数是由FunctionAnalysisManager 或ModuleAnalysisManager 管理。这意味着，
    开发人员需要实现更复杂的方法——使用OuterAnalysisManagerProxy 类——来检索它们。
    LoopStandardAnalysisResults 实例可以帮助我们提前检索这些分析数据。
    • 最后一个参数用于通知PassManager 新添加的循环，以便可以在以后处理这些新循环之前，
    将它们放入队列中。它还可以告诉PassManager 将当前循环再次放入队列中。

1 // Perfect loops
2 for(int i=…) {
3 for(int j=…){…}
4 }
5 // Non-perfect loops
6 for(int x=…) {
7 foo();
8 for(int y=…){…}
9 }
非完美循环的例子中，foo 调用点是上循环和下循环之间的间隙。
许多循环优化中，完美循环是更好的选择，例如：展开完美嵌套的循环更容易——理想情况
下，只需要复制最内层的循环体。

ScalarEvolution 是LLVM 中一个有趣且强大的框架。简单地说，它试图跟踪值如何变化——
通过算术运算——在程序路径上。将此放到循环优化的上下文中，可用于捕获循环中的递归值改变
行为，这与引导变量有很强的关系。