FROM ubuntu:latest
RUN mkdir /usr/src/MisakaBot
RUN mkdir /MisakaBot
COPY . /usr/src/MisakaBot
VOLUME ["/etc/MisakaBot","/MisakaBot/chatrecord"]
WORKDIR /usr/src/MisakaBot
RUN bash install_depend.sh
RUN bash install.sh 
RUN cp MisakaBot /MisakaBot/
ENTRYPOINT ["/MisakaBot/MisakaBot"]
