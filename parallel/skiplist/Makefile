# 编译器配置
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -O2 -g
INCLUDES := -I.

# 源文件定义
SRCS := arena.cc skiplist.h random.h test_skiplist.cpp
OBJS := $(patsubst %.cc,%.o,$(filter %.cc,$(SRCS)))
TESTSRC := test_skiplist.cpp
TESTOBJ := $(patsubst %.cpp,%.o,$(TESTSRC))
TESTBIN := skiplist_test

# 静态库配置
LIBNAME := libskiplist.a
AR := ar
ARFLAGS := rcs

# 默认目标：编译测试程序
all: $(TESTBIN)

# 编译静态库
$(LIBNAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# 测试程序
$(TESTBIN): $(TESTOBJ) $(LIBNAME)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L. -lskiplist

# 通用编译规则
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TESTOBJ) $(LIBNAME) $(TESTBIN)

.PHONY: all clean