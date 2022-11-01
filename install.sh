mkdir build
git submodule update --init --force --remote
cd build
sudo make clean
sudo cmake ..
sudo make 
