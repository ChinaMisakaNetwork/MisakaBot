#pragma once
#include <mysql++.h>
#include <ssqls.h>
#include <fstream>
#include <string>
using namespace std;
struct db_info {
	string db_addr, db_username, db_password;
	string db_admin, db_chat, db_denied;
	int port;
};
class permchecker {
protected:
	string db_addr,db_name;
	int port;
	string username;
	string pwd;
	bool connected = 0;
	mysqlpp::Connection conn;
	mysqlpp::Query query=conn.query();
public:
	permchecker(db_info dbinf) {
		db_name = dbinf.db_admin;
		db_addr = dbinf.db_addr;
		port = dbinf.port;
		username = dbinf.db_username;
		pwd = dbinf.db_password;
		connected = conn.connect(db_name.c_str(), db_addr.c_str(), username.c_str(), pwd.c_str(), port);
		query = conn.query();
	}
	void change_db_name(string newname) {
		conn.disconnect();
		conn.connect(newname.c_str(), db_addr.c_str(), username.c_str(), pwd.c_str(), port);
		query = conn.query();
	}
	bool checkperm(const int& groupid, const int& qq) {
		query << "select * from qqadmin where groupid =" << mysqlpp::quote << to_string(groupid) << " and adminqq =" << mysqlpp::quote << to_string(qq);
		mysqlpp::SimpleResult res=query.execute();
		if ((!res) || res.rows() < 0) {
			return false;
		}
		return true;
	}
	string grantperm(const int& groupid, const int& qq) {
		query << "insert into qqadmin(groupid,adminqq) values (" << mysqlpp::quote << to_string(groupid) << "," << mysqlpp::quote << to_string(qq) << ")";
		mysqlpp::SimpleResult res=query.execute();
		if (string(res.info()).empty()) {
			return "Added.";
		}
		cout << string(res.info()) << endl;
		return "出现错误，请查看终端以获取详细信息";
	}
};