#include "util.hpp"

extern cl_int err;

void check_error() {
  if (err != CL_SUCCESS) {
    std::cerr << "FAIL" << '\n';
    exit(1);
  }
}

void print_build_info(cl::Program &program, bool print_banner) {
  if (print_banner)
    std::cout << "=== Build Options ===" << '\n';
  auto opts = program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(&err);
  check_error();
  for (auto opt : opts) {
    print_dev_info(opt.first, print_banner);
    std::cout << "Options: " << opt.second << '\n';
  }

  if (print_banner)
    std::cout << "=== Build Logs ===" << '\n';
  auto logs = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(&err);
  check_error();
  for (auto log : logs) {
    std::cout << "logs: " << log.second << '\n';
  }

  if (print_banner)
    std::cout << "=== Build Status ===" << '\n';
  auto statuses = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(&err);
  check_error();
  for (auto status : statuses) {
    std::cout << "statuses: " << status.second << '\n';
  }
  if (print_banner)
    std::cout << "=========" << '\n';
}

void print_dev_info(cl::Device &dev, bool print_banner) {
  if (print_banner)
    std::cout << "=== Device ===" << '\n';
  std::string vendor  = dev.getInfo<CL_DEVICE_VENDOR>();
  std::string version = dev.getInfo<CL_DEVICE_VERSION>();
  std::string name    = dev.getInfo<CL_DEVICE_NAME>();
  std::cout << "Vendor:  " << vendor  << '\n';
  std::cout << "Version: " << version << '\n';
  std::cout << "Name:    " << name    << '\n';
}


std::string read_to_string(const char *fname) {
  std::ifstream file_stream(fname);
  std::string src((std::istreambuf_iterator<char>(file_stream)), (std::istreambuf_iterator<char>()));
  return src;
}
