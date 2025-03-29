#include <iostream>

constexpr int calc_len(int v) {
    // std::cout << "in compiler time.\n";
    // return v * v;
    if (v % 2 == 1) {
        return v;
    }
    return v + 1;
}

// template<class T> bool is_same_value(T a, T b)
// {
//   return a == b;
// }

// template<> bool is_same_value<double>(double a, double b)
// {
//   if (std::abs(a - b) < 0.0001) {
//        return true;
//   }
//   else {
//        return false;
//   }
// }

#include <type_traits>
template<class T> bool is_same_value(T a, T b)
{
    if constexpr (std::is_same<T, double>::value) {
        if (std::abs(a - b) < 0.0001) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

template<class T> auto any2i(T t) {
    if constexpr (std::is_same<T, std::string>::value && T::npos == -1) {
        return atoi(t.c_str());
    } else {
        return t;
    }
}

template<class T> auto minus(T a, T b)
{
  if constexpr (std::is_same<T, double>::value) {
       if (std::abs(a - b) < 0.0001) {
            return 0.;
       }
       else {
            return a - b;
       }
  }
//   else {
       return static_cast<int>(a - b);
//   }
}

int main() {
    int fes[calc_len(3)] = { 0 };
    constexpr int iv = calc_len(3);

    for (int i = 0; i < iv; i++) {
        fes[i] = i + 1;
    }

    double x = 0.1 + 0.1 + 0.1 - 0.3;
    std::cout << std::boolalpha;
    std::cout << "is_same_value(5, 5)  : " << is_same_value(5, 5) << std::endl;
    std::cout << "x == 0.0               : " << (x == 0.) << std::endl;
    std::cout << "is_same_value(x, 0.) : " << is_same_value(x, 0.) << std::endl;

    std::cout << any2i(std::string("6")) << std::endl;
    // std::cout << any2i(6) << std::endl;

    // std::cout << minus(5.6, 5.11) << std::endl;
    // std::cout << minus(5.60002, 5.600011) << std::endl;
    std::cout << minus(6, 5) << std::endl;
    return 0;
}