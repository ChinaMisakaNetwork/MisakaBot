FROM ubuntu:latest
RUN mkdir /usr/src/MisakaBot
RUN mkdir /MisakaBot
COPY . /usr/src/MisakaBot
VOLUME ["/etc/MisakaBot","/MisakaBot/chatrecord"]
WORKDIR /usr/src/MisakaBot
RUN install_depend.sh
RUN install.sh 
RUN cp MisakaBot /MisakaBot/
ENTRYPOINT ["/MisakaBot/MisakaBot"]
