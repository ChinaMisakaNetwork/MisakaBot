#include <mirai.h>
#include <cpr/cpr.h>
#include "md5.hpp"
#include <string>
#include <ctime>
#include <vector>
#include <random>
using namespace std;
using namespace Cyan;
class genshin {
private:
	string ltk, ltuid, ckt, accid;
	string translate(const string&base) {
		if (base == "Electro")return "雷";
		if (base == "Pyro")return "火";
		if (base == "Anemo")return "风";
		if (base == "Geo")return "岩";
		if (base == "Cryo")return "冰";
		if (base == "Hydro")return "水";
		if (base == "Dendro")return "草";
		return "未知";
	}
	string getserver(const string& uid) {
		if (uid[0] == '1' || uid[0] == '2')return "cn_gf01";
		return "cn_qd01";
	}
	int getrand(int L,int R) {
		mt19937 rnd(time(nullptr));
		const int res = static_cast<int>((1.0 * rnd() / UINT_MAX) * (R - L + 1)) + L;
		return res;
	}
	cpr::Header getheader(string cookie,string body,string query) {
		cpr::Header heads{ {"x-rpc-app_version","2.37.1"},{"x-requested_with","com.mihoyo.hyperion"}, {"x-rpc-client_type","5"},{"Cookie",cookie},{"ds",get_ds(body,query)},{"origin","https://webstatic.mihoyo.com"},{"Referer","https://webstatic.mihoyo.com"},{"user-agent","Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) miHoYoBBS/2.13.1)"}};
		return heads;
	}
	const string salt = "xV8v4Qu54lUKrEYFZkJhB8cuOh9Asafs";
	string get_ds(string body,string query) {
		if(!query.empty())query.erase(query.begin());
		string ts = to_string(time(nullptr));
		const int rnd = getrand(100000, 999999);
		const string get_ds = "salt=" + salt + "&t=" + ts + "&r=" + to_string(rnd) + "&b=" + body + "&q=" + query;
		return ts + ',' + to_string(rnd) + ',' + md5(get_ds);
	}
public:
	genshin(const string&ltk,const string&ltuid,const string&ckt,const string&accid) {
		this->ltk = ltk;
		this->ltuid = ltuid;
		this->ckt = ckt;
		this->accid = accid;
	}
	MessageChain handler(GroupMessage m) {
		stringstream sin((m.MessageChain.GetPlainText()));
		vector<string>commands;
		MessageChain msg;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.size() == 0)return MessageChain();
		if(*commands.begin()=="原神") {
			try {
				if (commands[1] == "查首页") {
					string querycmd = "?role_id=" + commands[2] + "&server=" + getserver(commands[2]);
					auto res = cpr::Get(cpr::Url{ "https://api-takumi-record.mihoyo.com/game_record/app/genshin/api/index"+querycmd},getheader("ltoken="+ltk+";ltuid="+ltuid+";cookie_token="+ckt+";account_id="+accid, "", querycmd));
					json data = json::parse(res.text)["data"];
					json reply = data["avatars"];
					msg.Add<PlainMessage>("结果如下：\n昵称为" + data["role"]["nickname"].get<string>() + "的旅行者现有角色" + to_string(data["stats"]["avatar_number"].get<int>())+"个，分别为：\n");
					for (auto i = reply.begin(); i != reply.end();++i) {//Get Avatars
						msg.Add<PlainMessage>("角色：" + i->at("name").get<string>() + '\n');
						msg.Add<PlainMessage>("		元素：" + translate(i->at("element").get<string>()) + '\n');
						msg.Add<PlainMessage>("		星级：" + to_string(i->at("rarity").get<int>()) + '\n');
						msg.Add<PlainMessage>("		等级：" + to_string(i->at("level").get<int>()) + '\n');
						msg.Add<PlainMessage>("		命之座：" + to_string(i->at("actived_constellation_num").get<int>()) + '\n');
					}
					msg.Add<PlainMessage>("深渊层数：" + data["stats"]["spiral_abyss"].get<string>() + '\n');
					msg.Add<PlainMessage>("激活的传送锚点：" + to_string(data["stats"]["way_point_number"].get<int>()) + '\n');
					return msg;
				}
				if(commands[1]=="查角色") {
					auto res = cpr::Post(cpr::Url{ "https://api-takumi-record.mihoyo.com/game_record/app/genshin/api/character"},cpr::Payload{{"role_id",commands[2]},{"server",getserver(commands[2])}},getheader("ltoken=" + ltk + ";ltuid=" + ltuid + ";cookie_token=" + ckt + ";account_id=" + accid,"{role_id="+commands[2]+","+"server="+getserver(commands[2]),""));
					json data = json::parse(res.text)["data"];
					json reply = data["avatars"];
					msg.Add<PlainMessage>("结果如下：\n昵称为" + data["role"]["nickname"].get<string>() + "的旅行者现有角色" + to_string(data["stats"]["avatar_number"].get<int>()) + "个，分别为：\n");
					for (auto i = reply.begin(); i != reply.end(); ++i) {
						msg.Add<PlainMessage>("角色：" + i->at("name").get<string>() + '\n');
						msg.Add<PlainMessage>("		元素：" + translate(i->at("element").get<string>()) + '\n');
						msg.Add<PlainMessage>("		星级：" + to_string(i->at("rarity").get<int>()) + '\n');
						msg.Add<PlainMessage>("		等级：" + to_string(i->at("level").get<int>()) + '\n');
						msg.Add<PlainMessage>("		命之座：" + to_string(i->at("actived_constellation_num").get<int>()) + '\n');
						msg.Add<PlainMessage>("		圣遗物：\n");
					for(auto j=i->at("reliquaries").begin();j!= i->at("reliquaries").end();++i) {
						msg.Add<PlainMessage>("				" + j->at("name").get<string>() + '\n');
						msg.Add<PlainMessage>("						等级：" + to_string(j->at("level").get<int>()) + '\n');
						msg.Add<PlainMessage>("						星级：" + to_string(j->at("rarity").get<int>()) + '\n');
					}
						msg.Add<PlainMessage>("		武器：\n");
						msg.Add<PlainMessage>("				" + i->at("weapon")["name"].get<string>() + '\n');
						msg.Add<PlainMessage>("						等级：" + to_string(i->at("weapon")["level"].get<int>()) + '\n');
						msg.Add<PlainMessage>("						星级：" + to_string(i->at("weapon")["rarity"].get<int>()) + '\n');
						msg.Add<PlainMessage>("						武器类型：" + i->at("weapon")["type_name"].get<string>() + '\n');
					}
				}
			}catch(const exception&ex) {
				cout << ex.what() << endl;
				msg.Add<PlainMessage>("似乎天空岛暂时关闭了大门，请稍后再试吧~  ——派蒙");
				return msg;
			}
		}
		return MessageChain();
	}
};