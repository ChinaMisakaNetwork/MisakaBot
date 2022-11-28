#pragma once
#include <mirai.h>
#include <iostream>
#include <cpr/cpr.h>
#include <string>
using namespace Cyan;
using namespace std;
class neteasemusic {
public:
    MessageChain search(vector<string>commands) const {
        if (*commands.begin() == "搜歌") {
            MessageChain msg;
            if (commands.size() != 2) {
				msg.Add<PlainMessage>("参数错误");
                return msg;
            }
            try {
                msg.Add<PlainMessage>("结果如下：\n");
                auto res = cpr::Get(cpr::Url{ "https://v1.hitokoto.cn/nm/search/" + commands[1]});
                json data = json::parse(res.text);
                json reply = data["result"]["songs"];
                if (reply.empty()) {
                    return MessageChain().Plain("未找到结果");
                }
                for (auto i = reply.begin(); i != reply.end() && (i-reply.begin())<=10; ++i) {
                    string ans;
                    ans += to_string(i-reply.begin() + 1);
                    ans += '.';
                    ans += "歌曲名：";
                    ans += i->at("name").get<string>();
                    ans += "  创作者：";
                    ans += i->at("artists")[0].at("name").get<string>();
                    ans += "  歌曲id：";
                    ans += to_string(i->at("id").get<int>());
                    ans += '\n';
                    msg.Add<PlainMessage>(ans);
                }
                return msg;
            }
            catch (const exception& ex) {
                cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请稍后再试");
                return msg;
            }
        }
        return MessageChain();
    }
    static MessageChain sing(GroupMessage m, vector<string>commands) {
        if (*commands.begin() != "唱歌")return MessageChain();
        MessageChain msg;
        if (commands.size() != 2) {
            return msg.Add<PlainMessage>("格式错误，请检查格式");
        }
        try {
            MiraiBot& bot = m.GetMiraiBot();
            MessageChain mc;
            VoiceMessage vc;
            string url = "http://music.163.com/song/media/outer/url?id=" + commands[1] + ".mp3";
            vc.Url(url);
            mc.Add<VoiceMessage>(vc);
            return mc;
        }
        catch (...) {}
        return MessageChain();
    }
	MessageChain handler(GroupMessage m) {
        stringstream sin((m.MessageChain.GetPlainText()));
        vector<string>commands;
        string temp;
        while (getline(sin, temp, ' ')) {
            if (temp.empty())continue;
            commands.push_back(temp);
        }
        if (commands.size() == 0)return MessageChain();
        // ReSharper disable once CppTooWideScopeInitStatement
        MessageChain search1 = search(commands);
        if (!search1.Empty())return search1;
        return sing(m, commands);
	}
};