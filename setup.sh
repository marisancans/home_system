sudo apt-get install libmongoc-1.0-0

cd /tmp

#  ------------------ LIBSON -----------
rm -rf libbson-1.3.5
rm libbson-1.3.5.tar.gz

wget https://github.com/mongodb/libbson/releases/download/1.3.5/libbson-1.3.5.tar.gz
tar -xzf libbson-1.3.5.tar.gz
cd libbson-1.3.5
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/lib/libson
make -j`nproc`
make install


# ------------------- LIBMONGO C -------------
rm -rf mongo-c-driver-1.16.2
rm mongo-c-driver-1.16.2.tar.gz

cd /tmp
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.16.2/mongo-c-driver-1.16.2.tar.gz
tar xzf mongo-c-driver-1.16.2.tar.gz
cd mongo-c-driver-1.16.2
mkdir build
cd build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_INSTALL_PREFIX=$HOME/lib/mongocdriver ..
make -j`nproc`
make install

cd /tmp


# ------------------- MONGOCXX -------------
rm -rf mongo-cxx-driver
git clone https://github.com/mongodb/mongo-cxx-driver.git \
    --branch releases/stable --depth 1
cd mongo-cxx-driver/build

cmake ..                                       \
    -DCMAKE_BUILD_TYPE=Release                 \
    -DCMAKE_PREFIX_PATH=$HOME/lib/mongocdriver \
    -DCMAKE_INSTALL_PREFIX=$HOME/lib/mongocxxdriver \
    -DBSONCXX_POLY_USE_BOOST=0                 \
    -DBSONCXX_POLY_USE_STD_EXPERIMENTAL=0      \

make -j`nproc`
make install
