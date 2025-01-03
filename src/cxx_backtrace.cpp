#include "cxx_backtrace.h"
#include <cstdlib>
#include <backtrace.h>
#include <cxxabi.h>

void *__bt_state = nullptr;

std::string backtrace_str;
int bt_index = 0;

int bt_callback(void *, uintptr_t, const char *filename, int lineno, const char *function) {
  /// demangle function name
  const char *func_name = function;
  int status;
  char *demangled = abi::__cxa_demangle(function, nullptr, nullptr, &status);
  if (status == 0) {
    func_name = demangled;
  }

  if (filename && func_name) {
    printf("%s:%d in function %s\n", filename, lineno, func_name);
  }
  return 0;
}

int get_bt_callback(void *, uintptr_t, const char *filename, int lineno, const char *function) {
  /// demangle function name
  const char *func_name = function;
  int status;
  char *demangled = abi::__cxa_demangle(function, nullptr, nullptr, &status);
  if (status == 0) {
    func_name = demangled;
  }

  if (filename && func_name) {
    backtrace_str += "b-" + std::to_string(bt_index) + " " + std::string(filename) + ":" + std::to_string(lineno) + " " + func_name + "\n";
    bt_index++;
  }
  return 0;
}

void bt_error_callback(void *, const char *msg, int errnum) {
  printf("Error %d occurred when getting the stacktrace: %s", errnum, msg);
}

void bt_error_callback_create(void *data, const char *msg, int errnum) {
  printf("Error %d occurred when initializing the stacktrace: %s", errnum, msg);
  bool *status = (bool *)data;
  *status = false;
}

bool init_back_trace(const char *filename) {
  bool status = true;
  __bt_state = backtrace_create_state(filename, 0, bt_error_callback_create, (void *)status);
  return status;
}

void print_back_trace() {
  if (!__bt_state) { /// make sure init_back_trace() is called
    printf("Make sure init_back_trace() is called before calling print_stack_trace()\n");
    abort();
  }
  backtrace_full((backtrace_state *)__bt_state, 0, bt_callback, bt_error_callback, nullptr);
}

std::string get_back_trace() {
  if (!__bt_state) { /// make sure init_back_trace() is called
    printf("Make sure init_back_trace() is called before calling print_stack_trace()\n");
    abort();
  }
  backtrace_str.clear();
  backtrace_full((backtrace_state *)__bt_state, 0, get_bt_callback, bt_error_callback, nullptr);

  return backtrace_str;
}