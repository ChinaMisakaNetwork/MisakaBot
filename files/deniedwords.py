from mirai import Mirai, WebSocketAdapter, GroupMessage, Plain
import requests
import flask, pymysql, json, datetime, mcrcon, yaml, threading, re
async def on_group_message(event: GroupMessage):
    if(str(event.message_chain).split()[0]=="添加违禁词"):
        conn_obj = pymysql.connect(host='localhost', user='${YOUR SQL USER}', password="${YOUR SQL USER PWD}", database='miraiadmin', charset='utf8mb4')
        cursor=conn_obj.cursor()
        cursor.execute('select * from qqadmin where groupid='+str(event.sender.group.id)+' and adminqq='+str(event.sender.id))
        if(len(cursor.fetchall())==0):
            await bot.send(event,"您的权限不足！")
            return 0
        else:
            deniedword = str(event.message_chain).split(' ')[1]
            conn_obj.commit()
            cursor.close()
            conn_obj.close()
            conn_obj1 = pymysql.connect(host='localhost', user='misakanetwork', password="misakanetwork", database='miraideniedwords', charset='utf8mb4')
            cursor1=conn_obj1.cursor()
            cursor1.execute('insert into deniedwords(groupid, word) values (%s, "%s")', [int(event.sender.group.id),deniedword])
#                    cursor.execute('insert into miraichat(question,answer,have_img) values (\''+ question + '\',\''+answer+'\',\''+str(have_img)+'\')')
            conn_obj1.commit()
            cursor1.close()
            conn_obj1.close()
            await bot.send(event,"已添加该违禁词")
            return 0
