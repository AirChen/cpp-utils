#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <list>

using namespace std;
bool isVowel(char c){
    string myVowels{"aeiouäöü"};
    set<char> vowels(myVowels.begin(), myVowels.end());
    return (vowels.find(c) != vowels.end());
}

int main() {
    list<char> myCha{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    int cha[]= {'A', 'B', 'C'};
    cout << *find(myCha.begin(), myCha.end(), 'g'); // g
    cout << *find_if(myCha.begin(), myCha.end(), isVowel); // a
    cout << *find_if_not(myCha.begin(), myCha.end(), isVowel); // b
    
    auto iter= find_first_of(myCha.begin(), myCha.end(), cha, cha + 3);
    if (iter == myCha.end()) cout << "None of A, B or C."; // None of A, B or C.
    
    auto iter2= find_first_of(myCha.begin(), myCha.end(), cha, cha+3, [](char a, char b){ return toupper(a) == toupper(b); });
    if (iter2 != myCha.end()) cout << *iter2; // a
    
    auto iter3= adjacent_find(myCha.begin(), myCha.end());
    if (iter3 == myCha.end()) cout << "No same adjacent chars.";
    // No same adjacent chars.
    
    auto iter4= adjacent_find(myCha.begin(), myCha.end(),
    [](char a, char b){ return isVowel(a) == isVowel(b); });
    if (iter4 != myCha.end()) cout << *iter4; // b  -- 都不是元音
    return 0;
}