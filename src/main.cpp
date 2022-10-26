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
using namespace std;
using namespace Cyan;
map<int, TrieAC>ACer;
map<int, bool>wordcheck_list;
mutex ACer_lock, wordcheck_list_lock;
string handle_message(GroupMessage m,MiraiBot& bot,db_info dbf,GroupImage& img) {
	administrative admin(dbf);
	deniedwords dw(bot, dbf);
	chatobj chat(dbf);
	calc calculator;
	commands cmd(dbf);
	wordchecker checker(dbf);
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
	fin.open("/etc/MisakaBot/MisakaBot.conf");
	if (!fin.good()) {
		cout << "Conf file isn't exist." << endl;
		fout.open("/etc/MisakaBot/MisakaBot.conf");
		if(!fout.good())system("mkdir /etc/MisakaBot");
		fout.open("/etc/MisakaBot/MisakaBot.conf");
		fout << "BotQQ=12345678" << endl << "HttpHostname=localhost" << endl << "WebSocketHostname=localhost" << endl << "HttpPort=8080" << endl << "WebSocketPort=8080" << endl << "VerifyKey=VerifyKey" << endl << "admin_database=DatabaseName" <<endl<<"chat_database=DatabaseName"<<endl<<"denied_database=DatabaseName"<< endl << "MySQLaddress=localhost" << endl << "MySQLusername=username" << endl << "MySQLpassword=password" << endl << "MySQLconnectPort=3306";
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
		db_information.db_admin = conf[6];
		db_information.db_chat = conf[7];
		db_information.db_denied = conf[8];
		db_information.db_addr = conf[9];
		db_information.db_username = conf[10];
		db_information.db_password = conf[11];
		db_information.port = atoi(conf[12].c_str());
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
				result = handle_message(m, bot, db_information, img);
				cout << result;
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
	while (cin >> cmd){
		if (cmd == "exit"){
			bot.Disconnect();
			break;
		}
		if (cmd == "reboot") {
			bot.Disconnect();
			goto reboot;
		}
	}
	return 0;
}