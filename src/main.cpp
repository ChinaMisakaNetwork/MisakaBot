#include <iostream>
#include <map>
#include <chrono>
#include <fstream>
#include <mirai.h>
#include <mysql++.h>
#include <cpr/cpr.h>
//#include "minigames.hpp"
#include <mutex>
#include <string>
#include <vector>
#include "administrative.hpp"
#include "deniedwords.hpp"
#include "calc.hpp"
#include "chat.hpp"
#include "getxgp.hpp"
#include "wordcheck.hpp"
#include "hitokoto.hpp"
#include "commands.hpp"
#include "genshin.hpp"
#include "minecraft.hpp"
//#include "watchdog.hpp"
#include "neteasemusic.hpp"
#include "weather.hpp"
#include "record.hpp"
#include "DeepAi.hpp"
#include "bilibili.hpp"
#include "maimai.hpp"
using namespace std;
using namespace Cyan;
map<long long, trie_ac>ACer;
map<long long, bool>wordcheck_list;
mutex ACer_lock, wordcheck_list_lock;
map<long long, mutex>fslock;
map<unsigned long long, chrono::steady_clock::time_point>cooldown;
queue<string>xgplist;
string GenshinlToken, GenshinltUID, GenshinCookieToken, GenshinAccountId;
map<long long, bool>exemptrecord;
MessageChain handle_message(GroupMessage m, db_info dbf, const string& deepai_key, const double& nsfw_value,const string& hypapi,const bool& nsfw_enabled) {
	neteasemusic music;
	chatobj chat(dbf);
	GetXGP xgp;
	weather wt;
	bilibili bz;
	hitokoto htkt;
	recorder rcer;
	maimai mai;
	hypixel hyp(hypapi);
	deepai_picture pic(deepai_key);
	administrative admin(dbf);
	deniedwords dw(dbf);
	calc calculator;
	wordchecker checker(dbf);
	commands cmd(dbf);
	genshin gs(GenshinlToken, GenshinltUID, GenshinCookieToken, GenshinAccountId);
	rcer.handler(m, fslock,exemptrecord);
	MessageChain res;
	res = dw.handler(m, ACer, wordcheck_list, ACer_lock, wordcheck_list_lock);
	if (!res.Empty())return res;
	res = checker.handler(m, ACer, wordcheck_list, deepai_key, nsfw_value,nsfw_enabled);
	if (!res.Empty())return res;
	res = hyp.handler(m);
	if (!res.Empty())return res;
	res = xgp.handler(m, xgplist, cooldown);
	res = admin.handler(m);
	if (!res.Empty()) return res;
	res = mai.handler(m);
	if (!res.Empty())return res;
	res = gs.handler(m);
	if (!res.Empty())return res;
	res = calculator.handler(m);
	if (!res.Empty())return res;
	res = cmd.handler(m);
	if (!res.Empty())return res;
	res = chat.handler(m);
	if (!res.Empty())return res;
	res = htkt.handler(m);
	if (!res.Empty())return res;
	res = music.handler(m);
	if (!res.Empty())return res;
	res = wt.handler(m);
	if (!res.Empty())return res;
	res = bz.handler(m);
	if (!res.Empty())return res;
	res = pic.handler(m);
	if (!res.Empty())return res;
	return {};
}
MessageChain handle_message(FriendMessage m,db_info db_information){
    //watchdog wd(db_information);
    administrative admin(db_information);
    chatobj chater(db_information);
    MessageChain mc;
    //mc = wd.handler(m, report_list, wdl);
    //if (!mc.Empty())return mc;
    mc = admin.handler(m);
    if (!mc.Empty())return mc;
    mc = chater.handler(m);
    if(!mc.Empty())return mc;
    return {};
}
int main(int argc,const char* argv[]) {
reboot:
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif
	system("clear");
    time_t nt;
    time(&nt);
    tm nowdate = *localtime(&nt);
	MiraiBot bot;
	SessionOptions opts;
	ifstream fin;
	double nsfw_value;
	string deepai_key;
	db_info db_information;
	string genshinapi;
	bool nsfw_enabled = false;
	string hypapi;
	string welcomemessage = "";
	fin.open("/etc/MisakaBot/MisakaBot.conf");
	if (!fin.good()) {
		ofstream fout;
		cout << "Conf file isn't exist." << endl;
		fout.open("/etc/MisakaBot/MisakaBot.conf");
		if(!fout.good())system("mkdir /etc/MisakaBot");
		fout.open("/etc/MisakaBot/MisakaBot.conf");
		fout << "BotQQ=12345678" << endl << "HttpHostname=localhost" << endl << "WebSocketHostname=localhost" << endl << "HttpPort=8080" << endl << "WebSocketPort=8080" << endl
			<< "VerifyKey=VerifyKey" << endl << "MySQLdatabase=DatabaseName" << endl << "MySQLaddress=localhost" << endl << "MySQLusername=username" << endl << "MySQLpassword=password" << endl
			<< "MySQLconnectPort=3306" << endl << "入群欢迎词=欢迎词" << endl << "DeepaiKey=" << endl << "nsfw_value=0.75" << endl << "nsfw_enabled=false" << endl
			<< "GenshinlToken=" << endl << "GenshinltUID=" << endl << "GenshinCookieToken=" << endl << "GenshinAccountId=" << endl << "HypixelAPIKey="<<endl<<"ChatRecordExempt=";
		cout << "Conf created at /etc/MisakaBot/MisakaBot.conf , please edit it and restart the Bot." << endl;
		return 1;
	}
	vector<string>conf;
	string temp;
	while (fin >> temp) {
		for (auto i = temp.begin(); i != temp.end(); ++i) {
			if (*i == '=') {
				conf.push_back(string(i+1,temp.end()));
			}
		}
	}
	fin.close();
	fin.open("/etc/MisakaBot/xgp_account.txt");
	while (fin >> temp) {
		xgplist.push(temp);
	}
	fin.close();
	opts.BotQQ = QQ_t((stoi(conf[0])));
	opts.HttpHostname = conf[1];
	opts.WebSocketHostname = conf[2];
	opts.HttpPort = stoi(conf[3]);
	opts.WebSocketPort = stoi(conf[4]);
	opts.VerifyKey = conf[5];
	opts.ThreadPoolSize = 6;
	db_information.db_name = conf[6];
	db_information.db_addr = conf[7];
	db_information.db_username = conf[8];
	db_information.db_password = conf[9];
	db_information.port = atoi(conf[10].c_str());
	welcomemessage = conf[11];
	deepai_key = conf[12];
	nsfw_value = atof(conf[13].c_str());
	if (conf[14] == "true")nsfw_enabled = true;
	GenshinlToken = conf[15];
	GenshinltUID = conf[16];
	GenshinCookieToken = conf[17];
	GenshinAccountId = conf[18];
	hypapi = conf[19];
	//string temp;
	while (getline(stringstream(conf[20]), temp, ',')) {
		if (temp.empty())continue;
		exemptrecord[stol(temp)] = 1;
	}
	wordchecker wordchecker_init(db_information);
	wordchecker_init.init(ACer,wordcheck_list,ACer_lock,wordcheck_list_lock);
	while (true){
		try{
			cout << "尝试连接指定的Bot…" << endl;
			bot.Connect(opts);
			break;
		}
		catch (const std::exception& ex){
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "连接成功，正在运行中…" << endl;
	bot.On<GroupMessage>(
		[&](GroupMessage m) {
			try {
				MessageChain mc = handle_message(m, db_information, deepai_key, nsfw_value, hypapi, nsfw_enabled);
				if(!mc.Empty()) {
					MessageChain reply;
					reply.At(m.Sender.QQ);
					reply = reply + mc;
					bot.SendMessage(m.Sender.Group.GID, reply);
				}
			}
			catch (const exception& ex) {
				cerr << ex.what() << endl;
			}
		});
	bot.On<NewFriendRequestEvent>([&](NewFriendRequestEvent e) {
		e.Accept();
		});
	bot.On<BotInvitedJoinGroupRequestEvent>([&](BotInvitedJoinGroupRequestEvent e) {
		e.Accept();
		});
	bot.On<MemberJoinEvent>([&](MemberJoinEvent m) {
			try {
				if (welcomemessage.empty())return;
				MessageChain mc;
				mc.At(m.NewMember.QQ);
				mc.Add<PlainMessage>(welcomemessage);
				bot.SendMessage(m.NewMember.Group.GID, mc);
			}catch(...){}
		});
	bot.On<FriendMessage>(
		[&](FriendMessage m) {
			try {
                MessageChain mc=handle_message(m,db_information);
                if(!mc.Empty())bot.SendMessage(m.Sender.QQ,mc);
			}
			catch (...) {}
		});
    bot.On<MemberLeaveEventQuit>([&](MemberLeaveEventQuit m){
        try{
            if(m.Member.SpecialTitle.empty())bot.SendMessage(m.Member.Group.GID,MessageChain().Plain(m.Member.MemberName+"("+to_string(m.Member.QQ.ToInt64())+")"+"离开了"));
            bot.SendMessage(m.Member.Group.GID,MessageChain().Plain("["+m.Member.SpecialTitle+"]"+m.Member.MemberName+"("+to_string(m.Member.QQ.ToInt64())+")"+"离开了"));
        }catch(...){}
    });
    bot.On<MemberLeaveEventKick>([&](MemberLeaveEventKick m){
        try{
            if(m.Member.SpecialTitle.empty())bot.SendMessage(m.Member.Group.GID,MessageChain().Plain(m.Member.MemberName+"("+to_string(m.Member.QQ.ToInt64())+")"+"被踢了"));
            bot.SendMessage(m.Member.Group.GID,MessageChain().Plain("["+m.Member.SpecialTitle+"]"+m.Member.MemberName+"("+to_string(m.Member.QQ.ToInt64())+")"+"被踢了"));
        }catch(...){}
    });
	bot.On<LostConnection>([&](LostConnection e){
			cout << e.ErrorMessage << " (" << e.Code << ")" << endl;
			while (true)
			{
				try{
					cout << "尝试重新连接，请稍候…" << endl;
					bot.Reconnect();
					cout << "重新连接成功" << endl;
					break;
				}
				catch (const std::exception& ex){
					cout << ex.what() << endl;
				}
				MiraiBot::SleepSeconds(1);
			}
		});
	string cmd;
	vector<string>commands;
	while (getline(cin,cmd)){
		stringstream sin(cmd);
		string temp = "";
		while (getline(sin, temp, ' ')) {
			commands.push_back(temp);
		}
		if (commands.size() == 0)goto cont;
		if (cmd == "/stop" || cmd=="/exit") {
			bot.Disconnect();
			ofstream fout;
			fout.open("etc/MisakaBot/xgp_account.txt");
			while (!xgplist.empty()) {
				fout << xgplist.front() << endl;
				xgplist.pop();
			}
			break;
		}
		if (cmd == "/reboot" || cmd=="/restart") {
			system("clear");
			bot.Disconnect();
			commands.clear();
			goto reboot;
		}
		if (*commands.begin() == "/reloadxgp") {
			while (!xgplist.empty()) {
				xgplist.pop();
			}
			ifstream fin;
			fin.open("/etc/MisakaBot/xgp_account.txt");
			while (fin >> temp) {
				xgplist.push(temp);
			}
			fin.close()
		}
		if (*commands.begin() == "/queryxgp") {
			cout << xgplist.size() << endl;
		}
		if (*commands.begin() == "添加敏感词") {
			if (commands.size() != 3) {
				cout << "格式错误" << endl;
				goto cont;
			}
			DatabaseOperator db(db_information);
			db.query.reset();
			string s = commands[2];
			db.query << "insert into deniedwords(groupid, word) values (%0q,%1q)";
			db.query.parse();
			mysqlpp::SimpleResult res = db.query.execute(commands[1], s);
			if (string(res.info()).empty()) {
				cout << "已添加" << endl;
			}
			wordchecker checker(db_information);
			checker.init(ACer, wordcheck_list, ACer_lock, wordcheck_list_lock);
		}
		if (*commands.begin() == "删除敏感词") {
			if (commands.size() != 3) {
				cout << "格式错误" << endl;
				goto cont;
			}
			DatabaseOperator db(db_information);
			db.query.reset();
			string s = commands[2];
			db.query << "delete from deniedwords where groupid = %0q and word = %1q";
			db.query.parse();
			mysqlpp::SimpleResult res = db.query.execute(commands[1], s);
			if (string(res.info()).empty()) {
				cout << "已添加" << endl;
			}
			wordchecker checker(db_information);
			checker.init(ACer, wordcheck_list, ACer_lock, wordcheck_list_lock);
		}
		if (*commands.begin() == "/op") {
			if (commands.size() != 3) {
				cout << "格式错误" << endl;
				goto cont;
			}
			DatabaseOperator db(db_information);
			string res = db.grantperm(atoll(commands[1].c_str()), atoll(commands[2].c_str())).GetPlainText();
			cout << res << endl;
		}
		if (*commands.begin() == "/deop") {
			if (commands.size() != 3) {
				cout << "格式错误" << endl;
				goto cont;
			}
			DatabaseOperator db(db_information);
			string res = db.deperm(atoll(commands[1].c_str()), atoll(commands[2].c_str())).GetPlainText();
			cout << res << endl;
		}
	cont:;
		commands.clear();
	}
	return 0;
}