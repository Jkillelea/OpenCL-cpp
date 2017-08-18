#ifndef UTIL_H_
#define UTIL_H_

#include "cl_version.h"

#include <iostream>
#include <fstream>
#include <CL/cl2.hpp>

void check_error();
void print_build_info(cl::Program &program, bool print_banner = false);
void print_dev_info(cl::Device &dev,        bool print_banner = false);
std::string read_to_string(const char *fname);

#endif /* end of include guard: UTIL_H_ */
