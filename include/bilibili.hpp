#pragma once
#include <mirai.h>
#include <string>
#include <cpr/cpr.h>
using namespace std;
using namespace Cyan;
class bilibili {
public:
	MessageChain handler(GroupMessage m) const {
		stringstream sin(m.MessageChain.GetPlainText());
		MessageChain msg;
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return msg;
		if(*cmds.begin()=="bilibili搜索"|| *cmds.begin()=="哔哩哔哩搜索" || *cmds.begin()=="B站搜索" || *cmds.begin()=="/bilibili") {
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
					msg.Add<PlainMessage>(ans);
					return msg;
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
                    msg.Image(m.GetMiraiBot().UploadGroupImage(reply["pic"].get<string>(),true));
					ans += "简介：";
					ans += reply["desc"].get<string>();
                    ans += '\n';
                    ans+="分辨率：";
                    ans+=to_string(reply["dimension"]["width"].get<int>());
                    ans+="x";
                    ans+=to_string(reply["dimension"]["height"].get<int>());
                    ans+='\n';
                    ans+="播放量：";
                    ans+=to_string(reply["stat"]["view"].get<int>());
                    ans+='\n';
                    ans+="评论数：";
                    ans+=to_string(reply["stat"]["reply"].get<int>());
                    ans+='\n';
                    ans="点赞：";
                    ans+=to_string(reply["stat"]["like"].get<int>());
                    ans+='\n';
                    ans+="投币：";
                    ans+=to_string(reply["stat"]["coin"].get<int>());
                    ans+='\n';
                    ans+="收藏：";
                    ans+=to_string(reply["stat"]["favorite"].get<int>());
                    ans+='\n';
                    ans+="UP猪：";
                    ans+=reply["owner"]["name"].get<string>();
                    ans+='\n';
					msg.Add<PlainMessage>(ans);
                    msg.Image(m.GetMiraiBot().UploadGroupImage(reply["owner"]["face"].get<string>(), true));
					return msg;
				}
				msg.Add<PlainMessage>("请检查输入是否正确");
				return msg;
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
                    ans+='\n';
                    ans+="头像：";
					msg.Add<PlainMessage>(ans);
                    msg.Image(m.GetMiraiBot().UploadGroupImage(reply["face"].get<string>(), true));
                    msg.Add<PlainMessage>("签名："+reply["sign"].get<string>());
					return msg;
				}
			}
			msg.Add<PlainMessage>("请检查输入是否正确");
			return msg;
		}
		return msg;
	}
};