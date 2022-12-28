#pragma once
#include "exprtk.hpp"
#include <mirai.h>
#include <vector>
#include <string>
typedef double T;
typedef exprtk::expression<T>   expression_t;
typedef exprtk::parser<T>       parser_t;
using namespace std;
using namespace Cyan;
class calc {
public:
	MessageChain handler(GroupMessage m) const {
        vector<string>commands;
        MessageChain msg;
        stringstream sin(m.MessageChain.GetPlainText());
        string temp;
        while (getline(sin, temp, ' ')) {
            if (temp.empty())continue;
            commands.push_back(temp);
        }
        if (commands.size() == 0)return msg;
        if (commands.size() != 2)return msg;
        if (*commands.begin() == "计算") {
            string ans = "答案是：";
            expression_t exp;
            parser_t par;
            if (!par.compile(commands[1], exp)) {
				msg.Add<PlainMessage>("表达式错误");
            }
            ans += to_string(exp.value());
            msg.Add<PlainMessage>(ans);
        }
        return msg;
	}
};