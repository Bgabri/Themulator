

// default options
#define _COMPILER "gcc"
#define _COMPILER_FLAGS "-std=c99 -pedantic -Wall"

#define _VERBOSE 0
#define _DRYRUN 0
#define _VALGRIND 0;

#define _DIR "."
#define _BIN_DIR "bin"
#define _BIN_NAME "program"
#define _IN_DIR "in"
#define _OUT_DIR "out"
#define _REF_DIR "ref"

typedef struct Options {
    char *compiler;
    char *compilerFlags;

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
