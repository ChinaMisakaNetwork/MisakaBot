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
class chatobj :public DatabaseOperator{
	db_info dbinfo_;
    mutex safelock;
public:
	explicit chatobj(const db_info dbf): DatabaseOperator(dbf) {
		dbinfo_ = dbf;
	}
	MessageChain handler(GroupMessage m) {
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
                return msg;
            }
			/*
            json sent;
            cvl[m.Sender.Group.GID.ToInt64()].lock();
            pl[m.Sender.Group.GID.ToInt64()].lock();
            sent["message"]=s;
            sent["group_id"]=m.Sender.Group.GID.ToInt64();
			auto res = cpr::Post(cpr::Url{"http://localhost:1234/"},cpr::Body{sent.dump()},cpr::Header{{"Content-Type", "application/json"}});
            */
			msg.Add<PlainMessage>("我不知道呢");
			return msg;
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
            //cvl[m.Sender.Group.GID.ToInt64()].unlock();
            //pl[m.Sender.Group.GID.ToInt64()].unlock();
			msg.Add<PlainMessage>("出现错误，请稍后再试");
			return msg;
		}
	}
    MessageChain handler(FriendMessage m) {
		MessageChain msg;
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
                return msg;
            }
			/*
            json sent;
            cvl[m.Sender.QQ.ToInt64()].lock();
            pl[m.Sender.QQ.ToInt64()].lock();
            sent["message"]=s;
            sent["group_id"]=m.Sender.QQ.ToInt64();
			auto res = cpr::Post(cpr::Url{"http://localhost:1234/"},cpr::Body{sent.dump()},cpr::Header{{"Content-Type", "application/json"}});
            */
			msg.Add<PlainMessage>("我不知道呢");
			return msg;
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
			msg.Add<PlainMessage>("出现错误，请稍后再试");
			return msg;
		}
	}
};