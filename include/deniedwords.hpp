#pragma once
#include <mirai.h>
#include <string>
#include "admin.hpp"
#include "autoac.hpp"
#include "wordcheck.hpp"
using namespace std;
using namespace Cyan;
class deniedwords :public DatabaseOperator{
    db_info dbinfo_;
public:
    explicit deniedwords(const db_info dbinf) : DatabaseOperator(dbinf) {
        dbinfo_ = dbinf;
    }
    MessageChain handler(GroupMessage m,map<long long, trie_ac>& ac_automation, map<long long, bool>& wordcheck_list, mutex& ac_automation_lock, mutex& wordcheck_list_lock) {
        stringstream sin((m.MessageChain.GetPlainText()));
        MessageChain msg;
        vector<string>commands;
        string temp;
        while (getline(sin, temp, ' ')) {
            if (temp.empty())continue;
            commands.push_back(temp);
        }
		if (commands.size() == 0)return MessageChain();
        if (*commands.begin() == "添加敏感词") {
            if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
                return MessageChain();
            }
            if (commands.size() != 2)return MessageChain();
            try {
                query.reset();
                string s=commands[1];
                query << "insert into deniedwords(groupid, word) values (%0q,%1q)";
                query.parse();
                mysqlpp::SimpleResult res = query.execute(to_string(m.Sender.Group.GID.ToInt64()), s);
                if (string(res.info()).empty()) {
					msg.Add<PlainMessage>("添加成功");
                    wordchecker chk(dbinfo_);
					chk.init(ac_automation, wordcheck_list, ac_automation_lock, wordcheck_list_lock);
                    return msg;
                }
                cout << string(res.info()) << endl;
                msg.Add<PlainMessage>("添加失败，请查看终端以获取详细信息");
                return msg;
            }
            catch (const std::exception& ex) {
                cout << ex.what() << endl;
                msg.Add<PlainMessage>("添加失败，请查看终端以获取详细信息");
                return msg;
            }
        }
        if (*commands.begin() == "删除敏感词") {
            if (!checkperm(m.Sender.Group.GID.ToInt64(), m.Sender.QQ.ToInt64())) {
                return MessageChain();
            }
            if (commands.size() != 2)return msg;
            try {
                query.reset();
                string s=commands[1];
                query << "delete from deniedwords where groupid = %0q and word = %1q";
                query.parse();
                mysqlpp::SimpleResult res = query.execute(to_string(m.Sender.Group.GID.ToInt64()),s);
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
                msg.Add<PlainMessage>("添加失败，请查看终端以获取详细信息");
                return msg;
            }
        }
        return msg;
    }
};