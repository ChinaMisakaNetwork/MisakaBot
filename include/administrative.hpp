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
		if (*commands.begin() == "����") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.Mute(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())), atoi(commands[2].c_str()));
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("���ִ�����鿴�ն��Ի�ȡ��ϸ��Ϣ");
				return msg;
			}
		}
		if (*commands.begin() == "�߳�") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			if (commands.size() != 2)return msg;
			try {
				bot.Kick(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())));
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("���ִ�����鿴�ն��Ի�ȡ��ϸ��Ϣ");
				return msg;
			}
		}
		if (*commands.begin() == "ȡ������") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			if (commands.size() != 2)return msg;
			try {
				bot.UnMute(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())));
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("���ִ�����鿴�ն��Ի�ȡ��ϸ��Ϣ");
				return msg;
			}
		}
		if (*commands.begin() == "��ӹ���Ա") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			if (commands.size() != 2)return msg;
			return grantperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64());
		}
		if (*commands.begin() == "����Ⱥ��Ƭ") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			if (commands.size() != 3)return msg;
			try {
				bot.SetGroupMemberName(m.Sender.Group.GID, QQ_t(atoi(commands[1].c_str())), commands[2]);
				msg.Add<PlainMessage>("���ĳɹ�");
				return msg;
			}catch(...) {
				
			}
		}
		if (*commands.begin() == "ȫ�����") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			try {
				bot.MuteAll(m.Sender.Group.GID);
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("���ִ�����鿴�ն��Ի�ȡ��ϸ��Ϣ");
				return msg;
			}
		}
		if (*commands.begin() == "ȫ����") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("�����Ǳ�Ⱥ�Ĺ���Ա");
				return msg;
			}
			try {
				bot.UnMuteAll(m.Sender.Group.GID);
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("���ִ�����鿴�ն��Ի�ȡ��ϸ��Ϣ");
				return msg;
			}
		}
		return msg;
	}
};