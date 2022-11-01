sudo apt update;
sudo apt upgrade -y --force-yes -qq;
sudo apt install g++ gcc -y --force-yes -qq;
sudo apt install mysql-server -y --force-yes -qq;
sudo apt install libmysqlclient-dev -y --force-yes -qq;
sudo apt install libmysql++-dev -y --force-yes -qq;
cd /tmp;
wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.0.31-linux-glibc2.27-x86-64bit-debug.tar.gz
tar -zxvf mysql-connector-c++-8.0.31-linux-glibc2.27-x86-64bit-debug.tar.gz
cp mysql-connector-c++-8.0.31-linux-glibc2.27-x86-64bit-debug/lib64/* /usr/lib
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz;
tar -zxvf mysql++-3.3.0.tar.gz;
ln -s /usr/local/mysql/lib/mysql/libmysqlclient.so.15.0.0 /usr/local/mysql/lib/mysql/libmysqlclient_r.so
cd mysql++-3.3.0
sudo ./configure --prefix=/usr --prefix=/usr/local --enable-thread-check LDFLAGS='-pthread'
mv Makefile.simple Makefile;
sudo make install
