# ${\color{Dandelion}Themulator}$

Still very much a wip


## Install steps
```
git clone https://github.com/Bgabri/Themulator
cd Themulator
mkdir build
cd build
cmake ..
make
make install
```


## usage

To ensure Themulator works as intended in your project folder include a `in` and `ref` folder for the input and reference output of the testcases. The extension of the input files should end with `.in`. additionally its matching reference output should be named the same with `.out` as its extension, as shown below.
```
src/
|- in/
|  |- 1.in
|  |- 2.in
|
|- ref/
|  |- 1.out
|  |- 2.out
|
|- main.c

```

Then simply run `thml`.

### options
```
Usage: thml [options]
Options:
    -h              Display this information
    -d              run in dry-run mode: 
    -v              runs the binary through valgrind
    -b <name>       the name to give to the outputted binary
    -D <folder>     the folder of the source files to judge
    -I <folder>     the folder to the input of the test cases
    -O <folder>     the folder where the output is placed
    -B <folder>     the folder to output the binaries to
    -R <folder>     the folder with the reference outputs of the test cases
```

