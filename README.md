SimSpark-SPL
============

SimSpark simulator for Standard Platform Legaue

Improvements
------------

* NAO V4 Model
 * Realistic motor (stiffness, current, temperature)
 * Two cameras
 * Accurate mass model
 * Kinematics parameters
 * Image camera (optional)

* For Standard Platform League
 * Field, goal, ball
 * Game rules


Installation
------------

* Linux

  * AppImage:

    Download the appimage from https://github.com/BerlinUnited/SimSpark-SPL/releases and make it executable

  * Compile manually

    * download/install dependencies (see https://gitlab.com/robocup-sim/SimSpark/wikis/Installation-on-Linux)

    ```sh
    git clone https://github.com/BerlinUnited/SimSpark-SPL.git simspark
    cd simspark/spark/
    mkdir build
    cd build
    cmake ..
    make
    make install
    cd ../../rcssserver3d/
    mkdir build
    cd build
    cmake ..
    make
    make install
    ```

    if you want to install simspark somewhere else, you can specify an environment variable and modify the `cmake` calls:

    ```sh
    SIMSPARK_INSTALL="/path/to/install"
    # ...
    cmake -DCMAKE_INSTALL_PREFIX:STRING="$SIMSPARK_INSTALL" ..
    # ...
    cmake -DCARBON_DATA_DIR:STRING="$SIMSPARK_INSTALL/share" -DCARBON_INCLUDE_DIR:STRING="$SIMSPARK_INSTALL/include/simspark" -DKEROSIN_INCLUDE_DIR:STRING="$SIMSPARK_INSTALL/include/simspark" -DOXYGEN_INCLUDE_DIR:STRING="$SIMSPARK_INSTALL/include/simspark" -DRCSSNET3D_INCLUDE_DIR:STRING="$SIMSPARK_INSTALL/include/simspark" -DSALT_INCLUDE_DIR:STRING="$SIMSPARK_INSTALL/include/simspark" -DSPARK_INCLUDE_DIR:STRING="$SIMSPARK_INSTALL/include/simspark" -DCMAKE_INSTALL_PREFIX:STRING="$SIMSPARK_INSTALL" ..
    # ...
    ```

* Windows

  TODO!
