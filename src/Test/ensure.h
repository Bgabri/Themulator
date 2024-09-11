
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                 USAGE                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                       *
 * Repo: https://github.com/Bgabri/Ensure                                *
 *                                                                       *
 * A compile/run time assertion checker with unique exit errors.         *
 *                                                                       *
 * WARNING: A counter is used to give the assertions unique exit errors  *
 * starting from 0 with a maximum of 255. The counter is reset between   *
 * files, so it is recommended to offset the counter using the           *
 * INCREMENT_COUNT(_100, _10, _1) macro at the top of the file.          *
 *                                                                       *
 *                                                                       *
 * Simply invoke ensure(expression)                                      *
 *                                                                       *
 * INCREMENT_COUNT(0, 4, 2); // offset the counter by 42                 *
 * ensure(1 + 1 == 2); // holds true, does not exit with 42               *
 * ensure(1 + 1 == 3); // holds false, exits with 43                      *
 *                                                                       *
 *                                                                       *
 * To check the corrsponding exit error of an ensure use the following   *
 * compiler flags.                                                       *
 *                                                                       *
 * At compile time:                                                      *
 *     -D which_compile_error                                            *
 *                                                                       *
 * At run time:                                                          *
 *     -D which_run_error                                                *
 *                                                                       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ENSURE_H
#define ENSURE_H


/* * * * * * * * * * * * * * * * * * * * *
 *             ANSI colours              *
 * * * * * * * * * * * * * * * * * * * * */
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/* * * * * * * * * * * * * * * * * * * * *
 *                Offset                 *
 * * * * * * * * * * * * * * * * * * * * */
#define COUNT_0_0() 0
#define COUNT_0_1()  COUNT_0_0() + __COUNTER__
#define COUNT_0_2()  COUNT_0_1() + __COUNTER__
#define COUNT_0_3()  COUNT_0_2() + __COUNTER__
#define COUNT_0_4()  COUNT_0_3() + __COUNTER__
#define COUNT_0_5()  COUNT_0_4() + __COUNTER__
#define COUNT_0_6()  COUNT_0_5() + __COUNTER__
#define COUNT_0_7()  COUNT_0_6() + __COUNTER__
#define COUNT_0_8()  COUNT_0_7() + __COUNTER__
#define COUNT_0_9()  COUNT_0_8() + __COUNTER__
#define COUNT_0_10() COUNT_0_9() + __COUNTER__

#define COUNT_1_0()  0
#define COUNT_1_1()  COUNT_1_0() + COUNT_0_10()
#define COUNT_1_2()  COUNT_1_1() + COUNT_0_10()
#define COUNT_1_3()  COUNT_1_2() + COUNT_0_10()
#define COUNT_1_4()  COUNT_1_3() + COUNT_0_10()
#define COUNT_1_5()  COUNT_1_4() + COUNT_0_10()
#define COUNT_1_6()  COUNT_1_5() + COUNT_0_10()
#define COUNT_1_7()  COUNT_1_6() + COUNT_0_10()
#define COUNT_1_8()  COUNT_1_7() + COUNT_0_10()
#define COUNT_1_9()  COUNT_1_8() + COUNT_0_10()
#define COUNT_1_10() COUNT_1_9() + COUNT_0_10()

#define COUNT_2_0()  0
#define COUNT_2_1()  COUNT_2_0() + COUNT_1_10()
#define COUNT_2_2()  COUNT_2_1() + COUNT_1_10()
#define COUNT_2_3()  COUNT_2_2() + COUNT_1_10()
#define COUNT_2_4()  COUNT_2_3() + COUNT_1_10()
#define COUNT_2_5()  COUNT_2_4() + COUNT_1_10()
#define COUNT_2_6()  COUNT_2_5() + COUNT_1_10()
#define COUNT_2_7()  COUNT_2_6() + COUNT_1_10()
#define COUNT_2_8()  COUNT_2_7() + COUNT_1_10()
#define COUNT_2_9()  COUNT_2_8() + COUNT_1_10()
#define COUNT_2_10() COUNT_2_9() + COUNT_1_10()

// :p
// int __total_count__LINE__COUNT = SUM;
#define INCREMENT_COUNT(_100, _10, _1) \
    int CONCAT(CONCAT(__total_count__, __LINE__), CONCAT(__,CONCAT(CONCAT(_100, _10), _1))) = COUNT_0_##_1() + \
    COUNT_1_##_10() + \
    COUNT_2_##_100()


/* * * * * * * * * * * * * * * * * * * * *
 *         Compile Time Warnings         *
 * * * * * * * * * * * * * * * * * * * * */
#define _CONCAT(a,b) a##b
#define CONCAT(a,b) _CONCAT(a, b)

#define _STRING(a) #a
#define STRING(a) _STRING(a)

#define WARN(msg, count) __glibc_macro_warning(WHAT_THE(msg, count))

#define WHAT_THE(exp, count) STRING(Assertion\x1b[1m exp\x1b[0m exits with\x1b[1m count)



#ifdef which_compile_error
    // compiler warning
    #define DOUBLE(expr, count); { \
        WARN(expr, count) \
        ensureFailed((expr), #expr, count, __FILE__, __LINE__, __func__); \
    }
#else
    #define DOUBLE(expr, count); { \
        ensureFailed((expr), #expr, count, __FILE__, __LINE__, __func__); \
    }
#endif


#define ensure(expr) DOUBLE(expr, __COUNTER__)

void ensureFailed(int expr, char *assertion, unsigned char count, char *file, unsigned int line, const char *function);

#endif // ENSURE_H

