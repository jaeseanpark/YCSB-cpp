//
//  db_wrapper.h
//  YCSB-cpp
//
//  Copyright (c) 2020 Youngjae Lee <ls4154.lee@gmail.com>.
//

#ifndef YCSB_C_DB_WRAPPER_H_
#define YCSB_C_DB_WRAPPER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "db.h"
#include "measurements.h"
#include "timer.h"
#include "utils.h"

namespace ycsbc {

class DBWrapper : public DB {
 public:
  DBWrapper(DB *db, Measurements *measurements) : db_(db), measurements_(measurements) {}
  ~DBWrapper() {
    delete db_;
  }
  void Init() {
    db_->Init();
  }
  void Cleanup() {
    db_->Cleanup();
  }
  //NOTE: implemented logging
  Status Read(const std::string &table, const std::string &key,
              const std::vector<std::string> *fields, std::vector<Field> &result) {
    std::fstream outfile;
    outfile.open("readlog.txt", std::ios::app);
    timer_.Start();
    Status s = db_->Read(table, key, fields, result);
    outfile << key << std::endl;
    uint64_t elapsed = timer_.End();
    measurements_->Report(READ, elapsed);
    return s;
  }
  Status Scan(const std::string &table, const std::string &key, int record_count,
              const std::vector<std::string> *fields, std::vector<std::vector<Field>> &result) {
    timer_.Start();
    Status s = db_->Scan(table, key, record_count, fields, result);
    uint64_t elapsed = timer_.End();
    measurements_->Report(SCAN, elapsed);
    return s;
  }
  //NOTE: implemented logging
  Status Update(const std::string &table, const std::string &key, std::vector<Field> &values) {
    std::fstream outfile;
    outfile.open("updatelog.txt", std::ios::app);
    timer_.Start();
    Status s = db_->Update(table, key, values);
    outfile << key << std::endl;
    uint64_t elapsed = timer_.End();
    measurements_->Report(UPDATE, elapsed);
    return s;
  }
  //NOTE: implemented logging
  Status Insert(const std::string &table, const std::string &key, std::vector<Field> &values) {
    std::fstream outfile;
    outfile.open("writelog.txt", std::ios::app);
    timer_.Start();
    Status s = db_->Insert(table, key, values);
    outfile << key << std::endl;
    uint64_t elapsed = timer_.End();
    measurements_->Report(INSERT, elapsed);
    return s;
  }
  Status Delete(const std::string &table, const std::string &key) {
    timer_.Start();
    Status s = db_->Delete(table, key);
    uint64_t elapsed = timer_.End();
    measurements_->Report(DELETE, elapsed);
    return s;
  }
 private:
  DB *db_;
  Measurements *measurements_;
  utils::Timer<uint64_t, std::nano> timer_;
};

} // ycsbc

#endif // YCSB_C_DB_WRAPPER_H_
