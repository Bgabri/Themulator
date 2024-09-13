
typedef struct Options {
    
    char *compiler;
    char *compilerFlags;

    short silent;
    short dryRun;

    char *dir;
    char *binDir;
    char *binName;
    char *inDir;
    char *outDir;
    char *refDir;

} Options;


Options parseOptions(int argc, char *argv[]);


