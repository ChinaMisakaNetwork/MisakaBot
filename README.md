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
# 编译并安装本项目
bash install.sh
```
安装完成后可执行文件应位于MisakaBot目录下