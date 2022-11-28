#pragma once
#include <mirai.h>
#include <fstream>
#include <ctime>
#include <cpr/cpr.h>
using namespace std;
using namespace Cyan;
class recorder{
	ofstream fs;
	ofstream pfs;
public:
	void handler(GroupMessage m, map<long long, mutex>& fslock)
	{
		const string s = m.MessageChain.GetPlainText();
		fslock[m.Sender.Group.GID.ToInt64()].lock();
		fs.open("chatrecord/" + to_string(m.Sender.Group.GID.ToInt64()) + ".txt",ios::app);
		if (!fs.good()){
			system("mkdir chatrecord/");
			fs.open("chatrecord/" + to_string(m.Sender.Group.GID.ToInt64()) + ".txt");
		}
		pfs.open("chatrecord/picture/checker.txt");
		if (!pfs.good()) {
			system("mkdir chatrecord/picture/");
		}
		pfs.close();
		const time_t lol = m.Timestamp();
		const tm sendtime = *localtime(&lol);
		fs << "消息ID：" << to_string(m.MessageId()) << to_string(sendtime.tm_year + 1900) + "年" + to_string(sendtime.tm_mon + 1) + "月" + to_string(sendtime.tm_mday) + "日" + to_string(sendtime.tm_hour) + "时" + to_string(sendtime.tm_min) + "分" + to_string(sendtime.tm_sec) + "秒 ::::" << " 发送者：" << m.Sender.QQ.ToInt64() << " 内容：" << s << "\n";
		vector<ImageMessage>mc = m.MessageChain.GetAll<ImageMessage>();
		if(!mc.empty()) {
			fs << "图片ID: ";
			for(ImageMessage i:mc) {
				fs << i.Id() << ' ';
				pfs.open("chatrecord/picture/" + (i.Id()),ios::binary);
				auto res = cpr::Get(cpr::Url{ i.Url() });
				pfs << res.text;
				pfs.close();
			}
			fs << endl;
		}
		fs.close();
		fslock[m.Sender.Group.GID.ToInt64()].unlock();
	}
};