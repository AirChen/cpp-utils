Apache TVM 的一个主要设计目标是，能够轻松地定制优化流水线，以用于研究或开发目的，并迭代工程优化。

所有围绕 IRModule 优化的工作都可以与现有流水线组合。需要注意的是，每个优化都可以专注于计算图的某个部分 ，从而实现部分降低或部分优化。