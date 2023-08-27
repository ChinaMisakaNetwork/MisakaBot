#include <mirai.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iostream>
#include <queue>
#include "admin.hpp"
using namespace std;
using namespace Cyan;
class GetXGP :public DatabaseOperator{
public:
	MessageChain handler(GroupMessage m,queue<string> xgplist,map<unsigned long long,chrono::steady_clock::time_point> cooldown) {
		stringstream sin(m.MessageChain.GetPlainText());
		MessageChain msg;
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return msg;
		if (cmds[0] == "/getxgp") {
			if (chrono::duration_cast<chrono::minutes>(chrono::steady_clock::now() - cooldown[m.Sender.QQ.ToInt64()]).count() < 5) {
				return MessageChain().Plain("��ȴʱ�仹��" + to_string(chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - cooldown[m.Sender.QQ.ToInt64()]).count()) + "��Ŷ~");
			}
			m.GetMiraiBot().SendMessage(m.Sender.Group.GID, m.Sender.QQ, MessageChain().Plain(get_xgp(xgplist)));
			return MessageChain().Plain("�Ѿ�˽�ķ�������Ƥ����~");
		}
	}
	MessageChain handler(FriendMessage m, queue<string> xgplist) {
		stringstream sin(m.MessageChain.GetPlainText());
		MessageChain msg;
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return msg;
		if (cmds[0] == "/restockxgp") {
			if (!checkperm(1111, m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("��û��Ȩ�ޣ�");
			}
			for (auto i = cmds.begin() + 1; i != cmds.end(); ++i) {
				xgplist.push(*i);
			}
			return MessageChain().Plain("�����ɹ�����ʣ��XGP" + to_string(xgplist.size()) + "��");
		}
		if (cmds[0] == "/savexgp") {
			if (!checkperm(1111, m.Sender.QQ.ToInt64())) {
				return MessageChain().Plain("��û��Ȩ�ޣ�");
			}
			ofstream fout;
			fout.open("/etc/MisakaBot/xgp_account.txt");
			while (!xgplist.empty()) {
				fout << xgplist.front() << endl;
				xgplist.pop();
			}
			fout.close();
		}
	}
private:
	string get_xgp(queue<string> xgplist) {
		string res = xgplist.front();
		xgplist.pop();
		return res;
	}
};