#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <mirai.h>
#include <mysql++.h>
#include <vector>
#include <fstream>
using namespace std;
using namespace Cyan;
#include <cpr/cpr.h>
//TODO: Rewrite Calculator
class hitokoto {
private:
protected:
public:
	string handler(GroupMessage m,GroupImage& img) {
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ';')) {
			commands.push_back(temp);
		}
		if (commands.size() == 0)return "";
		if (*commands.begin() == "一言") {
			try {
				auto res = cpr::Get("https://v1.hitokoto.com");
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请查看终端以获取详细信息";
			}
		}
	}
};