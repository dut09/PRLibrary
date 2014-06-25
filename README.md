PRLibrary
=========

## Usage: ##

### Step 1: ###
copy all the contents in libpr/include/ into your project

### Step 2: ###
add libpr/include and libpr/include/3rdparty into the include path of your project

### Step 3: ###
open sample/lib, copy all the *.lib files into your project. In debug mode, add: libpr.lib, opencv_core249d.lib and opencv_highgui249d.lib; In release mode, add: libpr.lib, opencv_core249.lib and opencv_highgui249.lib.

### Sample project: ###
There is a sample project in sample/, running this project will generate a rendered Stanford Bunny image.
