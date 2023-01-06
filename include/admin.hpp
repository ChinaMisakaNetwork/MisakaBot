#pragma once
#include <mysql++.h>
#include <mirai.h>
#include <string>
using namespace std;
using namespace Cyan;
struct db_info {
	string db_addr, db_username, db_password;
	string db_name;
	int port;
};
class permchecker {
protected:
	string db_addr,db_name;
	int port;
	string username;
	string pwd;
	bool connected = false;
	mysqlpp::Connection conn;
public:
	mysqlpp::Query query = conn.query();
	explicit permchecker(db_info dbinf) {
		db_name = dbinf.db_name;
		db_addr = dbinf.db_addr;
		port = dbinf.port;
		username = dbinf.db_username;
		pwd = dbinf.db_password;
		connected = conn.connect(db_name.c_str(), db_addr.c_str(), username.c_str(), pwd.c_str(), port);
		query = conn.query();
	}
	bool checkperm(const long long& groupid, const long long& qq) {
		query.reset();
		query << "select * from qqadmin where groupid = %0q and adminqq= %1q";
		query.parse();
		const mysqlpp::StoreQueryResult res = query.store(groupid, qq);
		return !res.empty();
	}
	MessageChain grantperm(const long long& groupid, const long long& qq) {
		query.reset();
		query << "insert into qqadmin(groupid , adminqq) values (%0q,%1q)";
		query.parse();
		const mysqlpp::SimpleResult res=query.execute(groupid,qq);
		MessageChain result;
		if (string(res.info()).empty()) {
			result.Add<PlainMessage>("已添加");
			return result;
		}
		cout << string(res.info()) << endl;
		result.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
		return result;
	}
	MessageChain deperm(const long long& groupid, const long long& qq) {
		query.reset();
		query << "delete from qqadmin where groupid = %0q and adminqq = %1q";
		query.parse();
		const mysqlpp::SimpleResult res = query.execute(groupid, qq);
		MessageChain msg;
		if (string(res.info()).empty()) {
			msg.Add<PlainMessage>("已删除");
			return msg;
		}
		cout << string(res.info()) << endl;
		msg.Add<PlainMessage>("出现错误，请查看终端以获取详细信息");
		return msg;
	}
};