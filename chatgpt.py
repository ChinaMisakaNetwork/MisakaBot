import tornado.ioloop
import tornado.web
import asyncio
import json
from revChatGPT.ChatGPT import Chatbot
config = {
    "session_token":""
}
class MainHandler(tornado.web.RequestHandler):
    def post(self):
        payload = json.loads(self.request.body)
        param=payload["message"]
        cvid=payload["conversation_id"]
        #print(param)
        #print(cvid)
        pidd=payload["parent_id"]
        chatbot=0
        #print(pidd)
        if cvid=="0":
            chatbot = Chatbot(config,conversation_id=None)
        else:
            chatbot = Chatbot(config,conversation_id=cvid,parent_id=pidd)
        reply_json = (chatbot.get_chat_response(param))
        #print(reply_json['conversation_id'])
        message = reply_json['message']
        #mapping['groupid']=reply_json['conversation_id']
        ret = {}
        ret['message']=message
        ret['conversation_id']=reply_json['conversation_id']
        ret['parent_id']=reply_json['parent_id']
        self.write(json.dumps(ret))
def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
    ])

if __name__ == "__main__":
    app = make_app()
    app.listen(1234)
    tornado.ioloop.IOLoop.current().start()
