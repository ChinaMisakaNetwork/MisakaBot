﻿#pragma once
#include <mirai.h>
#include <string>
#include "admin.hpp"
#include "autoac.hpp"
using namespace std;
using namespace Cyan;
class deniedwords :public permchecker{
private:
    db_info dbinfo;
public:
    deniedwords(db_info dbinf) :permchecker(dbinf) {
        dbinfo = dbinf;
    }
    string handler(GroupMessage m) {
        MiraiBot& bot = m.GetMiraiBot();
        stringstream sin((m.MessageChain.GetPlainText()));
        vector<string>commands;
        string temp;
        while (getline(sin, temp, ' ')) {
            commands.push_back(temp);
        }
        if (commands.size() == 0)return "";
        if (*commands.begin() == "添加敏感词") {
            if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
                return "您不是本群的管理员";
            }
            if (commands.size() != 2)return "";
            try {
                query.reset();
                string s=commands[1];
                query << "insert into deniedwords(groupid, word) values (%0q,%1q)";
                query.parse();
                mysqlpp::SimpleResult res = query.execute(to_string(m.Sender.Group.GID.ToInt64()), s);
                if (string(res.info()).empty()) {
                    return "已添加";
                }
                cout << string(res.info()) << endl;
                return "出现错误，请稍后再试";
            }
            catch (const std::exception& ex) {
                cout << ex.what() << endl;
                return "出现错误，请稍后再试";
            }
        }
        if (*commands.begin() == "删除敏感词") {
            if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
                return "您不是本群的管理员";
            }
            if (commands.size() != 2)return "";
            try {
                query.reset();
                string s=commands[1];
                query << "delete from deniedwords where groupid = %0q and word = %1q";
                query.parse();
                mysqlpp::SimpleResult res = query.execute(to_string(m.Sender.Group.GID.ToInt64()),s);
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