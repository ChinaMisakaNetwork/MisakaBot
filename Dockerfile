FROM ubuntu:latest
VOLUME ["/etc/MisakaBot","/MisakaBot_exec/chatrecord"]
RUN mkdir -p /usr/src/MisakaBot/
RUN mkdir -p /MisakaBot_exec/
WORKDIR /usr/src/MisakaBot
RUN apt update && apt install git -yqq && git clone https://github.com/ChinaMisakaNetwork/MisakaBot.git && cd MisakaBot && install_depend.sh && install.sh && cp MisakaBot /MisakaBot_exec/
ENTRYPOINT ["/MisakaBot_exec/MisakaBot"]
