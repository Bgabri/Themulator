/**
 * Usage: thml [options] <command>
 * Commands:
 *     download
 *         Starts an interface to login and download files from Themis.
 *     run
 *         Compiles and runs the program with the given option.
 * Options:
 *     --help, -h
 *         Display this information
 *     --compiler, -c
 *         The compiler to use
 *     --compiler-flags, -f
 *         The flags to pass to the compiler
 *     --dry-run, -d
 *         run without command execution
 *     --verbose
 *         print out all the execution steps
 *     --valgrind, -v
 *         runs the binary through valgrind
 *     --bin-name <name>, -b <name>
 *         the name to give to the outputted binary
 *     --dir <folder>, -D <folder>
 *         the folder of the source files to judge
 *     --in-dir, -I <folder>
 *         the folder to the input of the test cases
 *     --out-dir <folder>, -O <folder>
 *         the folder where the output is placed
 *     --bin-dir <folder>, -B <folder>
 *         the folder to output the binaries to
 *     --ref-dir <folder>, -R <folder>
 *         the folder with the reference outputs of the test cases
 */

// default options
#define _COMPILER "gcc"
#define _COMPILER_FLAGS "-std=c99 -pedantic -Wall"

#define _COMMAND run
#define _VERBOSE 0
#define _DRYRUN 0
#define _VALGRIND 0;

#define _DIR "."
#define _BIN_DIR "bin"
#define _BIN_NAME "program"
#define _IN_DIR "in"
#define _OUT_DIR "out"
#define _REF_DIR "ref"

typedef enum commandType {
    run, compile, download
} commandType;


typedef struct Options {
    char *compiler;
    char *compilerFlags;

    commandType command;
    short verbose;
    short dryRun;
    short valgrind;

    char *dir;
    char *binDir;
    char *binName;
    char *inDir;
    char *outDir;
    char *refDir;

} Options;


Options parseOptions(int argc, char *argv[]);
void printOptions(Options options);
