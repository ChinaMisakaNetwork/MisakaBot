#pragma once
#include <mysql++.h>
#include <mirai.h>
#include <string>
#include <mutex>
using namespace std;
using namespace Cyan;
struct db_info {
	string db_addr, db_username, db_password;
	string db_name;
	int port;
};
class DatabaseOperator {
protected:
	string db_addr,db_name;
	int port;
	string username;
	string pwd;
	bool connected = false;
	mysqlpp::Connection conn;
public:
	mysqlpp::Query query = conn.query();
	explicit DatabaseOperator(db_info dbinf) {
		db_name = dbinf.db_name;
		db_addr = dbinf.db_addr;
		port = dbinf.port;
		username = dbinf.db_username;
		pwd = dbinf.db_password;
		connected = conn.connect(db_name.c_str(), db_addr.c_str(), username.c_str(), pwd.c_str(), port);
		query = conn.query();
	}
    ~DatabaseOperator() {
        query.clear();
        conn.disconnect();
    }
	bool checkperm(const long long& groupid, const long long& qq) {
        mysqlpp::StoreQueryResult res;
        retry:
		query.reset();
		query << "select * from qqadmin where groupid = %0q and adminqq= %1q";
		query.parse();
        try {
            res = query.store(groupid, qq);
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            goto retry;
        }
        
		return !res.empty();
	}
	MessageChain grantperm(const long long& groupid, const long long& qq) {
		query.reset();
        mysqlpp::SimpleResult res;
		query << "insert into qqadmin(groupid , adminqq) values (%0q,%1q)";
		query.parse();
        try {
            res = query.execute(groupid, qq);
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            return MessageChain().Plain("发生错误");
        }
		if (string(res.info()).empty()) {
            
			return MessageChain().Plain("已添加");
		}
        
		cerr << string(res.info()) << endl;
		return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
	}
	MessageChain deperm(const long long& groupid, const long long& qq) {
		query.reset();
		query << "delete from qqadmin where groupid = %0q and adminqq = %1q";
		query.parse();
        mysqlpp::SimpleResult res;
        try {
            res = query.execute(groupid, qq);
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            return MessageChain().Plain("发生错误");
        }
		if (string(res.info()).empty()) {
			return MessageChain().Plain("已删除");
		}
		cerr << string(res.info()) << endl;
		return MessageChain().Plain("出现错误，请查看终端以获取详细信息");
	}
};