sudo apt update;
sudo apt upgrade -y --force-yes -qq;
sudo apt install g++ gcc -y --force-yes -qq;
sudo apt install mysql-server -y --force-yes -qq;
sudo apt install libmysql++-dev -y --force-yes -qq;
sudo apt install cmake -y --force-yes -qq;
cd /tmp;
wget https://dev.mysql.com/get/Downloads/Connector-C++/libmysqlcppconn9_8.0.31-1ubuntu22.10_amd64.deb;
dpkg -i libmysqlcppconn9_8.0.31-1ubuntu22.10_amd64.deb;
cd /usr/lib/x86_64-linux-gnu/
ln -s libmysqlclient.so libmysqlclient_r.so
cd /tmp
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz;
tar -zxvf mysql++-3.3.0.tar.gz;
ln -s /usr/local/mysql/lib/mysql/libmysqlclient.so.15.0.0 /usr/local/mysql/lib/mysql/libmysqlclient_r.so
cd mysql++-3.3.0
sudo ./configure --prefix=/usr --prefix=/usr/local --enable-thread-check LDFLAGS='-pthread'
cat /home/runner/work/MisakaBot/MisakaBot/build/CMakeFiles/CMakeError.log
mv Makefile.simple Makefile;
sudo make install
