#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <iostream>
#include <vector>
#include <string>

#include "util.hpp"
#include "cl_version.h"

#include <CL/cl2.hpp>

cl_int err;

const int VEC_LEN = 1024;
std::vector<float> a;
std::vector<float> b;
std::vector<float> c;

int main(int argc, char const *argv[]) {

  // generate arrays of random numbers
  srand(time(NULL));
  for (size_t i = 0; i < VEC_LEN; i++) {
    a.push_back((float) rand());
    b.push_back((float) rand());
    c.push_back(0);
  }

  std::vector<cl::Platform> platforms;
  std::vector<cl::Device>   devices;

  cl::Platform::get(&platforms);

  cl::Platform platform = platforms[0];

  platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

  cl::Device dev = devices[0];

  cl::Context context(dev);

  std::string src = read_to_string("vadd.cl");
  cl::Program program(context, src);
  err = program.build(CL_BUILD_VERSION_TARGET);
  check_error();
  print_build_info(program, true);

  cl::Buffer d_a(context, CL_MEM_READ_ONLY  | CL_MEM_HOST_WRITE_ONLY, VEC_LEN * sizeof(float));
  cl::Buffer d_b(context, CL_MEM_READ_ONLY  | CL_MEM_HOST_WRITE_ONLY, VEC_LEN * sizeof(float));
  cl::Buffer d_c(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,  VEC_LEN * sizeof(float));

  cl::Kernel vadd(program, "vadd", &err);
  check_error();

  vadd.setArg(0, d_a);
  vadd.setArg(1, d_b);
  vadd.setArg(2, d_c);

  cl::CommandQueue queue(context);

  cl::NDRange global(VEC_LEN);

  // load data onto the GPU
  queue.enqueueWriteBuffer(d_a, true, 0, VEC_LEN * sizeof(float), a.data());
  queue.enqueueWriteBuffer(d_b, true, 0, VEC_LEN * sizeof(float), b.data());

  queue.enqueueNDRangeKernel(vadd, 0, global);

  queue.enqueueReadBuffer(d_c, true, 0,  VEC_LEN * sizeof(float), c.data());
  queue.finish();


  for (size_t i = 0; i < VEC_LEN; i++) {
    std::cout << a[i] << " + " << b[i] << " == " << c[i] << '\n';
    assert((c[i] == a[i] + b[i]));
  }
  std::cout << "Assertions passed." << '\n';

  return 0;
}
