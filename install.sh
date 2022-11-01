mkdir build
git submodule update --init --force --remote
cd build
sudo cmake ..
cat /home/runner/work/MisakaBot/MisakaBot/build/CMakeFiles/CMakeError.log
sudo make 
