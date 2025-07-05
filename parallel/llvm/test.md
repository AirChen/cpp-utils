LIT 是一个测试基础设施，最初是为运行LLVM 的回归测试而开发的。现在，它不仅是在 LLVM 中运行所有测试(包括单元测试和回归测试) 的工具，而且还是一个可以在LLVM 之外使用的通用测试框架。它还提供了广泛的测试格式来处理不同的场景。本章将全面介绍这个框架中的组件，并帮助您掌握LIT 的使用方式。

安装：
https://llvm.org/docs/TestSuiteGuide.html
比如本地有 llvm-15.x 代码仓库， 可以使用以下命令安装：（llvm19 独立出去了）
pip3 install ~/ws/githubs/llvm-project-15.x/llvm/utils/lit


FileCheck 是LLVM 的高级模式检查器，与Unix/Linux 系统中的grep 类似，使用基于行的上下文，从而提供了更强大而简单的语法。此外，可以将FileCheck 指令放在测试目标旁，可以让测试用例自包含，使测试更容易理解。
虽然基本的FileCheck 语法很容易上手，但FileCheck 还有许多其他功能，它们才能真正展示FileCheck 的强大功能，并极大地改善了开发者的测试体验——例如创建更简洁的测试脚本和解析更复杂的程序输出