#include <mirai.h>
#include <bits/extc++.h>
using namespace std;
using namespace Cyan;
class recorder{
protected:
public:
	struct identifier
	{
		GID_t gid;
		QQ_t qq;
		time_t time;
	};
	void handler(GroupMessage m, map<int, mutex>& fslock)
	{
		string s = m.MessageChain.GetPlainText();
		fslock[m.Sender.Group.GID.ToInt64()].lock();
		fs.open("chatrecord/" + to_string(m.Sender.Group.GID.ToInt64()) + ".txt");
		if (!fs.good())
		{
			string temp = "chatrecord/" + to_string(m.Sender.Group.GID.ToInt64());
			system(temp.c_str());
			fs.open("chatrecord/" + to_string(m.Sender.Group.GID.ToInt64()) + ".txt");
		}
		fs << "ʱ�����" << m.MessageChain.Timestamp() << " �����ߣ�" << m.Sender.QQ.ToInt64() << " ���ݣ�" << s << endl;
		fslock[m.Sender.Group.GID.ToInt64()].unlock();
	}
private:
	ofstream fs;

};