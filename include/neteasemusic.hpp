#pragma once
#include <mirai.h>
#include <iostream>
#include <cpr/cpr.h>
#include <string>
using namespace Cyan;
using namespace std;
class neteasemusic {
public:
    string search(vector<string>commands) {
        string ans = "搜索结果：";
        ans += '\n';
        if (*commands.begin() == "搜歌") {
            if (commands.size() != 2) {
                return "格式错误，请检查格式";
            }
            try {
                auto res = cpr::Get(cpr::Url{ "https://v1.hitokoto.cn/nm/search/" + commands[1]});
                json data = json::parse(res.text);
                json reply = data["result"]["songs"];
                for (auto i = reply.begin(); i != reply.end() && (i-reply.begin())<=10; ++i) {
                    ans += to_string(i-reply.begin() + 1);
                    ans += '.';
                    ans += "歌曲名：";
                    ans += i->at("name").get<string>();
                    ans += "  创作者：";
                    ans += i->at("artists")[0].at("name").get<string>();
                    ans += "  歌曲id：";
                    ans += to_string(i->at("id").get<int>());
                    ans += '\n';
                }
                return ans;
            }
            catch (const exception& ex) {
                cout << ex.what() << endl;
                return "出现错误，请稍后再试";
            }
        }
        return "";
    }
    string sing(GroupMessage m, vector<string>commands) {
        if (*commands.begin() != "唱歌")return "";
        if (commands.size() != 2) {
            return "格式错误，请检查格式";
        }
        try {
            MiraiBot& bot = m.GetMiraiBot();
            MessageChain mc;
            VoiceMessage vc;
            string url = "http://music.163.com/song/media/outer/url?id=" + commands[1] + ".mp3";
            vc.Url(url);
            mc.Add<VoiceMessage>(vc);
            bot.SendMessage(m.Sender.Group.GID, mc);
        }
        catch (...) {}
        return "";
    }
	string handler(GroupMessage m) {
        stringstream sin((m.MessageChain.GetPlainText()));
        vector<string>commands;
        string temp;
        while (getline(sin, temp, ' ')) {
            commands.push_back(temp);
        }
        if (commands.size() == 0)return "";
        string search1 = search(commands);
        if (!search1.empty())return search1;
        return sing(m, commands);
	}
};