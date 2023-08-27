#include <mirai.h>
#include <cpr/cpr.h>
#include <string>
#include <vector>
#include <time.h>
using namespace std;
using namespace Cyan;
class hypixel {
	string apikey_ = "";
	static string getuuid(const string& ign){
		auto url = cpr::Url{ "https://api.mojang.com/users/profiles/minecraft/" + ign };
		cpr::Response r = Get(url);
		if (r.status_code == 204)return "";
		json data = json::parse(r.text);
		return data["id"].get<string>();
	}
	static string game_translate(const string& gn) {
		if (gn == "DUELS"||gn=="Duels")return "决斗游戏";
		if (gn == "Bedwars" || gn=="BEDWARS")return "起床战争";
		if (gn == "SkyWars"||gn=="SKYWARS")return "空岛战争";
		if (gn == "UHC")return "极限生存冠军";
		if (gn == "Pit" || gn=="PIT")return "天坑";
		if (gn == "MurderMystery" || gn=="MURDERMYSTERY")return "密室杀手";
		return "";
	}
	static string game_reconvert(const string&gn) {
		if (gn == "bw")return "Bedwars";
		if (gn == "sw")return "SkyWars";
		if (gn == "duel")return "Duels";
		if (gn == "pit")return "Pit";
		if (gn == "UHC" || gn=="uhc")return "UHC";
		if (gn == "mm")return "MurderMystery";
		return "";
	}
	string swgetlevel(const int& exp) const {
		const int xps[] = { 0, 20, 70, 150, 250, 500, 1000, 2000, 3500, 6000, 10000, 15000 };
		if(exp>=15000) {
			return to_string((exp - 15000) / 10000 + 12);
		}
		for(int i=0;i<12;++i) {
			if(exp < xps[i]) {
				return to_string(i + static_cast<float>(exp - xps[i - 1]) / (xps[i] - xps[i - 1]));
			}
		}
		return "0";
	}
	static string rank_convert(const string& rank) {
		if (rank == "MVP_PLUS")return "MVP+";
		if (rank == "MVP")return "MVP";
		if (rank == "VIP_PLUS")return "VIP+";
		if (rank == "VIP")return "VIP";
		return "";
	}
	string getrank(const json& data) {
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
		else if (data["player"].contains("PackageRank"))rank = rank_convert(data["player"]["PackageRank"].get<string>());
		else rank = "无";
		return rank;
	}
public:
	explicit hypixel(const string& key) {
		apikey_ = key;
	}
	MessageChain handler(GroupMessage m) {
		stringstream sin(m.MessageChain.GetPlainText());
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.empty())return MessageChain();
		if(commands[0]=="mcuuid") {
			if (commands.size() < 2)return MessageChain().Plain("请输入玩家名");
			return MessageChain().Plain(getuuid(commands[1]));
		}
		if (commands[0]=="hyp") {
			string ign = commands[1];
			string uuid = getuuid(commands[1]);
			if (uuid == "")return MessageChain().Plain("玩家不存在");
			auto r = cpr::Get(cpr::Url{ "https://api.hypixel.net/player" }, cpr::Parameters{ {"key",apikey_},{"uuid",uuid} });
			json data = json::parse(r.text);
			if (data["success"].get<bool>() == false)return MessageChain().Plain("Hypixel日常抽风了呢 :[");
			if(commands.size()==2) {
				time_t llg = data["player"]["lastLogin"].get<time_t>() / 1000, llgo = data["player"]["lastLogout"].get<time_t>() / 1000;
				tm lastlogin = *localtime(&llg);
				tm lastlogout = *localtime(&llgo);
				string recentgame = game_translate(data["player"]["mostRecentGameType"].get<string>());
				string rank = getrank(data);
				string networklevel = to_string(int(floor(1 - 3.5 + sqrt(12.25 + 0.0008 * data["player"]["networkExp"].get<double>()))));
				string achievementpoints = to_string(data["player"]["achievementPoints"].get<int>());
				MessageChain reply;
				reply.Plain("\n");
				reply.Plain("游戏名：" + ign + "\n");
				reply.Plain("Rank：" + rank + "\n");
				reply.Plain("等级：" + networklevel + "\n");
				reply.Plain("成就点数：" + achievementpoints+'\n');
				reply.Plain("最后一次登录：" + to_string(lastlogin.tm_year + 1900) + "年" + to_string(lastlogin.tm_mon + 1) + "月" + to_string(lastlogin.tm_mday) + "日" + to_string(lastlogin.tm_hour) + "时" + to_string(lastlogin.tm_min) + "分" + to_string(lastlogin.tm_sec) + "秒\n");
				reply.Plain("最后一次登出：" + to_string(lastlogout.tm_year + 1900) + "年" + to_string(lastlogout.tm_mon + 1) + "月" + to_string(lastlogout.tm_mday) + "日" + to_string(lastlogout.tm_hour) + "时" + to_string(lastlogout.tm_min) + "分" + to_string(lastlogout.tm_sec) + "秒\n");
				if (llg > llgo)reply.Plain("当前状态：在线\n");
				else reply.Plain("当前状态：离线\n");
				if (recentgame != "")reply.Plain("最近一次游玩：" + recentgame + "\n");
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
				string rank = getrank(data);
				if(gametype=="Bedwars") {
					int kill, death, finalkill, finaldeath, wins, gameplayed, bb, bl, lvl;
					double kdr, fkdr, bblr, wlr;
					json bw = game["Bedwars"];
					if (bw.contains("kills_bedwars"))kill = bw["kills_bedwars"].get<int>();
					else kill = 0;
					if (bw.contains("deaths_bedwars")) {
						death = bw["deaths_bedwars"].get<int>();
						kdr = static_cast<double>(kill) / static_cast<double>(death);
					}
					else {
						death = 0;
						kdr = static_cast<double>(kill);
					}
					if (bw.contains("final_kills_bedwars"))finalkill = bw["final_kills_bedwars"].get<int>();
					else finalkill = 0;
					if (bw.contains("final_deaths_bedwars"))finaldeath = bw["final_deaths_bedwars"].get<int>(), fkdr = static_cast<double>(finalkill) / static_cast<double>(finaldeath);
					else {
						finaldeath = 0;
						fkdr = static_cast<double>(finalkill);
					}
					if (data["player"]["achievements"].contains("bedwars_level"))lvl = data["player"]["achievements"]["bedwars_level"].get<int>();
					else lvl = 0;
					if (bw.contains("beds_broken_bedwars"))bb = bw["beds_broken_bedwars"].get<int>();
					else bb = 0;
					if (bw.contains("beds_lost_bedwars"))bl = bw["beds_lost_bedwars"].get<int>(), bblr = static_cast<double>(bb) / static_cast<double>(bl);
					else {
						bl = 0;
						bblr = static_cast<double>(bb);
					}
					if (bw.contains("wins_bedwars"))wins = bw["wins_bedwars"].get<int>();
					else wins = 0;
					if (bw.contains("games_played_bedwars"))gameplayed = bw["games_played_bedwars"].get<int>(), wlr = static_cast<double>(wins) / static_cast<double>(gameplayed);
					else gameplayed = 0, wlr = 0, wins = 0;
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
					string lvl;
					int kill, death, assets, heads, gameplayed, wins;
					double wlr, kdr, arrowhitchance;
					if (sw.contains("skywars_experience"))lvl = swgetlevel(sw["skywars_experience"].get<int>());
					else lvl = "0";
					if (sw.contains("kills"))kill = sw["kills"].get<int>();
					else kill = 0;
					if(sw.contains("deaths"))death = sw["deaths"].get<int>(), kdr = static_cast<double>(kill) / static_cast<double>(death);
					else {
						death = 0;
						kdr = static_cast<double>(kill);
					}
					if (sw.contains("wins"))wins = sw["wins"].get<int>();
					else wins = 0;
					if (sw.contains("assists")) assets = sw["assists"].get<int>();
					else assets = 0;
					if (sw.contains("heads"))heads = sw["heads"].get<int>();
					else heads = 0;
					if (sw.contains("arrows_shot")) {
						if (sw.contains("arrows_hit")) arrowhitchance = static_cast<double>(sw["arrows_hit"].get<int>()) / static_cast<double>(sw["arrows_shot"].get<int>());
						else arrowhitchance = 0;
					}
					if (sw.contains("games_played_skywars"))gameplayed = sw["games_played_skywars"].get<int>(), wlr = static_cast<double>(wins) / static_cast<double>(gameplayed);
					else gameplayed = 0, wlr = 0;
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
					reply.Plain("箭矢命中率：" + to_string(arrowhitchance * 100) + "%\n");
					reply.Plain("头颅个数：" + to_string(heads) + '\n');
					return reply;
				}
				if(gametype=="Duels") {
					json duel = game["Duels"];
					int ws, curws, kill, wins, death, gameplayed;
					double wlr, kdr;
					if (duel.contains("best_overall_winstreak"))ws = duel["best_overall_winstreak"].get<int>();
					else ws = 0;
					if (duel.contains("current_winstreak"))curws = duel["current_winstreak"].get<int>();
					else curws = 0;
					if (duel.contains("kills"))kill = duel["kills"].get<int>();
					else kill = 0;
					if(duel.contains("deaths")) {
						death = duel["deaths"].get<int>();
						kdr = static_cast<double>(kill) / static_cast<double>(death);
					}else {
						death = 0;
						kdr = static_cast<double>(kill);
					}
					if (duel.contains("wins"))wins = duel["wins"].get<int>();
					else wins = 0;
					if(duel.contains("games_played_duels")) {
						gameplayed = duel["games_played_duels"].get<int>();
						wlr = static_cast<double>(wins) / static_cast<double>(gameplayed);
					}else {
						gameplayed = 0;
						wlr = 0;
					}
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
				if (gametype == "UHC") {
					json uhc = game["UHC"];
					int score, solokill, solodeath, teamkill, teamdeath, sologheadeaten, teamgheadeaten;
					double solokdr, teamkdr;
					if (uhc.contains("score"))score = uhc["score"].get<int>();
					else score = 0;
					if (uhc.contains("kills_solo")) {
						solokill = uhc["kills_solo"].get<int>();
					}
					else solokill = 0;
					if (uhc.contains("deaths_solo")) {
						solodeath = uhc["deaths_solo"].get<int>();
						solokdr = static_cast<double>(solokill) / static_cast<double>(solodeath);
					}
					else solokdr = static_cast<double>(solokill), solodeath = 0;
					if (uhc.contains("kills")) {
						teamkill = uhc["kills"].get<int>();
					}
					else teamkill = 0;
					if (uhc.contains("deaths")) {
						teamdeath = uhc["deaths"].get<int>();
						teamkdr = static_cast<double>(teamkill) / static_cast<double>(teamdeath);
					}
					else teamdeath = 0, teamkdr = static_cast<double>(teamkill);
					if (uhc.contains("heads_eaten_solo"))sologheadeaten = uhc["heads_eaten_solo"].get<int>();
					else sologheadeaten = 0;
					if (uhc.contains("heads_eaten"))teamgheadeaten = uhc["heads_eaten"].get<int>();
					else teamgheadeaten = 0;
					MessageChain reply;
					reply.Plain('\n');
					reply.Plain("Rank：" + rank + "游戏名：" + ign + "的极限生存冠军信息如下：" + "\n");
					reply.Plain("硬币：" + to_string(coins) + '\n');
					reply.Plain("单人模式击杀数：" + to_string(solokill) + '\n');
					reply.Plain("单人模式死亡数：" + to_string(solodeath) + '\n');
					reply.Plain("单人模式K/D：" + to_string(solokdr) + '\n');
					reply.Plain("单人模式吃头数：" + to_string(sologheadeaten) + '\n');
					reply.Plain("团队模式击杀数：" + to_string(teamkill) + '\n');
					reply.Plain("团队模式死亡数：" + to_string(teamdeath) + '\n');
					reply.Plain("团队模式K/D：" + to_string(teamkdr) + '\n');
					reply.Plain("团队模式吃头数：" + to_string(sologheadeaten) + '\n');
					return reply;
				}
				return MessageChain().Plain("更多查询正在开发中哦");
			}
		}
		return MessageChain();
	}
};