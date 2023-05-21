# Supernova engine

## Building
```
mkdir build
cd build
cmake ..
make install
```

## Known Issues
* The project won't compile in termux. For fixing compile with `cmake .. -DCMAKE_CXX_FLAGS=-U__ANDROID__`.
