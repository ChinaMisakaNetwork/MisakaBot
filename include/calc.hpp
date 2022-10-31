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
	string handler(GroupMessage m) {
        vector<string>commands;
        stringstream sin(m.MessageChain.GetPlainText());
        string temp;
        while (getline(sin, temp, ' ')) {
            commands.push_back(temp);
        }
        if (commands.size() == 0)return "";
        if (commands.size() != 2)return "";
        if (*commands.begin() == "计算") {
            string ans = "答案是：";
            expression_t exp;
            parser_t par;
            if (!par.compile(commands[1], exp)) {
                return "格式错误";
            }
            ans += to_string(exp.value());
            return ans;
        }
        return "";
	}
};