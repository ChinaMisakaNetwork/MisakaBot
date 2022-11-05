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

#define VISIBLE_NUMBER 224

//定义节点结构
struct StateNode {
    bool finish_{ false };
    int state_{ 0 };
    int num_{ 0 };
    string pattern_{};
    //goto table
    vector<StateNode*> transition_table_{ vector<StateNode*>(VISIBLE_NUMBER) };
};
//定义一个树的类
class TrieAc {
private:
    StateNode* start_node_;
    int state_count_;
    vector<StateNode*> corresponding_node_;
    vector<StateNode*> fail_;
public:
    TrieAc() : start_node_{ new StateNode() }, state_count_{ 0 } {
        //state0 is start_node_
        corresponding_node_.push_back(start_node_);
    }
    ~TrieAc() {};
    //read all patterns and produce the goto table
    void load_pattern(const vector<string>& _Patterns);
    //produce fail function
    void dispose();
    //search matching
    void match(const string& _Str, set<int>& matched);
};

class TreeModels {
public:
    TrieAc trieac;
    map<string, int> tagcollect;
    vector<string> patterns;
    void quick_find_tags(std::string& query);
    TreeModels();
    ~TreeModels() {};
};
void TrieAc::load_pattern(const vector<string>& _Patterns) {
    int latest_state = 1;
    int count = 0;
    for (const auto& pattern : _Patterns) {
        auto* p = start_node_;
        for (int i = 0; i < pattern.size(); ++i) {
            StateNode* next_node; 
            bool chineseoreng = 0;
            if (pattern[i] > 32) {
                next_node = p->transition_table_[pattern[i]]; 
                chineseoreng = 1;
            }
            else next_node = p->transition_table_[0x20 - pattern[i]];//针对中文
            if (next_node == nullptr) {
                next_node = new StateNode();
            }
            if (next_node->state_ == 0) {
                next_node->state_ = latest_state++;
                corresponding_node_.push_back(next_node);
            }
            if (!chineseoreng)p->transition_table_[0x20 - pattern[i]] = next_node;
            else p->transition_table_[pattern[i]] = next_node;
            p = next_node;
        }
        p->finish_ = true;
        p->pattern_ = pattern;
        p->num_ = count++;
    }
    int aaa = 0;
    int bbb = 0;
    for (int i = 0; i < 224; ++i) {
        if (start_node_->transition_table_[i] == nullptr) {
            start_node_->transition_table_[i] = start_node_;
        }
    }
    state_count_ = latest_state;
}

//produce fail function
void TrieAc::dispose() {
    queue<StateNode*> q;
    fail_ = std::move(vector<StateNode*>(state_count_));

    for (const auto nxt : start_node_->transition_table_) {
        if (nxt && nxt->state_ != 0) {
            fail_[nxt->state_] = start_node_;
            q.push(nxt);
        }
    }
    while (!q.empty()) {
        auto known = q.front();
        q.pop();
        for (int i = 0; i < 224; ++i) {
            auto nxt = known->transition_table_[i];
            if (nxt && nxt->state_ != 0) {
                auto p = fail_[known->state_];
                while (!p->transition_table_[i]) {
                    p = fail_[p->state_];
                }
                fail_[nxt->state_] = p->transition_table_[i];
                q.push(nxt);
            }
        }
    }
}

//search matching
void TrieAc::match(const string& _Str, set<int>& matched) {
    int trans = 0;
    auto p = start_node_;
    for (int i = 0; i < _Str.size(); ++i) {
        if (_Str[i] > 0x20)trans = _Str[i];
        else trans = 0x20 - _Str[i];
        p = p->transition_table_[trans] ? p->transition_table_[trans] : (--i, fail_[p->state_]);
        if (p && p->finish_) {
            matched.insert(p->num_);
        }
        else {
            if (p == nullptr) {
                p = start_node_;
            }
        }
    }
}

//cc:https://www.codeleading.com/article/96835705766/