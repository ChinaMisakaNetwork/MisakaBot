#include <iostream>
#include <map>
#include <fstream>
#include <mirai.h>
#include <mysql++.h>
#include <mutex>
#include "administrative.hpp"
#include "deniedwords.hpp"
#include "calc.hpp"
#include "chat.hpp"
#include "wordcheck.hpp"
#include "hitokoto.hpp"
#include "commands.hpp"
#include "neteasemusic.hpp"
#include "weather.hpp"
#include "record.hpp"
#include "bilibili.hpp"
using namespace std;
using namespace Cyan;
map<int, TrieAc>ACer;
map<int, bool>wordcheck_list;
mutex ACer_lock, wordcheck_list_lock;
map<int, mutex>fslock;
string handle_message(GroupMessage m,MiraiBot& bot,db_info dbf,GroupImage& img) {
	administrative admin(dbf);
	deniedwords dw(bot, dbf);
	neteasemusic music;
	chatobj chat(dbf);
	weather wt;
	calc calculator;
	bilibili bz;
	commands cmd(dbf);
	wordchecker checker(dbf);
	hitokoto htkt;
	recorder rcer;
	rcer.handler(m, fslock);
	string res = "";
	res = admin.handler(m);
	if (!res.empty())return res;
	res = dw.handler(m);
	if (!res.empty()) {
		checker.init(ACer, wordcheck_list, ACer_lock, wordcheck_list_lock);
		return res;
	}
	res = calculator.handler(m);
	if (!res.empty())return res;
	res = cmd.handler(m);
	if (!res.empty())return res;
	res = checker.handler(m,ACer,wordcheck_list);
	if (!res.empty())return res;
	res = chat.handler(m, img);
	if (!res.empty())return res;
	res = htkt.handler(m,img);
	if (!res.empty())return res;
	res = music.handler(m);
	if (!res.empty())return res;
	res = wt.handler(m);
	if (!res.empty())return res;
	res = bz.handler(m);
	if (!res.empty())return res;
	return "";
}
int main(int argc,const char* argv[]) {
reboot:
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif
	MiraiBot bot;
	SessionOptions opts;
	ifstream fin;
	ofstream fout;
	db_info db_information;
	string welcomemessage = "";
	fin.open("/etc/MisakaBot/MisakaBot.conf");
	if (!fin.good()) {
		cout << "Conf file isn't exist." << endl;
		fout.open("/etc/MisakaBot/MisakaBot.conf");
		if(!fout.good())system("mkdir /etc/MisakaBot");
		fout.open("/etc/MisakaBot/MisakaBot.conf");
		fout << "BotQQ=12345678" << endl << "HttpHostname=localhost" << endl << "WebSocketHostname=localhost" << endl << "HttpPort=8080" << endl << "WebSocketPort=8080" << endl << "VerifyKey=VerifyKey" << endl << "MySQLdatabase=DatabaseName" <<endl << "MySQLaddress=localhost" << endl << "MySQLusername=username" << endl << "MySQLpassword=password" << endl << "MySQLconnectPort=3306"<<endl<<"入群欢迎词=欢迎词";
		cout << "Conf created at /etc/MisakaBot/MisakaBot.conf , please edit it and restart the Bot." << endl;
		return 1;
	}
	else {
		vector<string>conf;
		string temp;
		while (fin >> temp) {
			for (auto i = temp.begin(); i != temp.end(); ++i) {
				if (*i == '=') {
					conf.push_back(string(i+1,temp.end()));
					continue;
				}
			}
		}
		opts.BotQQ = QQ_t((stoi(conf[0])));				// 请修改为你的机器人QQ
		opts.HttpHostname = conf[1];		// 请修改为和 mirai-api-http 配置文件一致
		opts.WebSocketHostname = conf[2];	// 同上
		opts.HttpPort = stoi(conf[3]);					// 同上
		opts.WebSocketPort = stoi(conf[4]);				// 同上
		opts.VerifyKey = conf[5];			// 同上
		db_information.db_name = conf[6];
		db_information.db_addr = conf[7];
		db_information.db_username = conf[8];
		db_information.db_password = conf[9];
		db_information.port = atoi(conf[10].c_str());
		welcomemessage = conf[11];
		wordchecker wordchecker_init(db_information);
		wordchecker_init.init(ACer,wordcheck_list,ACer_lock,wordcheck_list_lock);
	}
	while (true){
		try{
			cout << "Trying to establish the connection..." << endl;
			bot.Connect(opts);
			break;
		}
		catch (const std::exception& ex){
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "Bot Working..." << endl;
	bot.On<GroupMessage>(
		[&](GroupMessage m){
			try{
				string result = "";
				MessageChain mc;
				GroupImage img;
				VoiceMessage vc;
				result = handle_message(m, bot, db_information, img);
				if (!result.empty()) {
					mc.At(m.Sender.QQ);
					mc.Add<PlainMessage>(result);
					mc.Image(img);
					bot.SendMessage(m.Sender.Group.GID, mc);
					goto next;
				}
			}
			catch (const std::exception& ex){
				cout << ex.what() << endl;
			}
		next:;
		});
	bot.On<NewFriendRequestEvent>([&](NewFriendRequestEvent e) {
		e.Accept();
		});
	bot.On<BotInvitedJoinGroupRequestEvent>([&](BotInvitedJoinGroupRequestEvent e) {
		e.Accept();
		});
	bot.On<MemberJoinEvent>([&](MemberJoinEvent m) {
			try {
				MessageChain mc;
				mc.At(m.NewMember.QQ);
				mc.Add<PlainMessage>(welcomemessage);
				bot.SendMessage(m.NewMember.Group.GID, mc);
			}catch(...){}
		});
	bot.On<LostConnection>([&](LostConnection e){
			cout << e.ErrorMessage << " (" << e.Code << ")" << endl;
			while (true)
			{
				try{
					cout << "Trying to establish the connection..." << endl;
					bot.Reconnect();
					cout << "Reconnected." << endl;
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
		if (cmd == "exit") {
			bot.Disconnect();
			break;
		}
		if (cmd == "reboot") {
			bot.Disconnect();
			commands.clear();
			goto reboot;
		}
		if (*commands.begin() == "添加敏感词") {
			if (commands.size() != 3) {
				cout << "格式错误" << endl;
				goto cont;
			}
			permchecker db(db_information);
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
			permchecker db(db_information);
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
		if (*commands.begin() == "添加管理员") {
			if (commands.size() != 3) {
				cout << "格式错误" << endl;
				goto cont;
			}
			permchecker db(db_information);
			string res = db.grantperm(atoi(commands[1].c_str()), atoi(commands[2].c_str()));
			cout << res << endl;
		}
	cont:;
		commands.clear();
	}
	return 0;
}