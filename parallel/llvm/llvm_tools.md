诊断工具
代码部分
LLVM_DEBUG  -> LLVM_DEBUG(dbgs() << "Found a multiplication with operands ");
DEBUG_WITH_TYPE
LLVM_TYPE

命令行
$ opt -O3 -debug
$ opt -O3 -debug-only=sroa,simple-mul-opt

收集统计信息
• 使用Statistic 类
$ opt -stats –load-pass-plugin=…
$ opt -stats -stats-json –load-pass-plugin=…
$ opt -stats -stats-json -info-output-file=my_stats.json …

• 使用优化注释
LLVM 提供了一个很好的工具，称为优化注释，用于收集和报告在优化过程中发生的这种情况(或任何类型的信息)。
OptimizationRemarkEmitter

• 添加耗时测量
Timer 类和TimeTraceScope类


异常处理
虽然C++ 内置了对异常处理的支持，但LLVM 在其代码库中根本没有使用。这一决定背后
的基本原因是，尽管方便表达语法，但C++ 中的异常处理在性能方面需要付出了很高的代价。简
单地说，异常处理使原始代码更加复杂，并妨碍编译器对其进行优化。此外，在运行时，程序通常
需要花费更多的时间从异常中恢复。因此，LLVM 在其代码库中默认禁用异常处理，转而使用其他
错误处理方法

Error
ErrorOr