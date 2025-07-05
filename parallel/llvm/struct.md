Cmake 指令表

添加新的库
add_llvm_component_library

1. 要将常规构建目标依赖分配给库目标(相当于add_dependencies)，可以使用DEPENDS 参数
2. 如果想直接链接到一个普通的库(非LLVM 组件)，可以使用LINK_LIBS 参数

add_llvm_component_library(LLVMFancyOpt
    FancyOpt.cpp
    LINK_LIBS
    ${BOOST_LIBRARY})

为文件夹添加一个构建目标
add_subdirectory(AggressiveFancyOpt)

添加可执行文件或者工具
add_llvm_executable 或 add_llvm_tool
只能使用LLVM_LINK_COMPONENTS 来指定链接组件
set(LLVM_LINK_COMPONENTS
  AllTargetsAsmParsers
  AllTargetsCodeGens
  AllTargetsDescs
  AllTargetsInfos
  Analysis)


添加Pass插件
add_llvm_pass_plugin(MyPass
   HelloWorldPass.cpp)

有两种方式可以用来配置其他项目链接到LLVM 源码:
• 使用llvm-config 工具
• 使用LLVM 的CMake 模块  cmake -DLLVM_DIR=/opt/homebrew/opt/llvm@18 ..