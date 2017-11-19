#include "cl_version.h"
#include "CL/cl2.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "util.hpp"
cl_int err;

using std::string;
using std::vector;

int main(int argc, char const *argv[]) {
  vector<cl::Platform> platforms;
  vector<cl::Device> devices;

  cl::Platform::get(&platforms);

  cl::Platform platform = platforms[0];
  platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

  cl::Device device = devices[0];

  string src = read_to_string("helloworld.cl");

  // typedef vector<std::pair<const char*, size_type> > Sources;
  std::cout << "[sources]\n";
  cl::Program::Sources sources(1, src);

  std::cout << "[context]\n";
  cl::Context context(device);

  std::cout << "[program]\n";
  cl::Program program(context, src);
  check_error();


  std::cout << "[program.build]\n";
  err = program.build(CL_BUILD_VERSION_TARGET);
  // print_build_info(program, true);

  char buf[16];
  cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));

  std::cout << "[kernel]\n";
  cl::Kernel kernel(program, "helloworld", &err);
  check_error();

  kernel.setArg(0, memBuf);

  std::cout << "[command queue]\n";
  cl::CommandQueue queue(context);

  // global range of 1 (1 instance only)
  queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1));
  queue.enqueueReadBuffer(memBuf, true, 0, sizeof(buf), buf);
  queue.finish();

  std::cout << buf;

  return 0;
}
