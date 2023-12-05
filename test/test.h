#pragma once

#include <cstdio>

extern struct test_data_t {
  unsigned int ran, fail, pass;
  const char* name;
  unsigned int current_line;
  const char* file;
  const char* expr;
} test_data;

struct test_data_t test_data;

void init_test(const char * name) {
  test_data.ran = 0;
  test_data.pass = 0;
  test_data.fail = 0;
  test_data.name = name;
}

int report() {

  printf("[\033[ 1;33mTEST\033[0m] \033[1;37m%s\033[0m: ", test_data.name);
  //if (test_data.sig > 0) {
  //  printf("\033[91mUnexpected termination\033[0m.\n");
  //}
  //else
  if  (test_data.pass == test_data.ran)
    printf(
      "%d of %d \033[92mpassing\033[0m.\n",
      test_data.pass,
      test_data.ran);
  else
    printf(
      "%d of %d passing, %d of %d \033[91mfails\033[0m.\n",
      test_data.pass,
      test_data.ran,
      test_data.fail,
      test_data.ran);

  return test_data.ran - test_data.pass;
}

#define TEST(expr)                                                  \
  do {                                                              \
    test_data.ran++;                                              \
    test_data.fail++;                                               \
    if ((expr)) {                                                   \
      test_data.fail --;                                            \
      test_data.pass ++;                                            \
    } else {                                                        \
      if (test_data.fail == 1) printf("\n");                        \
      fprintf(stderr, "\033[91mFAIL\033[0m [%s:%d]: !(%s)\n",       \
              __FILE__, __LINE__, #expr);                           \
    }                                                               \
  }while (false);
