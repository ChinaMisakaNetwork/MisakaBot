#pragma once
#include <mirai.h>
#include <cpr/cpr.h>
#include <string>
using namespace std;
using namespace Cyan;
//http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=城市&needMoreData=true&pageNo=1&pageSize=7
class weather {
public:
	MessageChain handler(GroupMessage m) const {
		MessageChain msg;
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return MessageChain();
		if (*cmds.begin() == "查询天气") {
			if (cmds.size() != 2) {
				msg.Add<PlainMessage>("参数错误");
				return msg;
			}
			msg.Add<PlainMessage>(cmds[1] + "未来三天天气：" + '\n');
			auto res = cpr::Get(cpr::Url{ "http://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city=" + cmds[1] + "&needMoreData=true&pageNo=1&pageSize=7" });
			json data = json::parse(res.text);
			json reply = data["data"]["list"];
			if (reply.empty()) {
				msg.Add<PlainMessage>("未查询到结果");
				return msg;
			}
			for (auto i = reply.begin(); i != reply.end() && (i - reply.begin()) <= 3; ++i) {
				string ans = "";
				ans += "日期：";
				ans += i->at("date").get<string>();
				ans += "，天气：";
				ans += i->at("weather").get<string>();
				if (i == reply.begin()) {
					ans += "，目前温度：";
					ans += to_string(i->at("temp").get<int>());
				}
				ans += "，最高温度：";
				ans += to_string(i->at("high").get<int>());
				ans += "，最低温度：";
				ans += to_string(i->at("low").get<int>());
				ans += "，湿度：";
				ans += i->at("humidity").get<string>();
				ans += "，风向";
				ans += i->at("wind").get<string>();
				if (i->find("airData") != i->end() && i->find("airQuality")!=i->end() && i->find("pm25")!=i->end()) {
					ans += "，空气质量指数：";
					ans += i->at("airData").get<string>();
					ans += "（";
					ans += i->at("airQuality").get<string>();
					ans += "），PM2.5指数：";
					ans += to_string(i->at("pm25").get<int>());
				}
				ans += '\n';
				msg.Add<PlainMessage>(ans);
			}
			return msg;
		}
		return MessageChain();
	}
};