# MatrixModule
A simple library which adds easy to use matrix and vector functionality to C++.
  ## Features
  - Operator overloads for matrix arithmetic
  - Convolution operation
  - Load and save vectors and matrices from and to IDX files

# Usage
Due to the simplicity of this library, there are multiple possible ways to include the library in your project. I would recommend doing either of the following:
## Method 1 - Include the files:
Simply download the .h and .cpp files located within the root directory and include them directly within your project.

## Method 2 - Build as a static library:
Build the library with CMake using the following commands within the root directory of the project:
```
mkdir bin
cd bin
cmake ..
make
```


  
