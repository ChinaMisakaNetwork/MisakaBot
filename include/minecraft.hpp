#include <mirai.h>
#include <cpr/cpr.h>
#include <string>
#include <vector>
#include <time.h>
using namespace std;
using namespace Cyan;
class hypixel {
private:
	string apikey = "";
	static inline string getuuid(const string& ign){
		cpr::Url url = cpr::Url{ "https://api.mojang.com/users/profiles/minecraft/" + ign };
		cpr::Response r = Get(url);
		if (r.status_code == 204)return "";
		json data = json::parse(r.text);
		return data["id"].get<string>();
	}
	static inline string game_translate(const string& gn) {
		if (gn == "DUELS"||gn=="Duels")return "决斗游戏";
		if (gn == "Bedwars" || gn=="BEDWARS")return "起床战争";
		if (gn == "SkyWars"||gn=="SKYWARS")return "空岛战争";
		if (gn == "UHC")return "极限生存冠军";
		if (gn == "Pit" || gn=="PIT")return "天坑";
		if (gn == "MurderMystery" || gn=="MURDERMYSTERY")return "密室杀手";
		return "";
	}
	static inline string game_reconvert(const string&gn) {
		if (gn == "bw")return "Bedwars";
		if (gn == "sw")return "SkyWars";
		if (gn == "duel")return "Duels";
		if (gn == "pit")return "Pit";
		if (gn == "UHC")return "UHC";
		if (gn == "mm")return "MurderMystery";
		return "";
	}
	string swgetlevel(const string& base) {
		string ret = "";
		for(auto i:base) {
			if(isdigit(i)) {
				ret += i;
			}
		}
		return ret;
	}
	static inline string rank_convert(const string& rank) {
		if (rank == "MVP_PLUS")return "MVP+";
		if (rank == "MVP")return rank;
		if (rank == "VIP_PLUS")return "VIP+";
		if (rank == "VIP")return rank;
		return "";
	}
public:
	hypixel(const string& key) {
		apikey = key;
	}
	MessageChain handler(GroupMessage m) {
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			commands.push_back(temp);
		}
		if (commands.empty())return MessageChain();
		if (commands[0]=="hyp") {
			string ign = commands[1];
			string uuid = getuuid(commands[1]);
			if (uuid == "")return MessageChain().Plain("玩家不存在");
			auto r = cpr::Get(cpr::Url{ "https://api.hypixel.net/player" }, cpr::Parameters{ {"key",apikey},{"uuid",uuid} });
			//if (r.status_code == 204)return MessageChain().Plain("这个人似乎并没有出现过哦");
			json data = json::parse(r.text);
			if (data["success"].get<bool>() == false)return MessageChain().Plain("API错误");
			if(commands.size()==2) {
				time_t llg = data["player"]["lastLogin"].get<time_t>(), llgo = data["player"]["lastLogout"].get<time_t>();
				tm lastlogin = *localtime(&llg);
				tm lastlogout = *localtime(&llgo);
				string recentgame = game_translate(data["player"]["mostRecentGameType"].get<string>());
				string rank = "";
				if(data["player"].contains("rank")) {
					rank = data["player"]["rank"];
				}else if(data["player"].contains("monthlyPackageRank") && data["player"]["monthlyPackageRank"]=="SUPERSTAR") {
					rank = "MVP++";
				}else if(data["player"].contains("newPackageRank")) {
					rank = rank_convert(data["player"]["newPackageRank"].get<string>());
				}
				else rank = rank_convert(data["player"]["PackageRank"].get<string>());
				string networklevel = to_string(floor(1 - 3.5 + sqrt(12.25 + 0.0008 * data["player"]["networkExp"].get<double>())));
				string achievementpoints = to_string(data["player"]["achievementPoints"].get<int>());
				MessageChain reply;
				reply.Plain("\n");
				reply.Plain("游戏名：" + ign + "\n");
				reply.Plain("Rank：" + rank + "\n");
				reply.Plain("等级：" + networklevel + "\n");
				reply.Plain("成就点数：" + achievementpoints);
				return reply;
			}
			if(commands.size()==3) {
				string gametype = "";
				if(game_reconvert(commands[2]).empty()) {
					return MessageChain().Plain("请检查游戏名");
				}
				else gametype = game_reconvert(commands[2]);
				int coins = (data["player"]["stats"][gametype]["coins"].get<int>());
				json game = data["player"]["stats"];
				string rank = "";
				if (data["player"].contains("rank")) {
					rank = data["player"]["rank"];
				}
				else if (data["player"].contains("monthlyPackageRank") && data["player"]["monthlyPackageRank"] == "SUPERSTAR") {
					rank = "MVP++";
				}
				else if (data["player"].contains("newPackageRank")) {
					rank = rank_convert(data["player"]["newPackageRank"].get<string>());
				}
				else rank = rank_convert(data["player"]["PackageRank"].get<string>());
				if(gametype=="Bedwars") {
					json bw = game["Bedwars"];
					int kill = bw["kills_bedwars"].get<int>();
					int death = bw["deaths_bedwars"].get<int>();
					int finalkill = bw["final_kills_bedwars"].get<int>();
					double kdr = double(kill) / double(death);
					int finaldeath = bw["final_deaths_bedwars"].get<int>();
					double fkdr = double(finalkill) / double(finaldeath);
					int lvl = data["player"]["achievements"]["bedwars_level"].get<int>();
					int bb = bw["beds_broken_bedwars"].get<int>();
					int bl = bw["beds_lost_bedwars"].get<int>();
					double bblr = double(bb) / double(bl);
					int wins = bw["wins_bedwars"].get<int>();
					int gameplayed = bw["games_played_bedwars"].get<int>();
					double wlr = double(wins) / double(gameplayed);
					MessageChain reply;
					reply.Plain('\n');
					reply.Plain("Rank：" + rank + "游戏名：" + ign + "的起床战争信息如下：" + "\n");
					reply.Plain("硬币：" + to_string(coins) + '\n');
					reply.Plain("总游戏次数：" + to_string(gameplayed) + '\n');
					reply.Plain("胜利场数：" + to_string(wins) + '\n');
					reply.Plain("胜率：" + to_string(wlr * 100) + "%\n");
					reply.Plain("击杀数：" + to_string(kill) + '\n');
					reply.Plain("死亡数：" + to_string(death) + '\n');
					reply.Plain("K/D：" + to_string(kdr) + '\n');
					reply.Plain("最终击杀：" + to_string(finalkill) + '\n');
					reply.Plain("最终死亡：" + to_string(finaldeath) + '\n');
					reply.Plain("FKDR：" + to_string(fkdr) + '\n');
					reply.Plain("破坏床数：" + to_string(bb) + '\n');
					reply.Plain("失去床数：" + to_string(bl) + '\n');
					reply.Plain("床比：" + to_string(bblr) + '\n');
					return reply;
				}
				if(gametype=="SkyWars") {
					json sw = game["SkyWars"];
					string lvl = swgetlevel(sw["levelFormatted"].get<string>());
					int kill = sw["kills"].get<int>(), death = sw["deaths"].get<int>();
					double kdr = double(kill) / double(death);
					int wins = sw["wins"].get<int>();
					int assets = sw["assists"].get<int>();
					int heads = sw["heads"].get<int>();
					double arrowhitchange = double(sw["arrows_hit"].get<int>()) / double(sw["arrows_shot"].get<int>());
					int gameplayed = sw["games_played_skywars"].get<int>();
					double wlr = double(wins) / double(gameplayed);
					MessageChain reply;
					reply.Plain('\n');
					reply.Plain("Rank：" + rank + "游戏名：" + ign + "的空岛战争信息如下：" + "\n");
					reply.Plain("等级：" + lvl + '\n');
					reply.Plain("硬币：" + to_string(coins) + '\n');
					reply.Plain("总游戏次数：" + to_string(gameplayed) + '\n');
					reply.Plain("胜利场数：" + to_string(wins) + '\n');
					reply.Plain("胜率：" + to_string(wlr * 100) + "%\n");
					reply.Plain("助攻数：" + to_string(assets) + '\n');
					reply.Plain("击杀数：" + to_string(kill) + '\n');
					reply.Plain("死亡数：" + to_string(death) + '\n');
					reply.Plain("K/D：" + to_string(kdr) + '\n');
					reply.Plain("头颅个数：" + to_string(heads) + '\n');
					return reply;
				}
				if(gametype=="Duels") {
					json duel = game["Duels"];
					int ws = duel["best_overall_winstreak"].get<int>();
					int curws = duel["current_winstreak"].get<int>();
					int kill = duel["kills"].get<int>(), death = duel["deaths"].get<int>();
					int wins = duel["wins"].get<int>(), gameplayed = duel["games_played_duels"].get<int>();
					double wlr = double(wins) / double(gameplayed), kdr = double(kill) / double(death);
					MessageChain reply;
					reply.Plain('\n');
					reply.Plain("Rank：" + rank + "游戏名：" + ign + "的决斗游戏信息如下：" + "\n");
					reply.Plain("硬币：" + to_string(coins) + '\n');
					reply.Plain("总游戏次数：" + to_string(gameplayed) + '\n');
					reply.Plain("胜利场数：" + to_string(wins) + '\n');
					reply.Plain("胜率：" + to_string(wlr * 100) + "%\n");
					reply.Plain("击杀数：" + to_string(kill) + '\n');
					reply.Plain("死亡数：" + to_string(death) + '\n');
					reply.Plain("K/D：" + to_string(kdr) + '\n');
					return reply;
				}
			}
		}
		return MessageChain();
	}
};