CC = icc
CLANG = clang
# CV_FLAGS = $(pkg-config --cflags --libs opencv)
CV_FLAGS = -I/usr/local/Cellar/opencv/2.4.9/include/opencv -I/usr/local/Cellar/opencv/2.4.9/include /usr/local/Cellar/opencv/2.4.9/lib/libopencv_calib3d.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_contrib.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_core.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_features2d.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_flann.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_gpu.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_highgui.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_imgproc.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_legacy.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_ml.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_nonfree.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_objdetect.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_ocl.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_photo.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_stitching.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_superres.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_ts.a /usr/local/Cellar/opencv/2.4.9/lib/libopencv_video.dylib /usr/local/Cellar/opencv/2.4.9/lib/libopencv_videostab.dylib
CC_FLAGS = -fopenmp -std=c++11 -fno-exceptions

EXEC = rayTracer
SOURCES = $(wildcard ../*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CV_FLAGS) $(CC_FLAGS)

# Release
release: $(OBJECTS)
	$(CC) $(OBJECTS) -o rayTracerRelease $(CV_FLAGS) $(CC_FLAGS) -O2

clangRelease: $(OBJECTS)
	$(CLANG) $(OBJECTS) -stdlib=libc++ -o rayTracer $(CV_FLAGS) $(CC_FLAGS) -O2

# Generic rule
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)


