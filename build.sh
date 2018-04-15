cd spark
mkdir build
cd build
cmake ..
make -j
sudo make install
sudo ldconfig

cd ../../rcssserver3d
mkdir build
cd build
cmake ..
make -j
sudo make install
sudo ldconfig

