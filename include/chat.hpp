#pragma once
#include <mirai.h>
#include "admin.hpp"
#include <mysql++.h>
#include <vector>
#include <mutex>
#include <map>
#include <cpr/cpr.h>
#include <mutex>
using namespace std;
using namespace Cyan;
map<long long,string>cvids;
map<long long,string>pids;
map<long long,mutex>cvl,pl;
class chatobj :public permchecker{
	db_info dbinfo_;
    mutex safelock;
public:
	explicit chatobj(const db_info dbf):permchecker(dbf) {
		dbinfo_ = dbf;
	}
	MessageChain handler(GroupMessage m) {
        safelock.lock();
		MessageChain msg;
		if (!m.AtMe())return msg;
		try {
			string s = m.MessageChain.GetPlainText();
			query.reset();
			query << "select * from miraichat where %0q regexp question order by char_length(question) desc";
			query.parse();
			mysqlpp::StoreQueryResult sres = query.store(s);
			if (sres.size()>0) {
                string ans = "";
                if (sres[0]["have_img"].to_string(ans), ans == "1") {
                    sres[0]["img_url"].to_string(ans);
                    msg.Image(m.GetMiraiBot().UploadGroupImage(ans, true));
                }
                sres[0]["answer"].to_string(ans);
                msg.Add<PlainMessage>(ans);
                safelock.unlock();
                return msg;
            }
            json sent;
            cvl[m.Sender.Group.GID.ToInt64()].lock();
            pl[m.Sender.Group.GID.ToInt64()].lock();
            sent["message"]=s;
            if(cvids.contains(m.Sender.Group.GID.ToInt64())){
                sent["conversation_id"]=cvids[m.Sender.Group.GID.ToInt64()];
            }
            else{
                sent["conversation_id"]="0";
            }
            if(pids.contains(m.Sender.Group.GID.ToInt64())){
                sent["parent_id"]=pids[m.Sender.Group.GID.ToInt64()];
            }
            else{
                sent["parent_id"]="0";
            }
			auto res = cpr::Post(cpr::Url{"http://localhost:1234/"},cpr::Body{sent.dump()},cpr::Header{{"Content-Type", "application/json"}});
            json reply=json::parse(res.text);
            cvids[m.Sender.Group.GID.ToInt64()]=reply["conversation_id"].get<string>();
            pids[m.Sender.Group.GID.ToInt64()]=reply["parent_id"].get<string>();
            cvl[m.Sender.Group.GID.ToInt64()].unlock();
            pl[m.Sender.Group.GID.ToInt64()].unlock();
            msg.Add<PlainMessage>(reply["message"].get<string>());
            safelock.unlock();
			return msg;
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
            cvl[m.Sender.Group.GID.ToInt64()].unlock();
            pl[m.Sender.Group.GID.ToInt64()].unlock();
			msg.Add<PlainMessage>("出现错误，请稍后再试");
            safelock.unlock();
			return msg;
		}
	}
    MessageChain handler(FriendMessage m) {
		MessageChain msg;
        safelock.lock();
		try {
			string s = m.MessageChain.GetPlainText();
            if(s.empty())return MessageChain();
			query.reset();
			query << "select * from miraichat where %0q regexp question order by char_length(question) desc";
			query.parse();
			mysqlpp::StoreQueryResult sres = query.store(s);
			if (sres.size()>0) {
                string ans = "";
                if (sres[0]["have_img"].to_string(ans), ans == "1") {
                    sres[0]["img_url"].to_string(ans);
                    msg.Image(m.GetMiraiBot().UploadGroupImage(ans, true));
                }
                sres[0]["answer"].to_string(ans);
                msg.Add<PlainMessage>(ans);
                safelock.unlock();
                return msg;
            }
            json sent;
            cvl[m.Sender.QQ.ToInt64()].lock();
            pl[m.Sender.QQ.ToInt64()].lock();
            sent["message"]=s;
            if(cvids.contains(m.Sender.QQ.ToInt64())){
                sent["conversation_id"]=cvids[m.Sender.QQ.ToInt64()];
            }
            else{
                sent["conversation_id"]="0";
            }
            if(pids.contains(m.Sender.QQ.ToInt64())){
                sent["parent_id"]=pids[m.Sender.QQ.ToInt64()];
            }
            else{
                sent["parent_id"]="0";
            }
			auto res = cpr::Post(cpr::Url{"http://localhost:1234/"},cpr::Body{sent.dump()},cpr::Header{{"Content-Type", "application/json"}});
            json reply=json::parse(res.text);
            cvids[m.Sender.QQ.ToInt64()]=reply["conversation_id"].get<string>();
            pids[m.Sender.QQ.ToInt64()]=reply["parent_id"].get<string>();
            cvl[m.Sender.QQ.ToInt64()].unlock();
            pl[m.Sender.QQ.ToInt64()].unlock();
            msg.Add<PlainMessage>(reply["message"].get<string>());
            safelock.unlock();
			return msg;
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
            cvl[m.Sender.QQ.ToInt64()].unlock();
            pl[m.Sender.QQ.ToInt64()].unlock();
			msg.Add<PlainMessage>("出现错误，请稍后再试");
            safelock.unlock();
			return msg;
		}
	}
};