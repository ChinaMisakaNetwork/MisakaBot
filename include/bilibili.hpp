#pragma once
#include <mirai.h>
#include <string>
#include <cpr/cpr.h>
using namespace std;
using namespace Cyan;
class bilibili {
private:
protected:
public:
	string handler(GroupMessage m) {
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ' ')) {
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return "";
		if(*cmds.begin()=="bilibili搜索"|| *cmds.begin()=="哔哩哔哩搜索" || *cmds.begin()=="B站搜索") {
			if(cmds.size()==2) {
				if(cmds[1]=="热门视频") {
					auto res = cpr::Get(cpr::Url{ "https://api.bilibili.com/x/web-interface/popular?ps=1&pn=1" });
					json data = json::parse(res.text);
					json reply = data["data"]["list"][0];
					string ans = "";
					ans += "分区：";
					ans += reply["tname"].get<string>();
					ans += '\n';
					ans += "标题：";
					ans += reply["title"].get<string>();
					ans += '\n';
					ans += "简介：";
					ans += reply["desc"].get<string>();
					ans += "视频链接：";
					ans += reply["short_link_v2"].get<string>();
					return ans;
				}
				if(cmds[1][0]=='B' && cmds[1][1]=='V') {
					auto res = cpr::Get(cpr::Url{ "https://api.bilibili.com/x/web-interface/view?bvid=" + cmds[1] });
					json reply = json::parse(res.text)["data"];
					string ans = "";
					ans += "分区：";
					ans += reply["tname"].get<string>();
					ans += '\n';
					ans += "标题：";
					ans += reply["title"].get<string>();
					ans += '\n';
					ans += "简介：";
					ans += reply["desc"].get<string>();
					return ans;
				}
				return "格式错误，请检查输入";
			}
			if(cmds.size()==3) {
				if(cmds[1]=="搜用户") {
					auto res = cpr::Get(cpr::Url{ "https://api.bilibili.com/x/space/acc/info?mid=" + cmds[2] }, cpr::Header{{"user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36"}});
					json reply = json::parse(res.text)["data"];
					string ans = "";
					ans += "昵称：";
					ans += reply["name"].get<string>();
					ans += '\n';
					ans += "性别：";
					ans += reply["sex"].get<string>();
					ans += '\n';
					ans += "等级：";
					ans += to_string(reply["level"].get<int>());
					ans += '\n';
					ans += "硬币个数：";
					ans += to_string(reply["coins"].get<int>());
					ans += '\n';
					ans += "生日：";
					ans += reply["birthday"].get<string>();
					return ans;
				}
			}
			return "格式错误，请检查输入";
		}
		return "";
	}
};