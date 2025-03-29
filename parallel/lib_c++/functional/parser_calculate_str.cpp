#include <string>
#include <iostream>
#include <memory>
#include <deque>
#include <stack>

enum class OptionType {
    OptionTypeAdd,
    OptionTypeSub,
    OptionTypeMut,
    OptionTypeDiv,
    OptionTypeKuoHao
};

struct CalculateNode {
    std::shared_ptr<CalculateNode> left{nullptr};
    std::shared_ptr<CalculateNode> right{nullptr};
    std::shared_ptr<CalculateNode> mid{nullptr};
    OptionType type;

    int value;
    bool is_left_node{false};
};

int calculate_node_tree(const std::shared_ptr<CalculateNode>& node) {
    if (node == nullptr) {
        std::cout << "error, calculate node, the node is nullptr.\n";
        return 0;
    }

    if (node->is_left_node) return node->value;
    if (node->type == OptionType::OptionTypeKuoHao) return calculate_node_tree(node->mid);

    if (node->type == OptionType::OptionTypeAdd) {
        return calculate_node_tree(node->left) + calculate_node_tree(node->right);
    }
    if (node->type == OptionType::OptionTypeSub) {
        return calculate_node_tree(node->left) + calculate_node_tree(node->right);
    }
    if (node->type == OptionType::OptionTypeMut) {
        return calculate_node_tree(node->left) * calculate_node_tree(node->right);
    }
    if (node->type == OptionType::OptionTypeDiv) {
        int right_value = calculate_node_tree(node->right);
        if (right_value == 0) {
            std::cout << "error, calculate node, the div op right node is equal to zero.";
            return 0;
        }
        return calculate_node_tree(node->left) / right_value;
    }
    std::cout << "error, calculate node, not match type!\n";
    return 0;
}

void print_node_tree(const std::shared_ptr<CalculateNode>& node) {
    if (node == nullptr) {
        std::cout << "error, calculate node, the node is nullptr.\n";
        return;
    }
    if (node->is_left_node) {
        std::cout << " val: " << node->value;
        return;
    } 
    if (node->type == OptionType::OptionTypeKuoHao) 
        print_node_tree(node->mid);

    if (node->type == OptionType::OptionTypeAdd) {
        std::cout << "< ";
        print_node_tree(node->left);
        std::cout << " + ";
        print_node_tree(node->right);
        std::cout << ">"; 
    }
    if (node->type == OptionType::OptionTypeSub) {
        std::cout << "< ";
        print_node_tree(node->left);
        std::cout << " - ";
        print_node_tree(node->right);
        std::cout << ">"; 
    }
    if (node->type == OptionType::OptionTypeMut) {
        std::cout << "< ";
        print_node_tree(node->left);
        std::cout << " * ";
        print_node_tree(node->right);
        std::cout << ">"; 
    }
    if (node->type == OptionType::OptionTypeDiv) {
        std::cout << "< ";
        print_node_tree(node->left);
        std::cout << " / ";
        print_node_tree(node->right);
        std::cout << ">"; 
    }
}

