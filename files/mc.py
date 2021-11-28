from mirai import Mirai, WebSocketAdapter, GroupMessage, Plain
import requests
import flask, pymysql, json, datetime, mcrcon, yaml, threading
async def on_group_message(event: GroupMessage):
    if event.sender.group.id == ${YOUR MC GEOUP ID}:
        if ".wl" == str(event.message_chain).split(' ')[0]:
            id = str(event.message_chain).split(' ')[1]
            url = 'http://${YOUR MCSM ADDRESS}/api/execute/?apikey='
            data = {
                "name":"Waterfall_Proxy",
                "command":"whitelist add "+ str(id)
            }
            reponese = requests.post(url,data=data)
            conn_obj = pymysql.connect(host='localhost', user='', password="", database='mcdata', charset='utf8mb4')
            cursor = conn_obj.cursor()
            if len(str(id)) < 3 or len(str(id)) > 16:
                await bot.send(event,"用户名过长或过短！")
                return 0
            for legt in str(id):
                if legt not in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_":
                    await bot.send(event,"用户名中存在非法字符！")
                    return 0
            cursor.execute('select * from data where json_contains(Username, \'"'+str(id)+'"\', "$")')
            usc = cursor.fetchall()
            if len(usc)>0:
                await bot.send(event,"该ID已被注册，请更换一个心仪的ID！")
                return 0
            cursor.close()
            conn_obj.close()
            conn_obj = pymysql.connect(host='localhost', user='', password="", database='mcdata', charset='utf8mb4')
            cursor=conn_obj.cursor()
            cursor.execute('select * from data where QQ="'+str(event.sender.id)+'"')
            test = cursor.fetchall()
            cursor.close()
            conn_obj.close()
            conn_obj = pymysql.connect(host='localhost', user='', password="", database='mcdata', charset='utf8mb4')
            cursor=conn_obj.cursor()
            if len(test)==0:
                cursor.execute('insert into data(QQ, Username) values ("'+str(event.sender.id)+'", \'["'+str(id)+'"]\')')
                conn_obj.commit()
            else:
                if len(json.loads(test[0][2]))==4:
                    cursor.close()
                    conn_obj.close()
                    await bot.send(event,"你注册了太多的ID辣")
                    return 0
                else:
                    oldr = json.loads(test[0][2])
                    oldr.append(str(id))
                    cursor.execute('update data set Username=\''+json.dumps(oldr)+'\' where QQ="'+str(event.sender.id)+'"')
                    conn_obj.commit()
                cursor.close()
                conn_obj.close()
            await bot.send(event,"白名到账")
            return 0
        elif ".ban" == str(event.message_chain).split(' ')[0]:
            if event.sender.id == ${YOUT ADMIN QQ}:
                id = str(event.message_chain).split(' ')[1]
                url = 'http://localhost:25569/api/execute/?apikey='
                data = {
                        "name":"BungeecordProxy",
                        "command":"gban "+ str(id) + " You are banned by our staff."
                }
                reponese = requests.post(url,data=data)
                await bot.send(event,"已封禁"+id)
            else:
                await bot.send(event,"抱歉，您没有这个权限")
            return 0
        elif ".wlr" == str(event.message_chain).split(' ')[0]:
            if event.sender.id == ${YOUR ADMIN QQ}:
                id = str(event.message_chain).split(' ')[1]
                url = 'http://localhost:25569/api/execute/?apikey='
                data = {
                        "name":"Waterfall_proxy",
                        "command":"whitelist remove " + str(id)
                }
                reponese = requests.post(url,data=data)
                conn_obj = pymysql.connect(host='localhost', user='', password="", database='mcdata', charset='utf8mb4')
                cursor = conn_obj.cursor()
                cursor.execute('select Username from mcdata.data where json_contains(Username, \'"%s"\', "$")'%(str(id)))
                res = cursor.fetchall()
                if len(res) == 0:
                    await bot.send(event, "未找到用户名")
                    return 0
                fet_usr=json.loads(res[0][0])
                fet_usr.remove(str(id))
                stusr=json.dumps(fet_usr)
                cursor.close()
                conn_obj.close()
                conn_obj = pymysql.connect(host='localhost', user='', password="", database='mcdata', charset='utf8mb4')
                cursor=conn_obj.cursor()
                cursor.execute("update mcdata.data set Username='%s' where json_contains(Username, '\"%s\"', '$')"%(stusr, str(id)))
                conn_obj.commit()
                await bot.send(event,"已移除"+id+"的白名单")
                return 0
            else:
                id = str(event.message_chain).split(' ')[1]
                if '"' in id or "'" in id or '\\' in id:
                    await bot.send(event, "SQL injection is not allowed")
                    return 0
                conn_obj = pymysql.connect(host='localhost', user='', password='', database='mcdata', charset='utf8mb4')
                cursor=conn_obj.cursor()
                cursor.execute('select QQ, Username from mcdata.data where json_contains(Username, \'"%s"\', "$")'%(id))
                ret=cursor.fetchall()
                cursor.close()
                conn_obj.close()
                if len(ret) == 0:
                    await bot.send(event, "未找到用户名")
                    return 0
                if int(ret[0][0]) != event.sender.id:
                    await bot.send(event,"抱歉，您没有这个权限")
                    return 0
                data = {
                        "name":"Waterfall_proxy",
                        "command":"whitelist remove " + str(id)
                }
                reponese = requests.post('http://localhost:25569/api/execute/?apikey=',data=data)
                conn_obj = pymysql.connect(host='localhost', user='', password='', database='mcdata', charset='utf8mb4')
                cursor=conn_obj.cursor()
                fet_usr = json.loads(ret[0][1])
                fet_usr.remove(id)
                stusr = json.dumps(fet_usr)
                cursor.execute("update mcdata.data set Username='%s' where json_contains(Username, '\"%s\"', '$')"%(stusr, str(id)))
                conn_obj.commit()
                await bot.send(event,"已移除"+id+"的白名单")
                return 0
        elif ".wlme" == str(event.message_chain).split(' ')[0]:
            conn_obj = pymysql.connect(host='localhost', user='', password="", database='mcdata', charset='utf8mb4')
            cursor = conn_obj.cursor()
            cursor.execute('select * from data where QQ='+str(event.sender.id))
            ret = cursor.fetchall()
            if len(ret) == 0:
                await bot.send(event,"您还没有注册哦")
                return 0
            else:
                users = json.loads(ret[0][2])
                nowlen = len(users)
                temp_string = ""
                for x in range(0, len(users)):
                    temp_string = temp_string + " " + str(users[x])
                await bot.send(event,"你有"+str(nowlen)+"个账户，分别是"+temp_string)
