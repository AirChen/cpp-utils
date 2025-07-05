#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class UnionFind {
    public:
        UnionFind() {}
        void makeSet(const std::string& x) {
            parent[x] = x;
            rank[x] = 1;
        }

        std::string find(const std::string& x) {
            if (parent.count(x) == 0) {
                makeSet(x);
                return x;
            }

            std::string root = x;
            while (parent[root] != root) {
                root = parent[root];
            }
            return root;
        }

        void unionSet(const std::string& x, const std::string& y) {
            std::string rootX = find(x);
            std::string rootY = find(y);
            if (rootX == rootY) return;

            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootX] = rootY;
                if (rank[rootX] == rank[rootY]) {
                    rank[rootY]++;
                }
            }
        }

        bool isConnected(const std::string& x, const std::string& y) {
            return find(x) == find(y);
        }
    private:
        std::unordered_map<std::string, std::string> parent;
        std::unordered_map<std::string, int> rank;
}

bool func(const std::vector<std::vector<std::string>>& data) {
    UnionFind uf;
    for (const auto& layer : data) {
        for (auto iter = layer.begin(, niter = std::next(layer.begin())); niter != layer.end(); iter++, niter++) {
            if (uf.isConnected(*iter, *niter)) {
                return false;
            } else {
                uf.unionSet(*iter, *niter); 
            }
        }
    }
    return true;
}