# jsonpp

Simple JSON parser written in C++

## Building

Whatever your platform is, make sure you have these things first:

* [CMake](https://cmake.org/)
* [vcpkg](https://vcpkg.io/en/)
* `VCPKG_ROOT` environment variable pointing to the directory where vcpkg is bootstrapped

### Windows

* Make sure you have C++ build tools (available with Visual Studio).
* Install fmt package with vcpkg (`vcpkg install fmt`)

After that you can just execute `build.bat` and executable will be built in `build/Debug` directory.

## License

MIT
