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
$mute QQ号 禁言秒数
$kick QQ号
```

5.数学计算（支持数学函数）

参考 http://www.partow.net/programming/exprtk/index.html

6.聊天记录（TODO）

7.（还没想好）
