from mirai import Mirai, WebSocketAdapter, GroupMessage, Plain
import asyncio, traceback
import json
if __name__ == '__main__':
    bot = Mirai(
        qq=,
        adapter=WebSocketAdapter(
            verify_key='', host='', port=
        )
    )
    @bot.on(GroupMessage)
    async def on_group_message(event: GroupMessage):
        jf = json.loads(open("fc.json").read())
        for x in jf:
            try:
                cds = open(x['file']).read()
                glo = {"bot": bot}
                exec(cds, glo, glo)
                func = glo['on_group_message']
                retu = func(event)
                print("File:",x['file'], "Return: ", retu)
            except Exception as e:
                print(''.join(traceback.format_exception(None, e, e.__traceback__)))
            if type(retu).__name__ == 'coroutine':
                try:
                    await retu
                except Exception as e:
                    print(''.join(traceback.format_exception(None, e, e.__traceback__)))
bot.run()
