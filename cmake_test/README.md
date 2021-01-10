## CMake test

This particular test does not use the standard Makefile system that majority of other tests use. Rather, we use CMake to compile this test.

CMake Build:
```
cmake ./ -D__VALUE__=<OFF/ON> -D__REFERENCE__=<OFF/ON> -D__POINTER__=<OFF/ON>
```

Build test using generated Makefile:
```
make
```

Run test:
```
./cmake_test.app
```
