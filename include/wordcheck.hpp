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
	db_info dbinf;
public:
	wordchecker(db_info dbinf1) :permchecker(dbinf1) {
		dbinf = dbinf1;
	}
	void init(map<long long, trie_ac>& ACer, map<long long, bool>& enabled,mutex& ACer_lock, mutex &wordcheck_list_lock) {
		ACer_lock.lock();
		wordcheck_list_lock.lock();
		ACer.clear();
		enabled.clear();
		query.reset();
		query << "select * from deniedwords";
		if (const mysqlpp::StoreQueryResult res = query.store()) {
			map<int, vector<string>> temp;
			for (size_t i = 0; i < res.num_rows(); ++i) {
				string m;
				res[i]["groupid"].to_string(m);
				const long long groupid = atoll(m.c_str());
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
	MessageChain handler(GroupMessage m, map<long long, trie_ac>& ACer, map<long long, bool>& enabled,string deepai_key,const double& nsfw_value,const bool& nsfw_enabled) {
		MiraiBot& bot = m.GetMiraiBot();
		MessageChain msg;
		string temp = m.MessageChain.GetPlainText();
		set<int>pair_res;
		if (m.MessageChain.GetPlainText().empty())goto chkpicture;;
		if (!enabled[m.Sender.Group.GID.ToInt64()])return MessageChain();
		ACer[m.Sender.Group.GID.ToInt64()].match(temp,pair_res);
		if (!pair_res.empty()) {
			try {
				bot.Recall(m.MessageId(), m.Sender.Group.GID);
				bot.Mute(m.Sender.Group.GID, m.Sender.QQ, 60);
				msg.Add<PlainMessage>("请注意言辞");
			}
			catch (...) {
			}
		}
		//Image nsfw check
		chkpicture:
		try {
			if (!nsfw_enabled)return MessageChain();
			if (deepai_key.empty())return msg;
			vector<ImageMessage>mc = m.MessageChain.GetAll<ImageMessage>();
			for (ImageMessage& i : mc) {
				auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/nsfw-detector" },
					cpr::Payload{ {"image",i.Url()},
					},
					cpr::Header{ {"Api-Key",deepai_key} });
				json reply = json::parse(res.text);
				double score = reply["output"]["nsfw_score"].get<double>();
				if (score >= nsfw_value) {
					bot.Recall(m.MessageId(), m.Sender.Group.GID);
					return MessageChain().Plain('\n' + "您这图片在这发不太合适吧…");
				}
			}
		}catch(...){}
		return MessageChain();
	}
};