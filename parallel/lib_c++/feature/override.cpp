#include <iostream>

class Base {
public:
    virtual void foo(int) {
        std::cout << "Base::foo(int)" << std::endl;
    }
    void bar() {
        std::cout << "Base::bar()" << std::endl;
    }
};

class Derived : public Base {
public:
    void foo(double) {
        std::cout << "Derived::foo(double)" << std::endl;
    }          // 隐藏 Base::foo(int)，非重写！
    
    using Base::foo;  // 显示地声明 Base::foo(int)
    using Base::bar;  // 显示地声明 Base::bar()
    void bar() {
        std::cout << "Derived::bar()" << std::endl;
    }         // 隐藏 Base::bar()
};

int main() {
    Derived d;
    d.foo(1);
    d.bar();
    return 0;
}