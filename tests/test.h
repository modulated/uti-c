#define test_assert(message, test) do { if (!(test)) return message; } while (0)
#define test_run_test(test) do { char *message = test(); test_count++; if (message) return message; } while (0)
extern int test_count;