这是compiler-rt项目的一部分。消毒器可以在clang中启用，要构建编译器-rt项
目，可以简单地在构建LLVM时将CMake变量-DLLVM_ENABLE_RUNTIMES=compiler-rt添加到
CMake 配置步骤中。

地址消毒器会替换对malloc()和free()函数的调用，并使用检查保护来检测所有内存
访问。这会给应用程序增加很多开销，而且只会在应用程序的测试阶段使用地址消毒器。
以在llvm/lib/Transforms/Instrumentation/AddressSanitzer.cpp 中为通道的实现源码

找到内存问题
 clang -fsanitize=address -g useafterfree.c -o useafterfree

找到未初始化的值
 clang -fsanitize=memory -g memory.c -o memory

线程消毒器可以检测基于pthread的应用程序和使用LLVMlib++实现的应用
程序中的数据竞争，可以在llvm/lib/Transforms/Instrumentation/ThreadSanitizer.cpp 文件中找到实现。
 clang -fsanitize=thread -g thread.c -o thread -lpthread

模糊测试
要测试应用程序，需要编写单元测试，这是确保软件正常运行的好方法。然而，由于可能输入
的指数数量，可能会错过某些奇怪的输入，以及一些错误。
模糊测试在这方面可以提供帮助。其思想是为应用程序提供随机生成的数据，或者基于有效输
入但随机更改的数据。这是重复进行的，因此应用程序将使用大量输入进行测试，这就是为什么模
糊测试是一种强大的测试方法。
LLVM自带了自己的模糊测试库。libFuzzer最初是LLVM核心库的一部分，最终移
到了compiler-rt 中，所以该库设计用于测试小而快速的函数。

clang -fsanitize=fuzzer,address -g fuzzer.c -o fuzzer

若应用程序似乎运行缓慢，可能想知道代码中的时间花在了哪里，可以用XRay检测代码可以
帮助完成这项任务。每个函数进入和退出时，都会向运行时库插入一个特殊的调用。允许计算函数
调用的频率，以及在函数中花费的时间。可以在llvm/lib/XRay/目录中找到检测通道的实现，其运行
时是compiler-rt 的一部分

clang -fxray-instrument -fxray-instruction-threshold=1 -g xraydemo.c -o xraydemo

以下是关于 LLVM 项目中 **compiler-rt** 的详细资料和资源整理，涵盖官方文档、代码结构、核心功能及实践指南：

---

### **1. 官方文档与权威资源**
#### **LLVM 官方文档**
- **compiler-rt 主页**  
  [LLVM compiler-rt Documentation](https://compiler-rt.llvm.org/)  
  - 介绍 compiler-rt 的作用、组件和构建方法。
- **源码结构说明**  
  [Source Code Overview](https://compiler-rt.llvm.org/docs/HowToBuild.html)  
  - 各目录功能解析（如 `lib/builtins`, `lib/sanitizer_common`）。

#### **LLVM 开发者手册**  
  [LLVM Developer Policy](https://llvm.org/docs/DeveloperPolicy.html#compiler-rt)  
  - 贡献代码的规范（如代码风格、测试要求）。

---

### **2. 核心模块与功能**
#### **2.1 内置函数（Builtins）**
- **功能**：提供处理器不直接支持的底层操作（如 128 位整数运算）。  
- **代码路径**：`lib/builtins/`  
  - 示例：`__muldi3`（64 位乘法）、`__udivti3`（128 位除法）。  
- **文档**：  
  [Builtins Implementation Notes](https://compiler-rt.llvm.org/docs/Builtins.html)

#### **2.2 Sanitizers（动态检测工具）**
- **AddressSanitizer (ASan)**  
  - 检测内存错误（越界、释放后使用）。  
  - 代码路径：`lib/asan/`  
- **ThreadSanitizer (TSan)**  
  - 检测数据竞争。  
  - 代码路径：`lib/tsan/`  
- **文档**：  
  [Sanitizers Wiki](https://github.com/google/sanitizers/wiki)

#### **2.3 Profile 工具**
- **代码覆盖率（Coverage）**  
  - `lib/profile/` 提供 `__llvm_profile_*` 函数。  
- **文档**：  
  [Source-based Code Coverage](https://clang.llvm.org/docs/SourceBasedCodeCoverage.html)

#### **2.4 其他模块**
- **XRay**：动态插桩工具（`lib/xray/`）。  
- **Control Flow Integrity (CFI)**：防止控制流劫持（`lib/cfi/`）。

---

### **3. 代码结构与关键文件**
```bash
compiler-rt/
├── lib/                     # 核心实现
│   ├── builtins/            # 内置函数（如整数运算）
│   ├── sanitizer_common/    # Sanitizer 共享代码
│   ├── asan/                # AddressSanitizer
│   ├── tsan/                # ThreadSanitizer
│   └── profile/             # 性能分析工具
├── include/                 # 公共头文件
├── cmake/                   # CMake 配置
└── test/                    # 测试用例
```

---

### **4. 实践指南**
#### **4.1 添加新的内置函数**
1. **在 `lib/builtins/` 下实现函数**（如 `__custom_add.c`）：
   ```c
   int __custom_add(int a, int b) { return a + b; }
   ```
2. **注册到 CMake**：  
   修改 `lib/builtins/CMakeLists.txt`：
   ```cmake
   add_builtin_library(compiler-rt-custom
     SOURCES __custom_add.c
   )
   ```

#### **4.2 集成 Sanitizer**
- **编译时启用 ASan**：
  ```bash
  clang -fsanitize=address -o program source.c
  ```
- **自定义 Sanitizer**：  
  参考 `lib/sanitizer_common/` 实现运行时逻辑。

#### **4.3 调试与测试**
- **运行单测**：
  ```bash
  ninja check-compiler-rt
  ```
- **查看生成的库**：
  ```bash
  ls build/lib/clang/15/lib/darwin/  # macOS 示例
  ```

---

### **5. 高级主题**
#### **5.1 交叉编译支持**
- **指定目标架构**：
  ```bash
  cmake -DCMAKE_OSX_ARCHITECTURES="arm64" ..
  ```
- **多平台适配**：  
  参考 `lib/builtins` 中的 `CMakeLists.txt` 对不同架构的条件编译。

#### **5.2 优化性能**
- **内联汇编**：在 `builtins` 中使用汇编优化关键路径。  
- **减少运行时开销**：Sanitizer 的 Shadow Memory 配置。

---

### **6. 社区资源**
- **邮件列表**：  
  [llvm-dev](https://lists.llvm.org/cgi-bin/mailman/listinfo/llvm-dev)（讨论开发问题）。  
- **代码审查**：  
  [Phabricator](https://reviews.llvm.org/)（提交 Patch）。  
- **Slack/Discord**：  
  LLVM 社区的 `#compiler-rt` 频道。

---

### **7. 书籍推荐**
- *《LLVM Cookbook》*：第 5 章详解 compiler-rt。  
- *《Advanced C and C++ Compiling》*：链接与运行时库实践。

---

通过以上资料，你可以深入理解 compiler-rt 的设计原理，并快速上手开发或调试。如需进一步分析具体问题，请提供：  
1. 你关注的模块（如 Sanitizer/Builtins）。  
2. 遇到的错误日志或代码片段。