!#/bin/bash
apt update;
apt upgrade;
apt install mysql-server;
apt install libmysqlclient-dev;
cd /tmp;
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz;
tar -zxvf mysql++-3.3.0.tar.gz;
cd mysql++-3.3.0
./configure --prefix=/usr --enable-thread-check LDFLAGS='-pthread'
mv Makefile.simple Makefile;
make
make install