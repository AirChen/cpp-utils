#include <type_traits>
template <typename T>T fac(T a){
    static_assert(std::is_integral<T>::value, "T not integral");
    return a;
}

int main() {
    fac(10);
    fac(10.1); // with T= double; T not integral
    return 0;
}