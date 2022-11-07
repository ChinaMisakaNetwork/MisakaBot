#include <mirai.h>
#include <cpr/cpr.h>
#include <string>
using namespace std;
using namespace Cyan;
//https://www.apifox.cn/apidoc/shared-e63772a9-debc-4e57-b30d-568bead9c81c/api-45194608
//https://github.com/armoe-project/mihoyo-api
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
protected:
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
			commands.push_back(temp);
		}
		if (commands.size() == 0)return MessageChain();
		if(*commands.begin()=="原神") {
			try {
				if (commands[1] == "查首页") {
					auto res = cpr::Get(cpr::Url{ "http://127.0.0.1:8000/genshin/index/" + commands[2] }, cpr::Cookies{ {"ltoken",ltk},{"ltuid",ltuid},{"cookie_token",ckt},{"account_id",accid} });
					if(json::parse(res.text)["code"].get<int>()==422) {
						msg.Add<PlainMessage>("似乎天空岛暂时关闭了大门，请稍后再试吧~  ——派蒙");
						return msg;
					}
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
					auto res = cpr::Get(cpr::Url{ "http://127.0.0.1:8000/genshin/character/" + commands[2] }, cpr::Cookies{{"ltoken",ltk},{"ltuid",ltuid},{"cookie_token",ckt},{"account_id",accid}});
					if (json::parse(res.text)["code"].get<int>() == 422) {
						msg.Add<PlainMessage>("似乎天空岛暂时关闭了大门，请稍后再试吧~  ——派蒙");
						return msg;
					}
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