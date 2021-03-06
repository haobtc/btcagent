/*
 Mining Pool Agent

 Copyright (C) 2016  BTC.COM

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <signal.h>
#include <execinfo.h>
#include <string>
#include "gtest/gtest.h"

#include <glog/logging.h>

using std::string;

//
// run all:      ./unittest
// run single:   ./unittest --gtest_filter=StratumSession\*
//
extern "C" {

static void handler(int sig);

// just for debug, should be removed when release
void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, 2);
  exit(1);
}
}

typedef char * CString;

int main(int argc, char **argv) {
  signal(SIGSEGV, handler);
  signal(SIGFPE, handler);
  signal(SIGPIPE, handler);

  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  FLAGS_logbuflevel = -1;
  FLAGS_logtostderr = true;
  FLAGS_colorlogtostderr = true;
  
  CString * newArgv = new CString [argc];
  memcpy(newArgv, argv, argc * sizeof(CString));
  string testname = "--gtest_filter=";
  if (argc == 2 && newArgv[1][0] != '-') {
    testname.append(newArgv[1]);
    newArgv[1] = (char*)testname.c_str();
  }
  testing::InitGoogleTest(&argc, newArgv);
  
  int ret = RUN_ALL_TESTS();
  delete [] newArgv;
  return ret;
}


