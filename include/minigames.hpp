#pragma once
#include <mysql++.h>
#include "admin.hpp"
#include <mirai.h>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include <random>
#include <ctime>
using namespace std;
using namespace Cyan;
struct tm nowdate;
struct tm newdate;
map<QQ_t,bool>SignTable;
map<QQ_t,int>RPTable;
bool operator!=(const tm& a, const tm& b) {
    return a.tm_year != b.tm_year || a.tm_mon != b.tm_mon || a.tm_mday != b.tm_mday;
}
class minigames:public DatabaseOperator{
    random_device rnd;
    mt19937 gainrnd;
    bool isNewMoney(const QQ_t& qq){
        query.reset();
        query<<"select * from money where user=%0q";
        query.parse();
        mysqlpp::StoreQueryResult res;
        res=query.store(qq.ToInt64());
        return res.empty();
    }
    static bool DateUpdated(){
        time_t nt;
        time(&nt);
        newdate = *localtime(&nt);
        if(newdate!=nowdate){
            SignTable.clear();
            RPTable.clear();
            nowdate=newdate;
            return true;
        }
        return false;
    }
    static bool SignToday(const QQ_t& qqid){
        if(SignTable.find(qqid)==SignTable.end() || DateUpdated()){
            SignTable[qqid]=true;
            return true;
        }
        return false;
    }
    QQ_t GetCP(const QQ_t& qqid){
        query.reset();
        query << "select * from couples where husband = %0q";
        query.parse();
        auto res=query.store(qqid.ToInt64());
        if(res.empty()){
            query.reset();
            query << "select * from couples where wife = %0q";
            query.parse();
            res=query.store(qqid.ToInt64());
            if(res.empty()){
                return QQ_t(0);
            }
            return QQ_t(res[0]["husband"]);
        }
        return QQ_t(res[0]["wife"]);
    }
    bool CheckMarried(const QQ_t& qqid){
        mysqlpp::StoreQueryResult res;
        query.reset();
        query << "select * from couples where husband = %0q or wife = %1q";
        query.parse();
        try {
            res = query.store(qqid.ToInt64(),qqid.ToInt64());
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            return false;
        }
        return !res.empty();
    }
    void BoundCouples(const QQ_t& husband,const QQ_t& wife,const long long& favor,const long long& money){
        query.reset();
        query<<"insert into couples(husband,wife,favor,money_spent) values(%0q,%1q,%2q,%3q)";
        query.parse();
        query.execute(husband.ToInt64(),wife.ToInt64(),favor,money);
    }
    void UnboundCouples(const QQ_t& singledog){
        query.reset();
        query<<"delete from couples where husband=%0q or wife=%1q";
        query.parse();
        query.execute(singledog.ToInt64(),singledog.ToInt64());
    }
    void AddFavor(const QQ_t& husband,const QQ_t& wife,const long long& favor){
        long long ff=GetFavor(husband,wife);
        long long money=GetMoneySpent(husband,wife);
        UnboundCouples(husband);
        BoundCouples(husband,wife,ff+favor,money);
    }
    void AddMoneySpent(const QQ_t& husband,const QQ_t& wife,const long long& money){
        long long ff=GetFavor(husband,wife);
        long long mm=GetMoneySpent(husband,wife);
        UnboundCouples(husband);
        BoundCouples(husband,wife,ff,mm+money);
    }
    void DecreaseMoneySpent(const QQ_t& husband,const QQ_t& wife,const long long& money){
        long long ff=GetFavor(husband,wife);
        long long mm=GetMoneySpent(husband,wife);
        UnboundCouples(husband);
        BoundCouples(husband,wife,ff,mm-money);
    }
    void DecreaseFavor(const QQ_t& husband,const QQ_t& wife,const long long& favor){
        long long ff=GetFavor(husband,wife);
        long long mm=GetMoneySpent(husband,wife);
        UnboundCouples(husband);
        BoundCouples(husband,wife,ff-favor,mm);
    }
    long long GetFavor(const QQ_t& husband,const QQ_t& wife){
        mysqlpp::StoreQueryResult res;
        query.reset();
        query<<"select * from couples where husband=%0q and wife=%1q";
        query.parse();
        try {
            res = query.store(husband.ToInt64(),wife.ToInt64());
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            return 0;
        }
        if(res.empty()){
            try{
                res=query.store(wife.ToInt64(),husband.ToInt64());
            }catch(const exception& ex){
                cerr<<ex.what()<<endl;
                return 0;
            }
        }
        if(res.empty()){
            return 0;
        }
        return res[0]["favor"];
    }
    long long GetMoneySpent(const QQ_t& husband,const QQ_t& wife){
        mysqlpp::StoreQueryResult res;
        query.reset();
        query<<"select * from couples where husband=%0q and wife=%1q";
        query.parse();
        try {
            res = query.store(husband.ToInt64(),wife.ToInt64());
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            return 0;
        }
        if(res.empty()){
            try{
                res=query.store(wife.ToInt64(),husband.ToInt64());
            }catch(const exception& ex){
                cerr<<ex.what()<<endl;
                return 0;
            }
        }
        if(res.empty()){
            return 0;
        }
        return res[0]["money_spent"];
    }
    bool SpendMoney(const QQ_t& qqid,const long long& money){
        if(isNewMoney(qqid)) {
            return false;
        }
        long long mm=GetMoney(qqid);
        if(mm<money)return false;
        query.reset();
        query<<"update money set money=%1q where user=%0q";
        query.parse();
        query.execute(qqid.ToInt64(), GetMoney(qqid)-money);
        return true;
    }
    void GainMoney(const QQ_t& qqid,const long long& money){
        long long mm=GetMoney(qqid);
        ClearMoney(qqid);
        query.reset();
        query<<"insert into money(user,money) values(%0q,%1q)";
        query.parse();
        query.execute(qqid.ToInt64(),mm+money);
    }
    void ClearMoney(const QQ_t& qqid){
        query.reset();
        query<<"delete from money where user=%0q";
        query.parse();
        query.execute(qqid.ToInt64());
    }
    long long GetMoney(const QQ_t& qqid){
        mysqlpp::StoreQueryResult res;
        query.reset();
        query<<"select * from money where user=%0q";
        query.parse();
        try {
            res = query.store(qqid.ToInt64());
        }catch(const exception& ex){
            cerr<<ex.what()<<endl;
            return 0;
        }
        if(res.empty())return 0;
        return res[0]["money"];
    }
public:
    explicit minigames(db_info dbinf1) : DatabaseOperator(std::move(dbinf1)){ // NOLINT(cert-msc51-cpp)
        gainrnd.seed(rnd());
    }
    MessageChain handler(GroupMessage m){
        stringstream sin(m.MessageChain.GetPlainText());
        vector<string>cmds;
        string temp;
        while (getline(sin, temp, ' ')) {
            cmds.push_back(temp);
        }
        if (cmds.empty())return {};
        if(cmds[0]=="强娶") {
            QQ_t husband = m.Sender.QQ;
            QQ_t wife = m.MessageChain.GetFirst<AtMessage>().Target();
            if (husband == wife) {
                return MessageChain().Plain("您没事吧");
            }
            if (!CheckMarried(husband)) {
                if (!CheckMarried(wife)) {
                    uniform_int_distribution<> range(10, 20);
                    int moneyNeed=range(gainrnd);
                    if(GetMoney(husband)<moneyNeed){
                        return MessageChain().Plain("您的金币不足");
                    }
                    BoundCouples(husband, wife,0,moneyNeed);
                    SpendMoney(husband,moneyNeed);
                    AddMoneySpent(husband,wife,moneyNeed);
                    GainMoney(wife,moneyNeed);
                    DecreaseFavor(husband,wife,10);
                    return MessageChain().Plain("恭喜你们成为了一对新人！");
                } else {
                    return MessageChain().Plain("抢对象这件事不太好吧");
                }
            } else {
                return MessageChain().Plain("你已经有老婆了");
            }
        }
        if(cmds[0]=="求婚") {
            QQ_t husband = m.Sender.QQ;
            QQ_t wife = m.MessageChain.GetFirst<AtMessage>().Target();
            if (husband == wife) {
                return MessageChain().Plain("您没事吧");
            }
            if (!CheckMarried(husband)) {
                if (!CheckMarried(wife)) {
                    bool agreed=false;
                    m.GetMiraiBot().SendMessage(m.Sender.Group.GID,MessageChain().At(wife).Plain("有人向您求婚，是否同意？"));
                    m.GetMiraiBot().On<GroupMessage>([&](const GroupMessage& m1){
                        if(m1.Sender.QQ==wife&&m1.MessageChain.GetPlainText()=="同意"){
                            agreed=true;
                        }
                    });
                    for(int i=0;i<60;++i){
                        if(!agreed) {
                            this_thread::sleep_for(chrono::seconds(1));
                        }else{
                            break;
                        }
                    }
                    if(agreed){
                        BoundCouples(husband, wife,10,0);
                        return MessageChain().Plain("恭喜你们成为了一对新人！");
                    }else{
                        return MessageChain().Plain("对方拒绝了您的求婚");
                    }
                } else {
                    return MessageChain().Plain("抢对象这件事不太好吧");
                }
            }
        }
        if(cmds[0]=="离婚"){
            QQ_t singledog = m.Sender.QQ;
            if(CheckMarried(singledog)){
                UnboundCouples(singledog);
                return MessageChain().Plain("离婚成功");
            }else{
                return MessageChain().Plain("您还没有结婚");
            }
        }
        if(cmds[0]=="查询亲密度"){
            QQ_t QT = m.Sender.QQ;
            if(CheckMarried(QT)){
                QQ_t QT1 = GetCP(QT);
                return MessageChain().Plain("您与"+to_string(QT1.ToInt64())+"的亲密度为"+to_string(GetFavor(QT,QT1)));
            }else{
                return MessageChain().Plain("您还没有结婚");
            }
        }
        if(cmds[0]=="查询余额"){
            QQ_t QT = m.Sender.QQ;
            return MessageChain().Plain("您的余额为"+to_string(GetMoney(QT)));
        }
        if(cmds[0]=="签到"){
            QQ_t QT = m.Sender.QQ;
            if(SignToday(QT)) {
                uniform_int_distribution<> range(0, 10);
                long long moneyGain = range(gainrnd);
                GainMoney(QT, moneyGain);
                long long favorGain = range(gainrnd);
                if(CheckMarried(QT)) {
                    QQ_t QT1 = GetCP(QT);
                    AddFavor(QT, QT1, favorGain);
                    return MessageChain().Plain("签到成功，您获得了" + to_string(moneyGain) + "金币和" + to_string(favorGain) + "亲密度");
                }
                return MessageChain().Plain("签到成功，您获得了" + to_string(moneyGain) + "r");
            }
            else {
                return MessageChain().Plain("您今天已经签到过了");
            }
        }
        if(cmds[0]=="jrrp"||cmds[0]=="今日人品"){
            QQ_t QT = m.Sender.QQ;
            uniform_int_distribution<> range(0, 100);
            long long rp;
            if(RPTable.find(QT)==RPTable.end()){
                rp = range(gainrnd);
                RPTable[QT]=rp; // NOLINT(cppcoreguidelines-narrowing-conversions)
            }
            else rp = RPTable[QT];
            MessageChain msg;
            //plz update
            GroupImage img;
            img.Url = "https://tenapi.cn/acg/";
            msg.Plain("今日人品为"+to_string(rp));
            return msg.Image(img);
        }
        return {};
    }
};
class RandomGameBase{
protected:
    string name;
    unsigned short id;
    string description;
    random_device rnd;
    mt19937 gainrnd;
public:
    RandomGameBase(string name,unsigned short id,string description):name(name),id(id),description(description){
        gainrnd.seed(rnd());
    }
    virtual MessageChain handler(const GroupMessage& m)=0;
    virtual ~RandomGameBase()=default;
};
class RandomSong : public RandomGameBase{
    public:
    RandomSong():RandomGameBase("随机歌曲",1,"随机一首歌曲"){}
    MessageChain handler(const GroupMessage& m) override{
        MessageChain msg;
        MusicShare ms;
        auto res=cpr::Get(cpr::Url{"https://autumnfish.cn/personalized/newsong"});
        json data=json::parse(res.text)["result"];
        const int size=data.size();
        uniform_int_distribution<> range(0, size-1);
        short index=range(gainrnd);
        string title=data[index]["name"].get<string>();
        string artist=data[index]["song"]["artists"][0]["name"].get<string>();
        string picurl=data[index]["song"]["album"]["picUrl"].get<string>();
        string refAddress="https://music.163.com/song?id="+data[index]["id"].get<string>();
        auto Musicres = cpr::Get(cpr::Url{ "https://autumnfish.cn/song/url?id=" + to_string(data[index]["song"]["id"].get<int>()) });
        json Musicdata = json::parse(res.text);
        string MusicUrl = data["data"][0].at("url").get<string>();
        string alias="";
        if(!data[index]["song"]["alias"].empty()){
            alias=data[index]["song"]["alias"][0].get<string>();
        }else alias="";
        if(!alias.empty()){
            ms.Title(title+"（"+alias+"）");
        }else ms.Title(title);
        ms.Summary(artist+" - "+title);
        ms.PictureUrl(picurl);
        ms.MusicUrl(MusicUrl);
        ms.JumUrl(refAddress);
        msg.Add<MusicShare>(ms);
        return msg;
    }
};
//この世界から旅立つ前に
//これまでの日々を浮かべる