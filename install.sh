sudo apt update;
sudo apt upgrade -y --force-yes -qq;
sudo apt install g++ -y --force-yes -qq;
sudo apt install gcc -y --force-yes -qq;
sudo apt install mysql-server -y --force-yes -qq;
sudo apt install libmysql++-dev -y --force-yes -qq;
sudo apt install cmake systemtap-sdt-dev manpages-posix-dev wget curl -y --force-yes -qq;
mkdir ./tmp;
cd ./tmp;
wget https://dev.mysql.com/get/Downloads/Connector-C++/libmysqlcppconn9_8.0.31-1ubuntu22.10_amd64.deb;
sudo dpkg -i libmysqlcppconn9_8.0.31-1ubuntu22.10_amd64.deb;
sudo ln -s /usr/lib/x86_64-linux-gnu/libmysqlclient.so /usr/lib/x86_64-linux-gnu/libmysqlclient_r.so
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz;
tar -zxvf mysql++-3.3.0.tar.gz;
cd mysql++-3.3.0
sudo ./configure --prefix=/usr --enable-thread-check LDFLAGS='-pthread'
sudo make
sudo make install
cd ../../
git submodule update --init --force --remote
mkdir build
cd build
sudo make clean
sudo cmake ..
sudo make 
