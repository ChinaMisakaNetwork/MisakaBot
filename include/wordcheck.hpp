#pragma once
#include <vector>
#include <mirai.h>
#include <map>
#include <cpr/cpr.h>
#include <string>
#include <mutex>
#include "admin.hpp"
#include "autoac.hpp"
using namespace std;
using namespace Cyan;
class wordchecker :public DatabaseOperator{
	db_info dbinf;
    bool deobfForward(const vector<ForwardMessage>& vec,const GroupMessage& m,map<long long, trie_ac>& ACer){
        if(vec.empty())return true;
        for(auto i:vec) {
            for (const auto& j: i.NodeList()) {
                if (!deobfForward(j.MessageChain.GetAll<ForwardMessage>(), m, ACer))return false;
                set<int> pair_res;
                //cerr << j.MessageChain.GetPlainText() << endl;
                ACer[m.Sender.Group.GID.ToInt64()].match(j.MessageChain.GetPlainText(), pair_res);
                if (!pair_res.empty()) return false;
            }
        }
        return true;
    }
    bool picdeobfForward(const vector<ForwardMessage>& vec,const string& deepai_key,const double& nsfw_value) {
        if(vec.empty())return true;
        for(auto i:vec) {
            for (const auto& j: i.NodeList()) {
                if (!picdeobfForward(j.MessageChain.GetAll<ForwardMessage>(), deepai_key, nsfw_value))return false;
                for (auto k: j.MessageChain.GetAll<ImageMessage>()) {
                    if (!picdeobf(k, deepai_key, nsfw_value))return false;
                }
            }
        }
        return true;
    }
    static bool picdeobf(ImageMessage &j,const string& deepai_key,const double& nsfw_value){
        try {
            auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/nsfw-detector" },cpr::Payload{ {"image",j.Url()},},cpr::Header{ {"Api-Key",deepai_key} });
            json reply = json::parse(res.text);
            auto score = reply["output"]["nsfw_score"].get<double>();
            if (score >= nsfw_value) {
                return false;
            }
        }
        catch (...) {}
        return true;
    }
public:
	explicit wordchecker(const db_info& dbinf1) : DatabaseOperator(dbinf1) {
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
			map<long long, vector<string>> temp;
			for (size_t i = 0; i < res.num_rows(); ++i) {
				string m;
				res[i]["groupid"].to_string(m);
				const long long groupid = atoll(m.c_str()); // NOLINT(cert-err34-c)
				string s;
				res[i]["word"].to_string(s);
				temp[groupid].push_back(s);
			}
			for (auto & i : temp) {
				ACer[i.first].load_pattern(i.second);
				ACer[i.first].dispose();
				cout << i.first << "敏感词加载完成" << endl;
				enabled[i.first] = true;
			}
		}
		ACer_lock.unlock();
		wordcheck_list_lock.unlock();
	}
	MessageChain handler(GroupMessage m, map<long long, trie_ac>& ACer, map<long long, bool>& enabled,const string& deepai_key,const double& nsfw_value,const bool& nsfw_enabled) {
		MiraiBot& bot = m.GetMiraiBot();
		MessageChain msg;
		set<int>pair_res;
		if (m.MessageChain.GetPlainText().empty())goto chkforward;
		if (!enabled[m.Sender.Group.GID.ToInt64()])return {};
		ACer[m.Sender.Group.GID.ToInt64()].match(m.MessageChain.GetPlainText(),pair_res);
		if (!pair_res.empty()) {
			try {
				bot.Recall(m.MessageId(), m.Sender.Group.GID);
				bot.Mute(m.Sender.Group.GID, m.Sender.QQ, 60);
				msg.Add<PlainMessage>("请注意言辞");
                return msg;
			}
			catch (...) {
			}
		}
        chkforward:
        if (m.MessageChain.GetAll<ForwardMessage>().empty())goto chkpicture;
        if (!deobfForward(m.MessageChain.GetAll<ForwardMessage>(), m, ACer)) {
            try {
                bot.Recall(m.MessageId(), m.Sender.Group.GID);
                bot.Mute(m.Sender.Group.GID, m.Sender.QQ, 60);
                msg.Add<PlainMessage>("请注意言辞");
                return msg;
            }
            catch (...) {}
        }
		//Image nsfw check
		chkpicture:
		try {
			if (!nsfw_enabled)return {};
			if (deepai_key.empty())return msg;
			vector<ImageMessage>mc = m.MessageChain.GetAll<ImageMessage>();
			for (ImageMessage& i : mc) {
				if (!picdeobf(i, deepai_key, nsfw_value)) {
					bot.Recall(m.MessageId(), m.Sender.Group.GID);
					return MessageChain().Plain("您这图片在这发不太合适吧…");
				}
			}
		}catch(...){}
        if (m.MessageChain.GetAll<ForwardMessage>().empty())return {};
        if (!picdeobfForward(m.MessageChain.GetAll<ForwardMessage>(), deepai_key, nsfw_value)) {
            bot.Recall(m.MessageId(), m.Sender.Group.GID);
            return MessageChain().Plain("您这图片在这发不太合适吧…");
        }
        return {};
	}
};