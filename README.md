# RayTrace
===

a 3D ray-tracing render. This was once one of my term projects for *advanced computer graphics* course.

### Usage
+ move to the build directory
+ execute make
+ ./rayTracer output.jpg scene.json

### Features
+ Basic ray tracing
+ Anti-Aliasing
+ Distributed computation
+ Support Json scene description file
+ Octree acceleration for complex obj models
+ Smoothing obj faces by interpolating normals
+ Soft-Shadows
+ Depth of field
+ Cached-Sampler acceleration

### Acknowledgements
+ Picojson: really nice json library
+ kixor obj loader
	+ I've made some small fixes to it
+ OpenCV lib
+ Blah-Geek
