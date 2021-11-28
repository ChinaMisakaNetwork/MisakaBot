from mirai import Mirai, WebSocketAdapter, GroupMessage, Plain
import requests
import flask, pymysql, json, datetime, mcrcon, yaml, threading, re
async def on_group_message(event: GroupMessage):
    if(str(event.message_chain).split()[0]=="添加对话"):
        if(event.sender.id in [${YOUR QQ NUMBER ADMIN}]):
            conn_obj = pymysql.connect(host='localhost', user='${YOUR SQL USER}', password="${YOUR SQL USER PWD}", database='miraichat', charset='utf8mb4')
            cursor=conn_obj.cursor()
            question = str(event.message_chain).split(' ')[1]
            try:
                re.compile(question)
            except re.error as e:
                await bot.send(event, str(e))
                return 0
            except:
                pass
            answer = str(event.message_chain).split(' ')[2]
            have_img = int(str(event.message_chain).split(' ')[3])
            if(have_img==1):
                img_url = str(event.message_chain).split(' ')[4]
                img_url=img_url.replace('\\','',1)
                cursor.execute('insert into miraichat(question, answer, have_img, img_url) values (%s, %s, %s, %s)', [question, answer, have_img, img_url])
#                    cursor.execute('insert into miraichat(question,answer,have_img,img_url) values (\''+ question + '\',\''+answer+'\',\''+str(have_img)+'\',\''+img_url+'\')')
            else:
                cursor.execute('insert into miraichat(question, answer, have_img) values (%s, %s, %s)', [question, answer, have_img])
#                    cursor.execute('insert into miraichat(question,answer,have_img) values (\''+ question + '\',\''+answer+'\',\''+str(have_img)+'\')')
            conn_obj.commit()
            cursor.close()
            conn_obj.close()
