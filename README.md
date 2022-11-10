# MisakaBot

一个基于mirai-cpp的机器人

## 使用方法

使用以下指令克隆本仓库

```bash
git clone https://github.com/ChinaMisakaNetwork/MisakaBot.git
cd MisakaBot
# 更新 mirai-cpp 到最新
git submodule update --init --force --remote
# 安装依赖库（mysql，mysql++）
bash install_depend.sh
# 编译本项目
bash install.sh
```
安装完成后可执行文件应位于当前目录下
## 配置文件
位于/etc/MisakaBot/MisakaBot.conf

请注意：配置项目顺序不能调换
## 支持的功能
```
# 如无特别说明，以下所指“管理”皆为在数据库中记录的管理权限
# 在群中授予这个群的管理权限
添加管理员 QQ号
# 在终端授予管理权限
添加管理员 群号 QQ号
```
1.聊天（支持自定义问答）
```
# 添加自定义问答
添加对话;{匹配对话正则表达式};{回答};0（无图片）
添加对话;{匹配对话正则表达式};{回答};1;{图片URL}（有图片回答）
```
2.一言 (hitokoto.cn)

3.敏感词撤回（机器人需要群管理员权限）【需要管理】
```
# 在群中添加
添加敏感词 $敏感词
# 在终端添加
添加敏感词 群号 $敏感词
```

4.群管理（踢出群成员/禁言群成员，机器人需要群管理员权限）【需要管理】
```
禁言 QQ号 禁言秒数
踢出 QQ号
```

5.数学计算（支持数学函数）

参考 http://www.partow.net/programming/exprtk/index.html

6.天气预报
```
格式：查询天气 $城市
```

7.入群欢迎（请编辑配置文件）

8.网易云搜歌/点歌
```
搜歌 $歌名（注意不能带空格）
唱歌 $歌曲ID

示例：
搜歌 DeatestDrop
唱歌 474574924
```

9.聊天记录

存放在与可执行文件同级下的chatrecord目录下

10.图片处理
```bash
# 提示：此功能为付费服务，请给DeepAI打钱获取apikey（）
# 请在配置文件中填写apikey
使用方法：指令+图片
指令有：图片上色、图片超分、图片降噪

以及：AI画图+描述，如：
AI画图 pineapple-pizza
（注：描述不可带空格，对中文支持可能不完全）
```
