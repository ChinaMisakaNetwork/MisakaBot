FROM ubuntu:latest
VOLUME ["/etc/MisakaBot","/MisakaBot/chatrecord"]
RUN mkdir /MisakaBot/
WORKDIR /usr/src/
RUN apt update
RUN apt install git -y --force-yes -qq;
RUN git clone https://github.com/ChinaMisakaNetwork/MisakaBot.git
WORKDIR /usr/src/MisakaBot
RUN install_depend.sh
RUN install.sh 
RUN cp MisakaBot /MisakaBot/
ENTRYPOINT ["/MisakaBot/MisakaBot"]
