cmake -G"Visual Studio 15 2017 Win64" -H. -BBuild -Dgtest_force_shared_crt=ON -DUSE_FOLDERS=ON
cmake --build Build