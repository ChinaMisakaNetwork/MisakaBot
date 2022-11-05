#pragma once
#include <vector>
#include <mirai.h>
#include <map>
#include <cpr/cpr.h>
#include <string>
#include <mutex>
#include "admin.hpp"
using namespace std;
using namespace Cyan;
class wordchecker :public permchecker{
private:
	db_info dbinf;
public:
	wordchecker(db_info dbinf1) :permchecker(dbinf1) {
		dbinf = dbinf1;
	}
	void init(map<int, TrieAc>& ACer, map<int, bool>& enabled,mutex& ACer_lock, mutex &wordcheck_list_lock) {
		map<int, vector<string>>temp;
		ACer_lock.lock();
		wordcheck_list_lock.lock();
		ACer.clear();
		enabled.clear();
		query.reset();
		query << "select * from deniedwords";
		mysqlpp::StoreQueryResult res = query.store();
		if (res) {
			for (size_t i = 0; i < res.num_rows(); ++i) {
				string m;
				res[i]["groupid"].to_string(m);
				int groupid = atoi(m.c_str());
				string s;
				res[i]["word"].to_string(s);
				temp[groupid].push_back(s);
			}
			for (auto i = temp.begin(); i != temp.end(); ++i) {
				ACer[i->first].load_pattern(i->second);
				ACer[i->first].dispose();
				cout << i->first << "敏感词加载完成" << endl;
				enabled[i->first] = true;
			}
		}
		ACer_lock.unlock();
		wordcheck_list_lock.unlock();
	}
	string handler(GroupMessage m, map<int, TrieAc>& ACer, map<int, bool>& enabled,string deepai_key,const double& nsfw_value) {
		MiraiBot& bot = m.GetMiraiBot();
		if (m.MessageChain.GetPlainText().empty())return "";
		string temp = m.MessageChain.GetPlainText();
		set<int>pair_res;
		if (!enabled[m.Sender.Group.GID.ToInt64()])return "";
		ACer[m.Sender.Group.GID.ToInt64()].match(temp,pair_res);
		if (!pair_res.empty()) {
			try {
				bot.Recall(m.MessageId(), m.Sender.Group.GID);
				bot.Mute(m.Sender.Group.GID, m.Sender.QQ, 60);
				return "请注意言辞";
			}
			catch (const std::exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		//Image nsfw check
		try {
			if (deepai_key.empty())return "";
			vector<ImageMessage>mc = m.MessageChain.GetAll<ImageMessage>();
			for (ImageMessage i : mc) {
				auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/nsfw-detector" },
					cpr::Payload{ {"image",i.Url()},
					},
					cpr::Header{ {"Api-Key",deepai_key} });
				json reply = json::parse(res.text);
				double score = reply["output"]["nsfw_score"].get<double>();
				if (score >= nsfw_value) {
					bot.Recall(m.MessageId(), m.Sender.Group.GID);
					bot.Mute(m.Sender.Group.GID, m.Sender.QQ, 60);
					return "您这图片在这发不太合适吧……";
				}
			}
		}catch(...){}
		return "";
	}
};