#pragma once
#include <mysql++.h>
#include <iostream>
#include <mirai.h>
#include "admin.hpp"
#include <mutex>
using namespace std;
using namespace Cyan;
class administrative :public DatabaseOperator {
public:
	explicit administrative(db_info dbinf): DatabaseOperator(dbinf) {
		
	}
	MessageChain handler(GroupMessage m) {
		MiraiBot& bot=(m.GetMiraiBot());
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.size() == 0){
            return MessageChain();
        }
		if (*commands.begin() == "/mute") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 3)return MessageChain();
			try {
				bot.Mute(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())), atoll(commands[2].c_str()));
			}
			catch (const std::exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
        if(*commands.begin()=="/recall"){
            if(!checkperm(m.Sender.Group.GID.ToInt64(),m.Sender.QQ.ToInt64())){
                return MessageChain().Plain("您不是本群的管理员");
            }
            if(commands.size()!=1)return MessageChain();
            try{
                bot.Recall(m.MessageChain.GetFirst<QuoteMessage>().MessageId(),m.Sender.Group.GID);
                bot.Recall(m.MessageId(),m.Sender.Group.GID);
            }
            catch(const std::exception& ex){
                cerr<<ex.what()<<endl;
                return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
            }
        }
		if (*commands.begin() == "/kick") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 2 && commands.size()!=3)return MessageChain();
			try {
				if (commands.size() == 2)bot.Kick(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())));
				else bot.Kick(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())), commands[2]);
			}
			catch (const exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/unmute") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 2)return MessageChain();
			try {
				bot.UnMute(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())));
			}
			catch (const exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/op") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 2)return MessageChain();
			return grantperm(m.Sender.Group.GID.ToInt64(),stoll(commands[1]));
		}
		if (*commands.begin() == "更改群名片") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 3)return MessageChain();
			try {
				bot.SetGroupMemberName(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())), commands[2]);
				return MessageChain().Plain("更改成功");
			}catch(const exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/muteall") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			try {
				bot.MuteAll(m.Sender.Group.GID);
			}
			catch (const std::exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/unmuteall") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			try {
				bot.UnMuteAll(m.Sender.Group.GID);
			}
			catch (const std::exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		return MessageChain();
	}
	MessageChain handler(FriendMessage m) {
		MiraiBot& bot = (m.GetMiraiBot());
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.size() == 0)return MessageChain();
		if (*commands.begin() == "/mute") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 4)return MessageChain();
			try {
				bot.Mute(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())), atoll(commands[3].c_str()));
			    return MessageChain().Plain("OK");
            }
			catch (const std::exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/kick") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 3 && commands.size() != 4)return MessageChain();
			try {
				if (commands.size() == 3)bot.Kick(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())));
				else bot.Kick(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())), commands[3]);
                return MessageChain().Plain("OK");
			}
			catch (const exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/unmute") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 3)return MessageChain();
			try {
				bot.UnMute(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())));
                return MessageChain().Plain("OK");
			}
			catch (const exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/op") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 3)return MessageChain();
			return grantperm(stoll(commands[1]), stoll(commands[2]));
		}
		if (*commands.begin() == "更改群名片") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			if (commands.size() != 4)return MessageChain();
			try {
				bot.SetGroupMemberName(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())), commands[3]);
				return MessageChain().Plain("更改成功");
			}
			catch (const exception& ex) {
                cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/muteall") {
			if (commands.size() != 2)return MessageChain();
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			try {
				bot.MuteAll(GID_t(stoll(commands[1])));
                return MessageChain().Plain("OK");
			}
			catch (const std::exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		if (*commands.begin() == "/unmuteall") {
			if (commands.size() != 2)return MessageChain();
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("您不是本群的管理员");
			}
			try {
				bot.UnMuteAll(GID_t(stoll(commands[1])));
                return MessageChain().Plain("OK");
			}
			catch (const std::exception& ex) {
				cerr << ex.what() << endl;
				return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
			}
		}
		return MessageChain();
	}
};