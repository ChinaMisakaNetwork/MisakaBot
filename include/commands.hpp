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
	explicit commands(const db_info dbinf) :permchecker(dbinf) {
		dbinfo = dbinf;
	}
	MessageChain handler(GroupMessage m) {
		MessageChain msg;
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ';')) {
			if (temp.empty())continue;
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return msg;
		if (*cmds.begin() == "添加对话") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (cmds.size() > 5 || cmds.size() < 4){
				msg.Add<PlainMessage>("参数错误");
				return msg;
			}
			try {
				query.reset();
				if (cmds[3] == "1") {
					query << "insert into miraichat(question, answer, have_img, img_url) values (%0q,%1q,%2q,%3q)";
					query.parse();
					mysqlpp::SimpleResult res = query.execute(cmds[1], cmds[2], 1, cmds[4]);
					if (string(res.info()).empty()) {
						msg.Add<PlainMessage>("添加成功");
						return msg;
					}
					cout << res.info() << endl;
					msg.Add<PlainMessage>("添加失败，请查看终端以获取详细信息");
					return msg;
				}
				else if (cmds[3] == "0") {
					query << "insert into miraichat(question, answer, have_img) values (%0q,%1q,%2q)";
					query.parse();
					mysqlpp::SimpleResult res = query.execute(cmds[1], cmds[2], 0);
					if (string(res.info()).empty()) {
						msg.Add<PlainMessage>("添加成功");
						return msg;
					}
					cout << string(res.info()) << endl;
					msg.Add<PlainMessage>("添加失败，请查看终端以获取详细信息");
				}
				return msg;
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请稍后再试");
				return msg;
			}
		}
		if (*cmds.begin() == "删除对话") {
			if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
				msg.Add<PlainMessage>("您不是本群的管理员");
				return msg;
			}
			if (cmds.size() != 2){
				msg.Add<PlainMessage>("参数错误");
				return msg;
			}
			try {
				query.reset();
				query << "delete from miraichat where question = %0q";
				query.parse();
				mysqlpp::SimpleResult res = query.execute(cmds[1]);
				if (string(res.info()).empty()) {
					msg.Add<PlainMessage>("删除成功");
					return msg;
				}
				cout << string(res.info()) << endl;
				msg.Add<PlainMessage>("添加失败，请查看终端以获取详细信息");
				return msg;
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("出现错误，请稍后再试");
				return msg;
			}
		}
		return msg;
	}
	
};