#libcxx_includes='/usr/local/bin/../lib/'

#g++ *.cpp -o build/RayTracer -fopenmp -I/usr/local/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -isystem ${libcxx_includes}/include/c++/v1 -nostdinc++ -nodefaultlibs -lc -lc++ -lc++abi -lgcc_s.10.5 -Wl,-no_compact_unwind -std=c++11 $(pkg-config --cflags --libs opencv)

clang ../*.cpp -o RayTracer -fopenmp -std=c++11 $(pkg-config --cflags --libs opencv) -fno-exceptions 
