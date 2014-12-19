//
//  ycsbc.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/19/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "utils.h"
#include "timer.h"
#include "core_workload.h"
#include "db_factory.h"

using namespace std;

void UsageMessage(const char *argv[]) {
  cout << "Usage: " << argv[0] << " [options]" << endl;
  cout << "Options:" << endl;
  cout << "  -threads n: execute using n threads (default: 1)" << endl;
  cout << "  -target n: attempt to do n operations per second (default: unlimited)" << endl;
  cout << "  -db dbname: specify the name of the DB to use (default: basic)" << endl;
  cout << "  -P propertyfile: load properties from the given file. Multiple files can" << endl;
  cout << "                   be specified, and will be processed in the order specified" << endl;
}

bool StrStartWith(const char *str, const char *pre) {
  return strncmp(str, pre, strlen(pre)) == 0;
}

int main(const int argc, const char *argv[]) {
  string dbname;
  utils::Properties props;
  int threadcount = 1;
  int target = 0;

  int argindex = 1;
  while (argindex < argc && StrStartWith(argv[argindex], "-")) {
    if (strcmp(argv[argindex], "-threads") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv);
        exit(0);
      }
      int tcount = atoi(argv[argindex]);
      props.SetProperty("threadcount", to_string(tcount));
      argindex++;
    } else if (strcmp(argv[argindex], "-target") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv);
        exit(0);
      }
      int ttarget = atoi(argv[argindex]);
      props.SetProperty("target", to_string(ttarget));
      argindex++;
    } else if (strcmp(argv[argindex], "-db") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv);
        exit(0);
      }
      dbname = argv[argindex];
      argindex++;
    } else if (strcmp(argv[argindex], "-P") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv);
        exit(0);
      }

      ifstream input(argv[argindex]);
      try {
        props.Load(input);
      } catch (const string &message) {
        cout << message << endl;
        exit(0);
      }
      input.close();
      argindex++;
    } else {
      cout << "Unknown option " << argv[argindex] << endl;
      exit(0);
    }
  }

  if (argindex == 1 || argindex != argc) {
    UsageMessage(argv);
    exit(0);
  }

  ycsbc::DB *db = ycsbc::DBFactory::CreateDB(dbname);
  if (!db) {
    cout << "Unknown database name " << dbname << endl;
    exit(0);
  }
  db->Init(props);
  utils::Timer<double> timer;
  timer.Start();
  cout << timer.End() << endl;
}

