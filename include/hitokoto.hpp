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
	MessageChain handler(GroupMessage m) const {
		MessageChain msg;
		if (m.MessageChain.GetPlainText() == "一言") {
			try {
				auto res = cpr::Get(cpr::Url{ "https://v1.hitokoto.cn/" });
				json reply = json::parse(res.text);
				auto htkt = reply["hitokoto"].get<string>();
				auto from = reply["from"].get<string>();
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
				GroupImage img;
				img.Url = reply["imgurl"].get<string>();
				msg.Image(img);
				msg.Add<PlainMessage>(ans);
				return msg;
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请稍后再试");
				return msg;
			}
		}
		return msg;
	}
};