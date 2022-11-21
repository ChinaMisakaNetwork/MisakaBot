FROM ubuntu:latest
VOLUME ["/etc/MisakaBot","/MisakaBot_exec/chatrecord"]
RUN mkdir -p /MisakaBot_exec/
RUN install_depend.sh
RUN install.sh 
RUN cp MisakaBot /MisakaBot_exec/
ENTRYPOINT ["/MisakaBot_exec/MisakaBot"]
