sudo apt update;
sudo apt upgrade -y --force-yes -qq;
sudo apt install g++ gcc libgcc libg++ 
sudo apt install mysql-server;
sudo apt install libmysqlclient-dev;
sudo apt install libmysql++-dev;
cd /tmp;
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz;
tar -zxvf mysql++-3.3.0.tar.gz;
cd mysql++-3.3.0
sudo ./configure --prefix=/usr --prefix=/usr/local --enable-thread-check LDFLAGS='-pthread'
mv Makefile.simple Makefile;
sudo make install
