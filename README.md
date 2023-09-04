# Supernova engine
<p align="center">
  <img src="res/banner.png" />
</p>

## Building
Clone this repository and run the these commands in the project root.
```
mkdir build
cd build
cmake ..
make install
```

## Known Issues
* The project won't compile in termux. For fixing compile with `cmake .. -DCMAKE_CXX_FLAGS=-U__ANDROID__`.
