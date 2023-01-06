#pragma once
#include <mirai.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <iostream>
#include "admin.hpp"
using namespace std;
using namespace Cyan;
class watchdog :public permchecker{
    db_info dbinfo;
    static bool ingroup(MiraiBot& bot,const long long&group,const long long&qq) {
        const vector<GroupMember> members = bot.GetGroupMembers(GID_t(group));
        for(auto i:members) {
			if (i.QQ == QQ_t(qq)) {
				return true;
			}
        }
        return false;
    }
public:
    struct report {
        long long group = 0;
        long long qq = 0;
        long long sender = 0;
        string reason = "";
    };
    watchdog(db_info dbinf) : permchecker(dbinf) {
        dbinfo = dbinf;
    }
    MessageChain handler(GroupMessage m, map<long long, vector<report>>& trtr, map<long long, mutex>& wdl) const {
        stringstream sin((m.MessageChain.GetPlainText()));
        MessageChain msg;
        vector<string>commands;
        string temp;
        while (getline(sin, temp, ' ')) {
            if (temp.empty())continue;
            commands.push_back(temp);
        }
        if (commands.size() == 0)return MessageChain();
        if (*commands.begin() == "/wdr") {
            if (commands.size() == 1) return MessageChain().Plain("您想举报谁呢？");
            report wdr;
            if (commands.size() >= 2) {
                wdr.group = m.Sender.Group.GID.ToInt64();
                wdr.sender = m.Sender.QQ.ToInt64();
                for (auto i : commands[1]) {
                    if (!isdigit(i))return MessageChain().Plain("你这QQ号不对啊");
                }
                wdr.qq = atoll(commands[1].c_str());
                if (!ingroup(m.GetMiraiBot(), wdr.group, wdr.qq)) {
                    return MessageChain().Plain("这个人不在群里啊");
                }
                if (commands.size() >= 3) {
                    wdr.reason = commands[2];
                }
                wdl[wdr.group].lock();
                trtr[wdr.group].push_back(wdr);
                wdl[wdr.group].unlock();
                return MessageChain().Plain("\n感谢您的举报，我们会尽快处理的");
            }
            return MessageChain();
        }
        return MessageChain();
    }
    MessageChain handler(FriendMessage m, map<long long, vector<report>>& trtr,map<long long,mutex>&wdl) {
        stringstream sin((m.MessageChain.GetPlainText()));
        MessageChain msg;
        vector<string>commands;
        string temp;
        while (getline(sin, temp, ' ')) {
            if (temp.empty())continue;
            commands.push_back(temp);
        }
        if (commands.size() == 0)return MessageChain();
        if (*commands.begin() == "/wdr") {
            if (commands.size() == 1) return MessageChain().Plain("您想举报谁呢？");
            report wdr;
            if (commands.size() >= 3) {
                wdr.sender = m.Sender.QQ.ToInt64();
                for (auto i : commands[1]) {
                    if (!isdigit(i)) {
                        return MessageChain().Plain("你这群号不对啊");
                    }
                }
                wdr.group = atoll(commands[1].c_str());
                for (auto i : commands[2]) {
                    if (!isdigit(i)) {
                        return MessageChain().Plain("你这QQ号不对啊");
                    }
                }
                wdr.qq = atoll(commands[2].c_str());
                if (!ingroup(m.GetMiraiBot(), wdr.group, wdr.qq)) {
                    return MessageChain().Plain("这个人不在群里啊");
                }
                if (commands.size() >= 4) {
                    wdr.reason = commands[3];
                }
                wdl[wdr.group].lock();
				trtr[wdr.group].push_back(wdr);
                wdl[wdr.group].unlock();
                return MessageChain().Plain("感谢您的举报，我们会尽快处理的");
            }
        }
        if(*commands.begin()=="/atlas") {
            if (commands.size() == 1)return MessageChain().Plain("请指定群号");
            if(commands.size()>=2) {
                for (auto i : commands[1]) {
                    if (!isdigit(i)) {
                        return MessageChain().Plain("你这群号不对啊");
                    }
                }
                long long group = atoll(commands[1].c_str());
                if (!checkperm(group, m.Sender.QQ.ToInt64())) {
                    return MessageChain().Plain("您不是该群的管理员");
                }
                if (trtr[group].empty())return MessageChain().Plain("没有待处理的举报");
                wdl[group].lock();
                report now = trtr[group][0];
				trtr[group].erase(trtr[group].begin());
                wdl[group].unlock();
                MiraiBot& bot = m.GetMiraiBot();
                MessageChain reply;
                reply.Plain("被举报者：" + to_string(now.qq) + '\n');
                reply.Plain("举报者：" + to_string(now.sender) + '\n');
                if (!now.reason.empty())reply.Plain("原因：" + now.reason);
                bot.SendMessage(m.Sender.QQ, reply);
            }
        }
        return MessageChain();
	}
};