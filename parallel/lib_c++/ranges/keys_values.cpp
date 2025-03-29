// rangesEntireContainer.cpp

#include <ranges>
#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> freqWord{ {"witch", 25}, {"wizard", 33},
                                                    {"tale", 45}, {"dog", 4},
                                                    {"cat", 34}, {"fish", 23} };
    std::cout << "Keys" << '\n';
    auto names = std::views::keys(freqWord); // (1)
    for (const auto& name : names){ std::cout << name << " "; }; std::cout << "\n";
    for (const auto& na : std::views::keys(freqWord)){ std::cout << na << " "; }; // (2)
    std::cout << "\n";
    
    auto firstw = [](const std::string& name) {
        return name[0] == 'w';
    };
    for (const auto& na : std::views::keys(freqWord) | std::views::filter(firstw)){ std::cout << na << " "; }; // (2)
    std::cout << "\n";

    // for (const auto& na : std::views::reverse(std::views::keys(freqWord))){ std::cout << na << " "; }; // (2)
    // std::cout << "\n";

    for (const auto& na : std::views::keys(freqWord) | std::views::take(4)){ std::cout << na << " "; }; // (2)
    std::cout << "\n";

    std::cout << "Values: " << '\n';
    auto values = std::views::values(freqWord); // (3)
    for (const auto& value : values){ std::cout << value << " "; }; std::cout << "\n";
    for (const auto& value : std::views::values(freqWord)){ // (4)
        std::cout << value << " ";
    }
    std::cout << "\n";

    return 0;
}