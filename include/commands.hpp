#pragma once
#include "admin.hpp"
#include <mirai.h>
#include <vector>
#include <exception>
#include <string>
using namespace std;
using namespace Cyan;
class commands :public permchecker{
private:
	db_info dbinfo;
public:
	commands(db_info dbinf) :permchecker(dbinf) {
		dbinfo = dbinf;
	}
	string handler(GroupMessage m) {
		MiraiBot& bot = m.GetMiraiBot();
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ';')) {
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return "";
		if (*cmds.begin() == "添加对话") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return "您不是本群的管理员";
			}
			if (cmds.size() > 5 || cmds.size() < 4)return "请检查输入格式！";
			try {
				query.reset();
				if (cmds[3] == "1") {
					query << "insert into miraichat(question, answer, have_img, img_url) values (%0q,%1q,%2q,%3q)";
					query.parse();
					mysqlpp::SimpleResult res = query.execute(cmds[1], cmds[2], 1, cmds[4]);
					if (string(res.info()).empty()) {
						return "已添加";
					}
					return string(res.info());
				}
				else if (cmds[3] == "0") {
					query << "insert into miraichat(question, answer, have_img) values (%0q,%1q,%2q)";
					query.parse();
					mysqlpp::SimpleResult res = query.execute(cmds[1], cmds[2], 0);
					if (string(res.info()).empty()) {
						return "已添加";
					}
					cout << string(res.info()) << endl;
					return "出现错误，请稍后再试";
				}
				return "";
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		if (*cmds.begin() == "删除对话") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				return "Permission Denied!";
			}
			if (cmds.size() != 2)return "请检查输入格式！";
			try {
				query.reset();
				query << "delete from miraichat where question = %0q";
				query.parse();
				mysqlpp::SimpleResult res = query.execute(cmds[1]);
				if (string(res.info()).empty()) {
					return "已删除";
				}
				cout << string(res.info()) << endl;
				return "出现错误，请稍后再试";
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		return "";
	}
	
};