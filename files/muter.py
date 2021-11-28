from mirai import Mirai, WebSocketAdapter, GroupMessage, Plain
from  mirai.models.api import Mute
import requests
import flask, pymysql, json, datetime, mcrcon, yaml, threading, re
async def on_group_message(event: GroupMessage):
    if(str(event.message_chain).split()[0]=='$mute'):
        conn_obj = pymysql.connect(host='localhost', user='${YOUR SQL USER}', password="${YOUR SQL USER PWD}", database='miraiadmin', charset='utf8mb4')
        cursor=conn_obj.cursor()
        cursor.execute('select * from qqadmin where groupid='+str(event.sender.group.id)+' and adminqq='+str(event.sender.id))
        if(len(cursor.fetchall())==0):
            await bot.send(event,"您的权限不足！")
            return 0
        else:
            aimmmm=str(event.message_chain).split()[1]
            a= bot.mute(member_id=int(aimmmm),target=int(event.sender.group.id),time=1800)
            await a
            await bot.send(event,"已禁言"+str(aimmmm))
        conn_obj.close()
