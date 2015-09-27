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

To generate a puzzle of size 50x80, 30% of words, save the puzzle and key
files in puz.txt and key.txt:

```
./runme -g -s 50 80 -p 30 -z puz.txt -k key.txt
```

To solve a puzzle:

```
./runme -s puz.txt
./runme -s puz.txt | diff - key.out  # only on Linux/OSX
```
