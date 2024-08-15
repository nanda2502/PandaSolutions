This project aims to auto-generate methods and results sections for basic psychology research papers. 
Scripts are written in .pnd files that are lexed and parsed in c++ and produce an R markdown file that can be rendered into a pdf.
The project is currently unfinished.

## Build Instructions

1. Ensure you have CMake (version 3.28 or higher) and a C++ compiler installed.
2. Clone the repository:
```
git clone https://github.com/nanda2502/PandaSolutions.git
cd PandaSolutions
```
3. Create a build directory:
```
mkdir build && cd build
```
4. Configure and build the project:
```
cmake ..
make
```
5. Run the project:
create a file `input.pnd` in the root directory and run the executable:

```
./PandaSolutions
```