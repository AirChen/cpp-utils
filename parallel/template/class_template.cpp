#include <vector>
#include <cassert>

template<typename T>
class Stack {
private:
    std::vector<T> elems;
public:
    void push(T const& elem);
    void pop();
    T const& top() const;
    bool empty() const {
        return elems.empty();
    }
};

template<typename T>
void Stack<T>::push(T const& elem) {
    elems.emplace_back(elem);
}

template<typename T>
void Stack<T>::pop() {
    assert(!elems.empty());
    elems.pop_back();
}

template<typename T>
T const& Stack<T>::top() const {
    assert(!elems.empty());
    return elems.back();
}

template<typename T, typename Con = std::vector<T>>
class StackContain {
private:
    Con elems;
public:
    void push(T const& elem);
    void pop();
    T const& top() const;
    bool empty() const {
        return elems.empty();
    }
};

template<typename T, typename Con>
void StackContain<T, Con>::push(T const& elem) {
    elems.emplace_back(elem);
}

template<typename T, typename Con>
void StackContain<T, Con>::pop() {
    assert(!elems.empty());
    elems.pop_back();
}

template<typename T, typename Con>
T const& StackContain<T, Con>::top() const {
    assert(!elems.empty());
    return elems.back();
}

using IntStack = Stack<int>;
using BoolStack = Stack<bool>;
using DoubleStack = Stack<double>;

typedef StackContain<int> IntStackContainer;
typedef StackContain<bool> BoolStackContainer;
typedef StackContain<double> DoubleStackContainer;

int main() {

    return 0;
}