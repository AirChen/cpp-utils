#include <iostream>
#include <string>

class MyString : public std::string {};
class MyStringV2 : public std::string {
    int age;
};
class MyStringV3 : public std::string {
public:
    int age;
};

class Count {
public:
    int Get() {return count++;}
    int count = 0;
};
class MyStringWithIndex : public std::string, public Count {
public:
    int index_ = 0;
};

// ----------------- compile error ? ------------------
class Base {
    int data_;
public:
    int Get() { return data_; }
protected:
    Base() : data_(11) {}
};
class Derived : public Base {
public:
};
// ------------------------------------------------------

struct Y {
    Y() = delete;
};
struct X {
private:
    X() = default;
};

struct YW {
    explicit YW() = default;
};
struct XW {
    XW();
};
XW::XW() = default;

int main() {
    std::cout << "is aggregate " << std::is_aggregate_v<std::string> << "\n";
    std::cout << "is aggregate " << std::is_aggregate_v<MyString> << "\n";
    std::cout << "is aggregate " << std::is_aggregate_v<MyStringV2> << "\n";
    std::cout << "is aggregate " << std::is_aggregate_v<MyStringV3> << "\n";

    MyStringV3 c{"hello world", 28};
    std::cout << "my string: " << c << " \n";

    MyStringWithIndex cs{"hello world", 7, 11};
    std::cout << "with index " << cs.c_str() << " index " << cs.index_ << " count " << cs.Get() << "\n";

    std::cout << "is aggregate " << std::is_aggregate_v<Derived> << "\n";
    std::cout << "is aggregate " << std::is_aggregate_v<Y> << "\n";
    Y y{}; // can't be -> Y y;
    std::cout << "is aggregate " << std::is_aggregate_v<X> << "\n";
    X x{}; // can't be -> X x;

    std::cout << "is aggregate " << std::is_aggregate_v<YW> << "\n";
    std::cout << "is aggregate " << std::is_aggregate_v<XW> << "\n";

    return 0;
}