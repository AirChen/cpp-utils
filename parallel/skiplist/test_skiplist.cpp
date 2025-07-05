#include "skiplist.h"
#include "arena.h"
#include <iostream>

typedef uint64_t Key;
struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

int main() {
    leveldb::Arena arena;
    Comparator cmp;
    leveldb::SkipList<Key, Comparator> list(cmp, &arena);
    leveldb::SkipList<Key, Comparator>::Iterator iter(&list);
    iter.SeekToFirst();
    iter.Seek(100);
    iter.SeekToLast();

    std::cout << "hello!\n";

    return 0;
}