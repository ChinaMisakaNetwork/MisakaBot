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
                auto res = cpr::Get(cpr::Url{ "https://autumnfish.cn/search?keywords=" + commands[1]});
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
            auto res = cpr::Get(cpr::Url{ "https://autumnfish.cn/song/url?id=" + commands[1] });
            json data = json::parse(res.text);
            string url = data["data"][0].at("url").get<string>();
            vc.Url(url);
            mc.Add<VoiceMessage>(vc);
            return mc;
        }
        catch (...) {}
        return MessageChain();
    }
    MessageChain MusicShareXML(GroupMessage m, vector<string>commands) const {
        if (*commands.begin() != "点歌")return MessageChain();
        MessageChain msg;
        if (commands.size() != 2) {
            return msg.Add<PlainMessage>("格式错误，请检查格式");
        }
        string& name=commands[1];
        string refAddress="https://music.163.com/song?id=";
        string alias;
        string title;
        string singer;
        MusicShare ms;
        auto res=cpr::Get(cpr::Url{"https://autumnfish.cn/search?keywords="+name});
        json data=json::parse(res.text)["result"]["songs"][0];
        if(data.empty())return MessageChain().Plain("未找到结果");
        alias=data["alias"][0].get<string>();
        title=data["name"].get<string>();
        singer=data["artists"][0]["name"].get<string>();
        refAddress+=to_string(data["id"].get<int>());
        ms.Title(title+" （"+alias+"）");
        ms.Summary(singer+" - "+title);
        ms.PictureUrl(data["album"]["picUrl"].get<string>());
        auto musicres = cpr::Get(cpr::Url{ "https://autumnfish.cn/song/url?id=" + to_string(data["id"].get<int>()) });
        json datamusic = json::parse(res.text);
        string musicurl = data["data"][0].at("url").get<string>();
        ms.MusicUrl(musicurl);
        ms.JumUrl(refAddress);
        msg.Add<MusicShare>(ms);
        return msg;
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
        MessageChain search1 = search(commands);
        if (!search1.Empty())return search1;
        search1=sing(m,commands);
        if (!search1.Empty())return search1;
        return MusicShareXML(m,commands);
	}
};