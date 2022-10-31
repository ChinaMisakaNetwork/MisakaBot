#include <mirai.h>
#include <string>
#include <cpr/cpr.h>
using namespace std;
using namespace Cyan;
class bilibili {
private:
protected:
public:
	string handler(GroupMessage m) {
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>cmds;
		string temp;
		while (getline(sin, temp, ' ')) {
			cmds.push_back(temp);
		}
		if (cmds.size() == 0)return "";
		if(*cmds.begin()=="bilibili����"|| *cmds.begin()=="������������" || *cmds.begin()=="Bվ����") {
			if(cmds.size()==2) {
				if(cmds[1]=="������Ƶ") {
					auto res = cpr::Get(cpr::Url{ "https://api.bilibili.com/x/web-interface/popular?ps=1&pn=1" });
					json data = json::parse(res.text);
					json reply = data["data"]["list"][0];
					string ans = "";
					ans += "������";
					ans += reply["tname"].get<string>();
					ans += '\n';
					ans += "���⣺";
					ans += reply["title"].get<string>();
					ans += '\n';
					ans += "��飺";
					ans += reply["desc"].get<string>();
					ans += "��Ƶ���ӣ�";
					ans += reply["short_link_v2"].get<string>();
					return ans;
				}
				if(cmds[1][0]=='B' && cmds[1][1]=='V') {
					auto res = cpr::Get(cpr::Url{ "https://api.bilibili.com/x/web-interface/view?bvid=" + cmds[1] });
					json reply = json::parse(res.text)["data"];
					string ans = "";
					ans += "������";
					ans += reply["tname"].get<string>();
					ans += '\n';
					ans += "���⣺";
					ans += reply["title"].get<string>();
					ans += '\n';
					ans += "��飺";
					ans += reply["desc"].get<string>();
					return ans;
				}
				return "��ʽ������������";
			}
			if(cmds.size()==3) {
				if(cmds[1]=="���û�") {
					auto res = cpr::Get(cpr::Url{"https://api.bilibili.com/x/space/acc/info?mid="+cmds[2]});
					json reply = json::parse(res.text)["data"];
					string ans = "";
					ans += "�ǳƣ�";
					ans += reply["name"].get<string>();
					ans += '\n';
					ans += "�Ա�";
					ans += reply["sex"].get<string>();
					ans += '\n';
					ans += "�ȼ���";
					ans += to_string(reply["level"].get<int>());
					ans += '\n';
					ans += "Ӳ�Ҹ�����";
					ans += to_string(reply["coins"].get<int>());
					ans += '\n';
					ans += "���գ�";
					ans += reply["birthday"].get<string>();
					return ans;
				}
			}
			return "��ʽ������������";
		}
		return "";
	}
};