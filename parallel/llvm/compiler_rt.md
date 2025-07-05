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