#include <mirai.h>
#include <bits/extc++.h>
using namespace std;
using namespace Cyan;
class recorder{
protected:
public:
	void handler(GroupMessage m, map<int, mutex>& fslock)
	{
		string s = m.MessageChain.GetPlainText();
		fslock[m.Sender.Group.GID.ToInt64()].lock();
		fs.open("chatrecord/" + to_string(m.Sender.Group.GID.ToInt64()) + ".txt",ios::app);
		if (!fs.good())
		{
			string temp = "chatrecord/";
			system(temp.c_str());
			fs.open("chatrecord/" + to_string(m.Sender.Group.GID.ToInt64()) + ".txt");
		}
		fs << "时间戳：" << m.MessageChain.Timestamp() << " 发送者：" << m.Sender.QQ.ToInt64() << " 内容：" << s << endl;
		fslock[m.Sender.Group.GID.ToInt64()].unlock();
	}
private:
	ofstream fs;
};