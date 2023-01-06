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
	MessageChain handler(GroupMessage m) {
		MessageChain msg;
		if (api_key.empty()) {
			msg.Add<PlainMessage>("未设置apikey，暂时无法使用该功能哦~");
			return msg;
		}
		stringstream sin((m.MessageChain.GetPlainText()));
		vector<string>commands;
		string temp;
		while (getline(sin, temp, '#')) {
			if (temp.empty())continue;
			commands.push_back(temp);
		}
		if (commands.size() == 0)return MessageChain();
		vector<ImageMessage>mc = m.MessageChain.GetAll<ImageMessage>();
		{
			MessageChain mcs;
			mcs.At(m.Sender.QQ);
			mcs.Add<PlainMessage>("正在处理中，请稍候…");
			if (*commands.begin() == "图片上色") {
				try {
					msg.Add<PlainMessage>("结果如下：");
					for (ImageMessage& i : mc) {
						GroupImage img;
						m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
						auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/colorizer" },
							cpr::Payload{ {"image",i.Url()},
							},
							cpr::Header{ {"Api-Key",api_key} });
						json reply = json::parse(res.text);
						img.Url = reply["output_url"].get<string>();
						msg.Image(img);
					}
					return msg;
				}
				catch (...) {
				}
			}
			//Super Resolution
			if (*commands.begin() == "图片超分") {
			try {
				msg.Add<PlainMessage>("结果如下：");
				for (ImageMessage& i : mc) {
					GroupImage img;
					m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
					auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/super-resolution" },
						cpr::Payload{ {"image",i.Url()},
						},
						cpr::Header{ {"Api-Key",api_key} });
					json reply = json::parse(res.text);
					img.Url = reply["output_url"].get<string>();
					msg.Image(img);
				}
				return msg;
			}
			catch (...) {
			}
			}
			if (*commands.begin() == "图片降噪") {
				try {
					msg.Add<PlainMessage>("结果如下：");
					for (ImageMessage& i : mc) {
						GroupImage img;
						m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
						auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/denoise" },
							cpr::Payload{ {"image",i.Url()},
							},
							cpr::Header{ {"Api-Key",api_key} });
						json reply = json::parse(res.text);
						img.Url = reply["output_url"].get<string>();
						msg.Image(img);
					}
					return msg;
				}
				catch (...) {
				}
			}
			//Text To Image
			if (*commands.begin() == "AI画图") {
				if (commands.size() != 2) {
					msg.Add<PlainMessage>("参数错误");
					return msg;
				}
				try {
					GroupImage img;
					msg.Add<PlainMessage>("结果如下：");
					m.GetMiraiBot().SendMessage(m.Sender.Group.GID, mcs);
					auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/stable-diffusion" },
						cpr::Payload{
							{"text",commands[1]},
						},
						cpr::Header{ {"Api-Key",api_key} }
					);
					json reply = json::parse(res.text);
					img.Url = reply["output_url"].get<string>();
					msg.Image(img);
					return msg;
				}
				catch (...) {
				}
			}
		}
		return MessageChain();
	}
	
};