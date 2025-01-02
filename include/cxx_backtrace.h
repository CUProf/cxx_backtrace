#ifndef __CXX_BACKTRACE_H__
#define __CXX_BACKTRACE_H__
#include <string>

extern "C" bool init_back_trace(const char *filename);
extern "C" void print_back_trace();
extern "C" std::string get_back_trace();

#endif // __CXX_BACKTRACE_H__
