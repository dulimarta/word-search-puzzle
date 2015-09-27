The files in this directory can be imported as a Clion project.

If you are using a different IDE, use `cmake` to generate
files for your IDE from `CMakeLists.txt`.

To compile directly from the command line:

```
g++ -std=c++11 main.cpp WordPuzzleGenerator.cpp WordPuzzleSolver.cpp -o runme
```

Then you can run the program by typing:

```
./runme    # on Linux/OSX
runme      # on Windows
```
