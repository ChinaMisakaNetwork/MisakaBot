FROM ubuntu:latest
VOLUME ["/etc/MisakaBot","/MisakaBot/chatrecord"]
RUN install_depend.sh
RUN install.sh 
ENTRYPOINT ["/MisakaBot/MisakaBot"]
