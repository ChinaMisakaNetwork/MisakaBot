FROM ubuntu:latest
VOLUME ["/etc/MisakaBot","/MisakaBot/chatrecord"]
RUN mkdir /MisakaBot/
WORKDIR /usr/src/
RUN apt-get update
RUN apt-get install git
RUN git clone https://github.com/ChinaMisakaNetwork/MisakaBot.git
WORKDIR /usr/src/MisakaBot
RUN install_depend.sh
RUN install.sh 
RUN cp MisakaBot /MisakaBot/
ENTRYPOINT ["/MisakaBot/MisakaBot"]
