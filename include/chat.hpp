#include <mirai.h>
#include "admin.hpp"
#include <mysql++.h>
#include <vector>
#include <fstream>
#include "mirai/third-party/httplib.h"
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
	string handler(GroupMessage m,GroupImage& img) {
		if (!m.AtMe())return "";
		try {
			string s = m.MessageChain.GetPlainText();
			change_db_name(dbinfo.db_chat);
			query.reset();
			query << "select * from miraichat where %0q regexp question order by char_length(question) desc";
			query.parse();
			mysqlpp::StoreQueryResult sres = query.store(s);
			if (sres.size()>0) {
				string ans = "";
				
				if (sres[0]["have_img"].to_string(ans),ans=="1") {
					sres[0]["img_url"].to_string(ans);
					img=m.GetMiraiBot().UploadGroupImage(ans,true);
				}
				sres[0]["answer"].to_string(ans);
				return ans;
			}
			httplib::Client QYK_API("http://api.qingyunke.com");
			s = "/api.php?key=free&appid=0&msg=" + s;
			auto res = QYK_API.Get(s.c_str());
			return string(res->body.begin() + 23, res->body.end() - 2);
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
			return "出现错误，请查看终端以获取详细信息";
		}
	}
};