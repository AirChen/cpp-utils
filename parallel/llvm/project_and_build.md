第一步
cmake -G "Ninja" ../llvm
ninja all
ninja -j8 all

Ninja 在大型代码库(如LLVM) 上的运行速度明显快于GNU Make

第二步
避免使用BFD 连接器
使用 gold 或者 lld 连接器

cmake -G "Ninja" -DLLVM_USE_LINKER=gold ../llvm
cmake -G "Ninja" -DLLVM_USE_LINKER=lld ../llvm

限制用于链接的并行线程的数量是减少(峰值) 内存消耗的另一种方法。你可以在cmake 过程中，通过指定LLVM_PARALLEL_LINK_JOBS=<N> 变量来实现，其中N 是期望的工作线程数。

第三步
RelWithDebInfo: 这种构建类型尽可能多地应用编译器优化(通常是-O2)，并保留所有调试信息。这是一个在空间消耗、运行速度和可调试性之间进行平衡的选项。
相比于 Release / Debug

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo …

除了配置构建类型，LLVM_ENABLE_ASSERTIONS 是另一个CMake(布尔型) 参数，它控制断言(也就是，assert(bool predicate) 函数，如果predicate 参数不为真，将终止程序) 是否启用。
assert 默认只在debug模型下打开

第四步
选择构建的目标
cmake -DLLVM_TARGETS_TO_BUILD="X86;AArch64;AMDGPU" …
注意用双引号包起来

第五步 构建动态库
cmake -DBUILD_SHARED_LIBS=ON …


第六步 拆除调试信息
cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_USE_SPLIT_DWARF=ON …

第七步 构建优化版的llvm-tblgen
TableGen 是一种领域特定语言(DSL)，用于描述结构化数据，这些数据将转换成相应的C/C++ 代码，作为LLVM 构建过程的一部分(将在后面的章节中了解更多)，转换工具为llvm-tblgen。

cmake -DLLVM_OPTIMIZED_TABLEGEN=ON -DCMAKE_BUILD_TYPE=Debug …
这样其他工具都采用debug编译类型，只有llvm-tblgen 采用优化版本

第八步 

PassManager 将优化和代码生成所需的所有任务(即Pass) 放在一起。

env CC=`which clang` CXX=`which clang++` \
cmake -DLLVM_USE_NEWPM=ON …

其他
使用 gn 构建项目

env CC=`which clang` CXX=`which clang++` \
cmake -G "Ninja" -DLLVM_USE_LINKER=lld -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_ASSERTIONS=ON -DBUILD_SHARED_LIBS=ON -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_USE_SPLIT_DWARF=ON -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_USE_NEWPM=ON

cmake -DLLVM_USE_LINKER=lld -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DBUILD_SHARED_LIBS=ON -DLLVM_USE_SPLIT_DWARF=ON -DLLVM_OPTIMIZED_TABLEGEN=ON  -DLLVM_USE_NEWPM=ON ..

苹果电脑指定一下
-DLLVM_DEFAULT_TARGET_TRIPLE=arm64-apple-darwin

中途改配置可以用 ccmake . 进行配置更新
改完后，用 ninja 重新构建

AArch64

安装
指定安装目录 -DCMAKE_INSTALL_PREFIX=/path/to/install
执行 ninja install

配置环境变量
# 临时生效（当前终端）
export PATH="/path/to/install/bin:$PATH"
export LD_LIBRARY_PATH="/path/to/install/lib:$LD_LIBRARY_PATH"  # Linux
export DYLD_LIBRARY_PATH="/path/to/install/lib:$DYLD_LIBRARY_PATH"  # macOS

# 永久生效（写入 shell 配置文件）
echo 'export PATH="/path/to/install/bin:$PATH"' >> ~/.bashrc  # 或 ~/.zshrc
echo 'export LD_LIBRARY_PATH="/path/to/install/lib:$LD_LIBRARY_PATH"' >> ~/.bashrc
source ~/.bashrc