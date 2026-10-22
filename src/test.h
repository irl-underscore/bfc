#ifndef TEST_H
#define TEST_H

#if defined(_TEST)
    #define TEST_LOG_FMT(fmt, ...) printf("(%s) " fmt "\n", __FUNCTION__, __VA_ARGS__)
    #define TEST_LOG(msg) printf("(%s) %s\n", __FUNCTION__, msg)
#else
    #define TEST_LOG_FMT(...)
    #define TEST_LOG(...)
#endif /* _TEST */

#endif /* TEST_H */
