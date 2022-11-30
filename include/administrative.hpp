#pragma once
#include <mysql++.h>
#include <iostream>
#include <mirai.h>
#include "admin.hpp"
using namespace std;
using namespace Cyan;
class administrative :public permchecker {
public:
	explicit administrative(db_info dbinf):permchecker(dbinf) {
		
	}
	MessageChain handler(GroupMessage m) {
		MiraiBot& bot=(m.GetMiraiBot());
		MessageChain msg;
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.size() == 0)return msg;
		if (*commands.begin() == "/mute") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.Mute(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())), atoll(commands[2].c_str()));
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/kick") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2 && commands.size()!=3)return msg;
			try {
				if (commands.size() == 2)bot.Kick(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())));
				else bot.Kick(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())), commands[2]);
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/unmute") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			try {
				bot.UnMute(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())));
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/op") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			return grantperm(m.Sender.Group.GID.ToInt64(),stoll(commands[1]));
		}
		if (*commands.begin() == "更改群名片") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.SetGroupMemberName(m.Sender.Group.GID, QQ_t(atoll(commands[1].c_str())), commands[2]);
				msg.Add<PlainMessage>("更改成功");
				return msg;
			}catch(...) {
				
			}
		}
		if (*commands.begin() == "/muteall") {
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
		if (*commands.begin() == "/unmuteall") {
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
	MessageChain handler(FriendMessage m) {
		MiraiBot& bot = (m.GetMiraiBot());
		MessageChain msg;
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.size() == 0)return msg;
		if (*commands.begin() == "/mute") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 4)return msg;
			try {
				bot.Mute(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())), atoll(commands[3].c_str()));
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/kick") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 3 && commands.size() != 4)return msg;
			try {
				if (commands.size() == 3)bot.Kick(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())));
				else bot.Kick(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())), commands[3]);
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/unmute") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			try {
				bot.UnMute(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())));
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/op") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 2)return msg;
			return grantperm(stoll(commands[1]), stoll(commands[2]));
		}
		if (*commands.begin() == "更改群名片") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.SetGroupMemberName(GID_t(stoll(commands[1])), QQ_t(atoll(commands[2].c_str())), commands[3]);
				msg.Add<PlainMessage>("更改成功");
				return msg;
			}
			catch (...) {

			}
		}
		if (*commands.begin() == "/muteall") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			try {
				bot.MuteAll(GID_t(stoll(commands[1])));
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
				return msg;
			}
		}
		if (*commands.begin() == "/unmuteall") {
			if (!checkperm(stoll(commands[1]), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			try {
				bot.UnMuteAll(GID_t(stoll(commands[1])));
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