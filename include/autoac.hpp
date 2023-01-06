#pragma once
#include <string>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <map>

using namespace std;

constexpr int visible_number = 224;

//定义节点结构
struct state_node {
    bool finish{ false };
    int state{ 0 };
    int num{ 0 };
    string pattern{};
    //goto table
    vector<state_node*> transition_table{ vector<state_node*>(visible_number) };
};
//定义一个树的类
class trie_ac {
    state_node* start_node_;
    int state_count_;
    vector<state_node*> corresponding_node_;
    vector<state_node*> fail_;
public:
    trie_ac() : start_node_{ new state_node() }, state_count_{ 0 } {
        //state0 is start_node_
        corresponding_node_.push_back(start_node_);
    }
    ~trie_ac() {};
    //read all patterns and produce the goto table
    void load_pattern(const vector<string>& patterns);
    //produce fail function
    void dispose();
    //search matching
    void match(const string& str, set<int>& matched);
};

class tree_models {
public:
    trie_ac trieac;
    map<string, int> tagcollect;
    vector<string> patterns;
    void quick_find_tags(std::string& query) = delete;
    tree_models() = default;
    ~tree_models() {};
};

inline void trie_ac::load_pattern(const vector<string>& patterns) {
    int latest_state = 1;
    int count = 0;
    for (const auto& pattern : patterns) {
        auto* p = start_node_;
        for (int i = 0; i < pattern.size(); ++i) {
            state_node* next_node; 
            bool chineseoreng = false;
            if (pattern[i] > 32) {
                next_node = p->transition_table[pattern[i]]; 
                chineseoreng = true;
            }
            else next_node = p->transition_table[static_cast<std::vector<state_node*, std::allocator<state_node*>>::size_type>(0x20) - pattern[i]];//针对中文
            if (next_node == nullptr) {
                next_node = new state_node();
            }
            if (next_node->state == 0) {
                next_node->state = latest_state++;
                corresponding_node_.push_back(next_node);
            }
            if (!chineseoreng)p->transition_table[static_cast<std::vector<state_node*, std::allocator<state_node*>>::size_type>(0x20) - pattern[i]] = next_node;
            else p->transition_table[pattern[i]] = next_node;
            p = next_node;
        }
        p->finish = true;
        p->pattern = pattern;
        p->num = count++;
    }
    int aaa = 0;
    int bbb = 0;
    for (int i = 0; i < 224; ++i) {
        if (start_node_->transition_table[i] == nullptr) {
            start_node_->transition_table[i] = start_node_;
        }
    }
    state_count_ = latest_state;
}

//produce fail function
inline void trie_ac::dispose() {
    queue<state_node*> q;
    fail_ = std::move(vector<state_node*>(state_count_));

    for (const auto nxt : start_node_->transition_table) {
        if (nxt && nxt->state != 0) {
            fail_[nxt->state] = start_node_;
            q.push(nxt);
        }
    }
    while (!q.empty()) {
        const auto known = q.front();
        q.pop();
        for (int i = 0; i < 224; ++i) {
            // ReSharper disable once CppTooWideScopeInitStatement
            auto nxt = known->transition_table[i];
            if (nxt && nxt->state != 0) {
                auto p = fail_[known->state];
                while (!p->transition_table[i]) {
                    p = fail_[p->state];
                }
                fail_[nxt->state] = p->transition_table[i];
                q.push(nxt);
            }
        }
    }
}

//search matching
inline void trie_ac::match(const string& str, set<int>& matched) {
    int trans = 0;
    auto p = start_node_;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] > 0x20)trans = str[i];
        else trans = 0x20 - str[i];
        p = p->transition_table[trans] ? p->transition_table[trans] : (--i, fail_[p->state]);
        if (p && p->finish) {
            matched.insert(p->num);
        }
        else {
            if (p == nullptr) {
                p = start_node_;
            }
        }
    }
}

//cc:https://www.codeleading.com/article/96835705766/