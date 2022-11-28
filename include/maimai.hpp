#include <mirai.h>
#include <cpr/cpr.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <time.h>
using namespace std;
using namespace Cyan;
class maimai {
private:
	json data;
	vector<pair<json,double>>rating_filter(const double& rmin, const double& rmax = INT_MAX, const string& type = "all") {
		vector<pair<json,double>>res;
		for (auto i : type) {
			if (i >= 'a' && i <= 'z') {
				int tmp = i - 'a';
				i = 'A' + tmp;
			}
		}
		for (auto i : data) {
			if (type != "ALL") {
				if (i.at("type") != type)continue;
			}
			for (auto j : i.at("ds")) {
				if (j.get<double>() >= rmin && j.get<double>() <= rmax) {
					res.push_back(make_pair(i,j.get<double>()));
				}
			}
		}
		return res;
	}
	vector<json>search_song(const string& base) {
		vector<json>res;
		for (auto i : data) {
			if(i["title"].get<string>().find(base)!=string::npos) {
				res.push_back(i);
			}
		}
		return res;
	}
	json random_song(const double& rmin, const double& rmax = INT_MAX, const string& type = "all") {
		vector<pair<json,double>> data = rating_filter(rmin, rmax,type);
		random_device rnd;
		int index = rnd() % data.size();
		return data[index].first;
	}
	json id_get_song(const string&id) {
		for(auto i:data) {
			if (i.at("id").get<string>() == id)return i;
		}
		return NULL;
	}
	/* Prob Unneeded
	string unicode2string(const string& unicode) {
		string res;
		for (int i = 0; i < unicode.size(); i++) {
			if (unicode[i] == '\\') {
				if (unicode[i + 1] == 'u') {
					string tmp;
					tmp += unicode[i + 2];
					tmp += unicode[i + 3];
					tmp += unicode[i + 4];
					tmp += unicode[i + 5];
					int num = stoi(tmp, nullptr, 16);
					res += (char)num;
					i += 5;
				}
				else {
					res += unicode[i];
				}
			}
			else {
				res += unicode[i];
			}
		}
		return res;
	}
	*/
public:
	maimai() {
		auto res = cpr::Get(cpr::Url{ "https://www.diving-fish.com/api/maimaidxprober/music_data" });
		data = json::parse(res.text);
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
		if (*commands.begin() == "mai") {
			if (commands.size() <= 4 && commands.size() >= 3) {
				if (commands[1] == "定数查歌") {
					if (commands.size() == 3) {
						try {
							double rating = stod(commands[2]);
							vector<pair<json, double>> res = rating_filter(rating);
							if (res.size() > 50)return MessageChain().Plain("结果太多了哦");
							MessageChain reply;
							reply.Plain("结果如下：\n");
							for (auto i : res) {
								reply.Plain(i.first.at("id").get<string>() + ". " + i.first.at("title").get<string>() + "谱面类型：" + i.first.at("type").get<string>() + "定数：" + to_string(i.second) + '\n');
							}
							return reply;
						}
						catch (const invalid_argument& ex) {
							return MessageChain().Plain("这真的是\"定数\"嘛？");
						}

					}
					else {
						try {
							double minrating = stod(commands[2]);
							double maxrating = stod(commands[3]);
							vector<pair<json, double>> res = rating_filter(minrating, maxrating);
							if (res.size() > 50)return MessageChain().Plain("结果太多了哦");
							MessageChain reply;
							reply.Plain("结果如下：\n");
							for (auto i : res) {
								reply.Plain(i.first.at("id").get<string>() + ". " + i.first.at("title").get<string>() + "谱面类型：" + i.first.at("type").get<string>() + "定数：" + to_string(i.second) + '\n');
							}
							return reply;
						}
						catch (const invalid_argument& ex) {
							return MessageChain().Plain("这真的是\"定数\"嘛？");
						}
					}
				}
				if (commands[1] == "搜歌") {
					string query = commands[2];
					vector<json>res = search_song(query);
					if (res.empty())return MessageChain().Plain("未找到结果哦");
					if (res.size() > 50)return MessageChain().Plain("结果太多了哦");
					MessageChain reply;
					for (auto i : res) {
						reply.Plain(i.at("id").get<string>() + ". " + i.at("title").get<string>() + "谱面类型：" + i.at("type").get<string>() + '\n');
					}
					return reply;
				}
				if (commands[1] == "查歌") {
					string id = commands[2];
					json res = id_get_song(id);
					if (res == NULL)return MessageChain().Plain("并没有这首歌哦");
					MessageChain reply;
					reply.Plain("歌名：" + res["title"].get<string>() + '\n');
					reply.Plain("谱面类型：" + res["type"].get<string>() + '\n');
					reply.Plain("作者：" + res["basic_info"]["artist"].get<string>() + '\n');
					reply.Plain("bpm：" + to_string(res["basic_info"]["bpm"].get<int>()) + '\n');
					reply.Plain("定数：");
					for (int i = 0; i < res["ds"].size(); ++i) {
						reply.Plain(to_string(res["ds"][i].get<double>()));
						if (i != res["ds"].size() - 1)reply.Plain("，");
					}
					reply.Plain('\n');
					if (res["basic_info"]["is_new"].get<bool>())reply.Plain("是新歌哦~");
					return reply;
				}
			}
			else return MessageChain().Plain("请检查输入");
		}
	}
};