IRModule 涵盖了所有机器学习模型，包括计算图、张量程序以及对外部库的潜在调用。
是 Apache TVM Unity 的核心抽象。

IRModules 可以通过多种方式初始化。下面我们将演示其中几种。
1. 从现有模型导入 
    relax.frontend.detach_params
2. 使用 Relax NN 模块编写
    from tvm.relax.frontend import nn
3. 通过 TVMScript 创建
    from tvm.script import ir as I
    from tvm.script import relax as R

IRModule 的属性
IRModule 是函数的集合，由 GlobalVars 索引。
    mod = mod_from_torch
    print(mod.get_global_vars())
    # index by global var name
    print(mod["main"])
    # index by global var, and checking they are the same function
    (gv,) = mod.get_global_vars()
    assert mod[gv] == mod["main"]

IRModules 上的转换
转换是 Apache TVM Unity 的核心组件。一个转换接收一个 IRModule，并输出另一个 IRModule。我们可以将一系列转换应用于一个 IRModule 以获得一个新的 IRModule。这是优化模型的常用方法。
    mod = mod_from_torch
    mod = relax.transform.LegalizeOps()(mod)
    mod.show()

查找转化：https://tvm.apache.org/docs/reference/api/python/relax/transform.html#api-relax-transformation
默认的转化：
LegalizeOps ：此转换将 Relax 运算符转换为 call_tir 函数，并带有相应的 TensorIR 函数。经过此转换后，IRModule 将同时包含 Relax 函数和 TensorIR 函数。
AnnotateTIROpPattern ：此转换注释了 TensorIR 函数的模式，为后续的运算符融合做好准备。
FoldConstant ：此过程执行常量折叠，优化涉及常量的操作。
FuseOps 和 FuseTIR ：这两个过程协同工作，根据上一步（AnnotateTIROpPattern）中注释的模式融合算子。这两个过程同时转换 Relax 函数和 TensorIR 函数。

通用部署 IRModule
Apache TVM Unity 提供了通用部署的能力，这意味着我们可以将相同的 IRModule 部署在不同的后端上，包括 CPU、GPU 以及其他新兴的后端。
GPU:
    我们使用 DLight 来生成 GPU 程序。
    在 GPU 上编译 IRModule，方式与在 CPU 上类似
