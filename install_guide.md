# How to set OpenGL in Visual Studios environment

## Download files
1. Download [GLFW 32-bit Windows binaries](www.glfw.org/download.html) and unzip
2. Download [GLEW Windows binaries](glew.sourceforge.net) and unzip

## Setup directories
1. Create **Dependencies > GLFW** folders in the project directory
1. Copy **include** and **lib-vc[your vs ver.]** folders from downloaded GLFW
2. Paste the folders in the **Dependencies > GLFW** folder
3. Change **glew-2.1.0** folder name into **GLEW** 
4. Move **GLEW** folder into **Dependencies folder**

## Setup in Visual Studios
From the menu bar, **Project > Properties**

1. C/C++ > General > Additional include directories:
```
$(SolutionDir)Dependencies\GLFW\include
$(SolutionDir)Dependencies\GLEW\include
```


2. Linker > General > Additional library directories:
```
$(SolutionDir)Dependencies\GLFW\lib-vc2015 
$(SolutionDir)Dependencies\GLEW\lib\Release\Win32
```


3. Linker > Input > Additional dependencies:
```
glfw3.lib, opengl32.lib
glew32s.lib
```
​
4. C/C++ > Preprocessor: add **GLEW_STATIC**  
5. Add **#include<GL/glew.h> #include<GLFW/glfw3.h>** in source code
