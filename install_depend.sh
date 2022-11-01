sudo apt update;
sudo apt upgrade -y --force-yes -qq;
sudo apt install g++ -y --force-yes -qq;
sudo apt install gcc -y --force-yes -qq;
sudo apt install mysql-server -y --force-yes -qq;
sudo apt install libmysql++-dev -y --force-yes -qq;
sudo apt install cmake systemtap-sdt-dev -y --force-yes -qq;
cd /tmp;
wget https://dev.mysql.com/get/Downloads/Connector-C++/libmysqlcppconn9_8.0.31-1ubuntu22.10_amd64.deb;
sudo dpkg -i libmysqlcppconn9_8.0.31-1ubuntu22.10_amd64.deb;
cd /usr/lib/x86_64-linux-gnu/
sudo ln -s libmysqlclient.so libmysqlclient_r.so
sudo ln -s /usr/include/x86_64-linux-gnu/sys/ /usr/include/
cd /tmp
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz;
tar -zxvf mysql++-3.3.0.tar.gz;
cd mysql++-3.3.0
sudo ./configure --prefix=/usr --prefix=/usr/local --enable-thread-check LDFLAGS='-pthread'
sudo mv Makefile.simple Makefile;
sudo make install