std::shared_ptr<CalculateNode> create_calculate_tree(const std::string& que) {
    std::deque<std::shared_ptr<CalculateNode>> node_deque;
    for (auto iter = que.begin(); iter != que.end(); iter++) {
        char ch = *iter;
        if (ch > '0' && ch <= '9') {
            int cur_val = ch - '0';
            std::shared_ptr<CalculateNode> node = std::make_shared<CalculateNode>();
            node->is_left_node = true;
            node->value = cur_val;
            node_deque.push_back(node);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            std::shared_ptr<CalculateNode> node = std::make_shared<CalculateNode>();
            switch (ch)
            {
            case '+':
                node->type = OptionType::OptionTypeAdd;
                break;
            case '-':
                node->type = OptionType::OptionTypeSub;
                break;
            case '*':
                node->type = OptionType::OptionTypeMut;
                break;
            case '/':
                node->type = OptionType::OptionTypeDiv;
                break;
            default:
                break;
            }

            node_deque.push_back(node); 
        } else if (ch == '(' || ch == ')') {
            std::shared_ptr<CalculateNode> node = std::make_shared<CalculateNode>();
            node->type = OptionType::OptionTypeKuoHao;
            node_deque.push_back(node);
        }
    }
    // std::shared_ptr<CalculateNode> root_node{nullptr};
    // std::shared_ptr<CalculateNode> pre_node{nullptr};
    std::stack<std::shared_ptr<CalculateNode>> node_stack;
    while (!node_deque.empty()) {
        std::shared_ptr<CalculateNode> node = node_deque.front();
        node_deque.pop_front();
        if (node->is_left_node) {
            if (node_stack.empty()) {
                node_stack.push(node);
            } else {
                std::shared_ptr<CalculateNode> pre_node = node_stack.top();
                if (pre_node->type != OptionType::OptionTypeKuoHao && !pre_node->is_left_node) {
                    auto keep_right = pre_node;
                    while (keep_right->right != nullptr) {
                        keep_right = keep_right->right;
                    }
                    keep_right->right = node;
                }
            }
        } else if (node->type == OptionType::OptionTypeAdd ||
                    node->type == OptionType::OptionTypeSub) {
            if (node_stack.empty()) {
                std::cout << "error, create node tree.";
                break;
            }
            std::shared_ptr<CalculateNode> pre_node = node_stack.top();
            if (pre_node->is_left_node) {
                node->left = pre_node;
                node_stack.pop();
            } else if (pre_node->type == OptionType::OptionTypeMut || pre_node->type == OptionType::OptionTypeDiv) {
                node->left = pre_node;
                node_stack.pop();
            } else if (pre_node->type == OptionType::OptionTypeAdd || pre_node->type == OptionType::OptionTypeSub) {
                node->left = pre_node;
                node_stack.pop();
            }
            node_stack.push(node);
        } else if (node->type == OptionType::OptionTypeMut ||
                    node->type == OptionType::OptionTypeDiv) {
            if (node_stack.empty()) {
                std::cout << "error, create node tree.";
                break;
            }
            std::shared_ptr<CalculateNode> pre_node = node_stack.top();
            if (pre_node->is_left_node) {
                node->left = pre_node;
                node_stack.pop();
                node_stack.push(node);
            } else if (pre_node->type == OptionType::OptionTypeMut || pre_node->type == OptionType::OptionTypeDiv) {
                node->left = pre_node;
                node_stack.pop();
                node_stack.push(node);
            } else if (pre_node->type == OptionType::OptionTypeAdd || pre_node->type == OptionType::OptionTypeSub) {
                if (pre_node->right != nullptr) {
                    std::cout << " pr is left " << pre_node->right->is_left_node << " type " << (int)pre_node->right->type << "\n";
                    node->left = pre_node->right;
                }
                pre_node->right = node;
            }
        }
    }
    std::cout << "node stack size: " << node_stack.size() << " \n";
    return node_stack.top();
}

static int calculate_str_equal(const std::string& que) {
    std::shared_ptr<CalculateNode> node_ptr = create_calculate_tree(que);
    print_node_tree(node_ptr), std::cout << "\n";
    return calculate_node_tree(node_ptr);
}

static int calculate_str_equal_bak(const std::string& que) {
    std::stack<int> tmp_task;
    char pre_op = 0;
    for (auto iter = que.begin(); iter != que.end(); iter++) {
        char ch = *iter;
        if (ch > '0' && ch < '9') {
            int cur_val = ch - '0';
            if (tmp_task.empty()) {
                tmp_task.push(cur_val);
            } else {
                int pre_val = tmp_task.top();
                tmp_task.pop();

                int ans = 0;
                if (pre_op == '+') {
                    ans = pre_val + cur_val;
                } else if (pre_op == '-') {
                    ans = pre_val - cur_val;
                } else if (pre_op == '*') {
                    ans = pre_val * cur_val;
                } else if (pre_op == '/') {
                    if (cur_val == 0) {
                        std::cout << "the bcs can't be zero.\n";
                    } else {
                        ans = pre_val / cur_val;
                    }
                }
                tmp_task.push(ans);
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            pre_op = ch;
        }
    }
    return tmp_task.top();
}

int main() {
    auto test_block = [](const std::string& que_str, int ans) {
        int test_ans = calculate_str_equal(que_str);
        std::cout << "test " << que_str << " == " << std::to_string(test_ans) << " ";
        if (ans == test_ans) {
            std::cout << "True\n"; 
        } else {
            std::cout << "False\n";
        }
    };
    test_block("2+3+4+5", 14);
    test_block("2*3+4*5", 26);
    test_block("2*3+4/2*5", 16);
    test_block("3*2*3+4/2*5", 28);
    test_block("3*2*3+4/2*5+9", 37);
    return 0;
}