 • 使用AST生成IR
    https://llvm.org/docs/LangRef.html#data-layout

 • 使用AST编号生成SSA格式的IR代码
 • 设置模块和驱动程序

  • 处理数组、结构体和指针
   数组 [10 x i64]
   结构体 { float, i64, float }
      内存对齐
       C++ 表示 struct __attribute__((__packed__)) { float x; long long color; float y; } 
       IR 表示 <{ float, i64, float }>
  • 正确获取应用程序的二进制接口(ABI)
  • 为类和虚函数创建IR代码