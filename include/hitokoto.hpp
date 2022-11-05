#pragma once
#include <mirai.h>
#include <mysql++.h>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace Cyan;
class hitokoto {
private:
protected:
public:
	string handler(GroupMessage m,GroupImage& img) {
		if (m.MessageChain.GetPlainText() == "一言") {
			try {
				auto res = cpr::Get(cpr::Url{ "https://v1.hitokoto.cn/" });
				json reply = json::parse(res.text);
				string htkt = reply["hitokoto"].get<string>();
				string from = reply["from"].get<string>();
				string id = to_string(reply["id"].get<int>());
				string ans = "";
				ans += htkt;
				ans += '\n';
				ans += "出自：";
				ans += from;
				ans += '\n';
				ans += "来源：hitokoto.cn，编号：";
				ans += id;
				res = cpr::Get(cpr::Url{ "https://tenapi.cn/acg/?return=json" });
				reply = json::parse(res.text);
				img.Url = reply["imgurl"].get<string>();
				return ans;
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		return "";
	}
};