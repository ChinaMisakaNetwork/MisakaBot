#include "exprtk.hpp"
#include <mirai.h>
#include <vector>
#include "wstring.hpp"
#include <string>
typedef double T;
typedef exprtk::expression<T>   expression_t;
typedef exprtk::parser<T>       parser_t;
using namespace std;
using namespace Cyan;
class calc {
public:
	string handler(GroupMessage m) {
        vector<string>commands = stringSplit(m.MessageChain.GetPlainText(), ' ');
        if (commands.size() == 0)return "";
        if (commands.size() != 2)return "";
        if (*commands.begin() == "计算") {
            string ans = "答案是：";
            expression_t exp;
            parser_t par;
            if (!par.compile(commands[1], exp)) {
                cout << "Compile Error";
                return "出现错误，请查看终端以获取详细信息";
            }
            ans += to_string(exp.value());
            return ans;
        }
        return "";
	}
};