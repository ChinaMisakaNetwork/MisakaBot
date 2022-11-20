FROM ubuntu:latest
RUN mkdir /usr/src/MisakaBot
RUN mkdir /MisakaBot
VOLUME ["/etc/MisakaBot","/MisakaBot/chatrecord"]
WORKDIR /usr/src/MisakaBot
COPY . .
RUN install_depend.sh
RUN install.sh 
RUN cp MisakaBot /MisakaBot/
ENTRYPOINT ["/MisakaBot/MisakaBot"]
