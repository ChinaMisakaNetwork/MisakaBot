#pragma once
#include <mysql++.h>
#include <iostream>
#include <mirai.h>
#include "admin.hpp"
using namespace std;
using namespace Cyan;
class administrative :public permchecker {
public:
	administrative(db_info dbinf):permchecker(dbinf) {
		
	}
	MessageChain handler(GroupMessage m) {
		MiraiBot& bot=(m.GetMiraiBot());
		MessageChain msg;
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			commands.push_back(temp);
		}
		if (commands.size() == 0)return msg;
		if (*commands.begin() == "禁言") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.Mute(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())), atoi(commands[2].c_str()));
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "踢出") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			try {
				bot.Kick(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())));
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "取消禁言") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			try {
				bot.UnMute(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())));
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "添加管理员") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			return grantperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64());
		}
		if (*commands.begin() == "更改群名片") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.SetGroupMemberName(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())), commands[2]);
				msg.Add<PlainMessage>("更改成功");
				return msg;
			}catch(...) {
				
			}
		}
		if (*commands.begin() == "全体禁言") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			try {
				bot.MuteAll(m.Sender.Group.GID);
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "全体解禁") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			try {
				bot.UnMuteAll(m.Sender.Group.GID);
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		return msg;
	}
};