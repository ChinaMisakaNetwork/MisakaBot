#pragma once
#include <mirai.h>
#include "admin.hpp"
#include <mysql++.h>
#include <vector>
#include <fstream>
#include <cpr/cpr.h>
using namespace std;
using namespace Cyan;
class chatobj :public permchecker{
private:
	db_info dbinfo;
protected:

public:
	chatobj(db_info dbf):permchecker(dbf) {
		dbinfo = dbf;
	}
	MessageChain handler(GroupMessage m) {
		MessageChain msg;
		if (!m.AtMe())return msg;
		try {
			string s = m.MessageChain.GetPlainText();
			s.erase(s.begin());
			query.reset();
			query << "select * from miraichat where %0q regexp question order by char_length(question) desc";
			query.parse();
			mysqlpp::StoreQueryResult sres = query.store(s);
			if (sres.size()>0) {
				string ans = "";
				if (sres[0]["have_img"].to_string(ans),ans=="1") {
					sres[0]["img_url"].to_string(ans);
					msg.Image(m.GetMiraiBot().UploadGroupImage(ans,true));
				}
				sres[0]["answer"].to_string(ans);
				msg.Add<PlainMessage>(ans);
				return msg;
			}
			s = "https://api.qingyunke.com/api.php?key=free&appid=0&msg=" + s;
			auto res = cpr::Get(cpr::Url{s});
			json reply = json::parse(res.text);
			msg.Add<PlainMessage>(reply["content"].get<string>());
			return msg;
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
			msg.Add<PlainMessage>("出现错误，请稍后再试");
			return msg;
		}
		return msg;
	}
};