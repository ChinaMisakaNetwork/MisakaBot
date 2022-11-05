#pragma once
#include <mirai.h>
#include <cpr/cpr.h>
#include <string>
using namespace std;
using namespace Cyan;
class deepai_picture {
private:
	string api_key;
protected:
public:
	deepai_picture(string key) {
		api_key = key;
	}
	string handler(GroupMessage m,GroupImage& img) {
		if (api_key.empty())return "未设置apikey，暂时无法使用该功能哦~";
		stringstream sin((m.MessageChain.GetPlainText()));
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			for (int i = 0; i < temp.size(); ++i) {
				if (temp[i] == '\n') {
					temp.erase(temp.begin()+i);
					--i;
				}
			}
			commands.push_back(temp);
		}
		if (commands.size() == 0)return "";
		vector<ImageMessage>mc = m.MessageChain.GetAll<ImageMessage>();
		{
			MessageChain mcs;
			mcs.At(m.Sender.QQ);
			mcs.Add<PlainMessage>("正在处理中，请稍候…");
			if (*commands.begin() == "图片上色") {
				try {
					m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
					auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/colorizer" }, 
						cpr::Payload{ {"image",mc[0].Url()},
						},
						cpr::Header{ {"Api-Key",api_key} });
					
					json reply = json::parse(res.text);
					img.Url = reply["output_url"].get<string>();
					return "结果如下：";
				}
				catch (...) {
				}
			}
			//Super Resolution
			if (*commands.begin() == "图片超分") {
			if (mc.size() == 0)return "格式错误";
			try {
				m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
				auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/torch-srgan" },
					cpr::Payload{ {"image",mc[0].Url()},
					},
					cpr::Header{ {"Api-Key",api_key} });
				json reply = json::parse(res.text);
				img.Url = reply["output_url"].get<string>();
				return "结果如下：";
			}
			catch (...) {
			}
			}
			if (*commands.begin() == "图片降噪") {
				if (mc.size() == 0)return "格式错误";
				try {
					m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
					auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/waifu2x" },
						cpr::Payload{ {"image",mc[0].Url()},
						},
						cpr::Header{ {"Api-Key",api_key} });
					json reply = json::parse(res.text);
					img.Url = reply["output_url"].get<string>();
					return "结果如下：";
				}
				catch (...) {
				}
			}
			//Text To Image
			if (*commands.begin() == "AI画图") {
				if (commands.size() != 2)return "请检查格式";
				try {
					m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
					auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/text2img" },
						cpr::Payload{
							{"text",commands[1]},
						},
						cpr::Header{ {"Api-Key",api_key} }
					);
					json reply = json::parse(res.text);
					cout << res.text << endl;
					img.Url = reply["output_url"].get<string>();
					return "结果如下：";
				}
				catch (...) {
				}
			}
		}
		return "";
	}
	
};