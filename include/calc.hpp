#include "calculator.hpp"
#include <mirai.h>
#include <vector>
#include "wstring.hpp"
#include <string>
using namespace std;
using namespace Cyan;
class calc {
public:
	string handler(GroupMessage m) {
        vector<string>commands = stringSplit(m.MessageChain.GetPlainText(), ' ');
        if (commands.size() == 0)return "";
        if (*commands.begin() == "计算") {
            string ans = "答案是：";
            ans += to_string(EvaluateExpression((commands[1])));
            return ans;
        }
        return "";
	}
};