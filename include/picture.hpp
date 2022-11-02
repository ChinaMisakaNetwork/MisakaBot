﻿#include <mirai.h>
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
		stringstream sin((m.MessageChain.GetPlainText()));
		vector<string>commands;
		string temp;
		while (getline(sin, temp, ' ')) {
			commands.push_back(temp);
		}
		if (commands.size() == 0)return "";
		//Image Colourization
		if (*commands.begin() == "图片上色") {
			try {
				auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/colorizer" },
					cpr::Multipart{
						{"image",m.MessageChain.GetFirst<MiraiImage>().Url},
					},
					cpr::Header{ {"api_key",api_key}, {"user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36"} }
				);
				json reply = json::parse(res.text);
				img.Url = reply["output_url"].get<string>();
				return "结果如下：";
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		//Super Resolution
		if(*commands.begin() =="图片超分") {
			try {
				auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/super-resolution" },
					cpr::Multipart{
						{"image",m.MessageChain.GetFirst<MiraiImage>().Url},
					},
					cpr::Header{ {"api_key",api_key},{"user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36"} }
				);
				json reply = json::parse(res.text);
				img.Url = reply["output_url"].get<string>();
				return "结果如下：";
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		//Text To Image
		if (*commands.begin() == "AI画图") {
			if (commands.size() != 2)return "请检查格式";
			try {
				auto res = cpr::Post(cpr::Url{ "https://api.deepai.org/api/text2img" },
					cpr::Multipart{
						{"text",commands[1]}
						
					},
					cpr::Header{ {"api_key",api_key},{"user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36"} }
				);
				json reply = json::parse(res.text);
				cout << res.text << endl;
				img.Url = reply["output_url"].get<string>();
				return "结果如下：";
			}
			catch (const exception& ex) {
				cout << ex.what() << endl;
				return "出现错误，请稍后再试";
			}
		}
		return "";
	}
	
};