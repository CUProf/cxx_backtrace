#ifndef __CXX_BACKTRACE_H__
#define __CXX_BACKTRACE_H__
#include <string>
#include <vector>

extern "C" bool init_backtrace(const char *filename);
extern "C" std::string print_backtrace(int verbose = 1);
extern "C" std::vector<std::string> get_backtrace(int keep = -1);

#endif // __CXX_BACKTRACE_H__
