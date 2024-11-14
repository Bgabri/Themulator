# ${\color{Dandelion}Themulator}$
A cli demake of Themis, the programing judge.

## Examples
An example of running `thml` in `src/tests/sample`.

![thml run example](run.png)
An example of running `thml download` in `src/tests/sample`.

![thml download example](download.png)

## Install steps
Themulator depends on the libcurl headers to be installed.

eg. on ubuntu: `apt-get install libcurl4-openssl-dev`
```
git clone https://github.com/Bgabri/Themulator
cd Themulator
mkdir build
cd build
cmake ..
make
make install
```


## Usage

Our coding monkeys do not guarantee the functionality of Themulator on non linux based systems.

### Default Folder Structure
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
### Running
Then simply run `thml`.

To download the test cases from themis use `thml download` then follow the prompts.

### options
```
Usage: thml [options] <command>
Commands:
    download
        Starts an interface to login and download files from Themis.
    run
        Compiles and runs the program with the given option.
    interpret
        interpret the program with the given option.
Options:
    --help, -h
        Display this information
    --compiler, -c
        The compiler or interpreter to use
    --compiler-flags, -f
        The flags to pass to the compiler
    --dry-run, -d
        run without command execution
    --verbose
        print out all the execution steps
    --quick-exit, -q
        exits immediately and prints error on runtime error
    --valgrind, -v
        runs the binary through valgrind
    --bin-name <name>, -b <name>
        the name to give to the outputted binary
    --dir <folder>, -D <folder>
        the folder of the source files to judge
    --in-dir, -I <folder>
        the folder to the input of the test cases
    --out-dir <folder>, -O <folder>
        the folder where the output is placed
    --bin-dir <folder>, -B <folder>
        the folder to output the binaries to
        will also be used as the cookie output path
    --ref-dir <folder>, -R <folder>
        the folder with the reference outputs of the test cases
```



# TOXO
 timer/maxtime
 quick-exit
 cfg
