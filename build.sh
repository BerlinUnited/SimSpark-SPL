#!/bin/bash
# Simple build and install script

# Build SimSpark
cd spark
mkdir build
cd build
cmake ..
make -j4
sudo make install

# Build rcssserver3d
cd ../../rcssserver3d
mkdir build
cd build
cmake ..
make -j4
sudo make install

# Configure library paths
echo -e '/usr/local/lib/simspark\n/usr/local/lib/rcssserver3d' | sudo tee /etc/ld.so.conf.d/spark.conf
sudo ldconfig

