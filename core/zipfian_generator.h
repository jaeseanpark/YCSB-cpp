//
//  zipfian_generator.h
//  YCSB-cpp
//
//  Copyright (c) 2020 Youngjae Lee <ls4154.lee@gmail.com>.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_ZIPFIAN_GENERATOR_H_
#define YCSB_C_ZIPFIAN_GENERATOR_H_

#include <cassert>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <iostream>

#include "generator.h"
#include "utils.h"

namespace ycsbc {

class ZipfianGenerator : public Generator<uint64_t> {
 public:
 //NOTE: Zipfian constant
  static constexpr double kZipfianConst = 0.2;
  static constexpr uint64_t kMaxNumItems = (UINT64_MAX >> 24);

  ZipfianGenerator(uint64_t num_items) :
      ZipfianGenerator(0, num_items - 1) {}

  ZipfianGenerator(uint64_t min, uint64_t max, double zipfian_const = kZipfianConst) :
      ZipfianGenerator(min, max, zipfian_const, Zeta(max - min + 1, zipfian_const)) {}

  ZipfianGenerator(uint64_t min, uint64_t max, double zipfian_const, double zeta_n) :
      items_(max - min + 1), base_(min), theta_(zipfian_const), allow_count_decrease_(false) {
    //ANCHOR
    std::cout << "___ZipfianGenerator(min, max, zipfconst, zeta_n): " << std::endl; 
    std::cout << "min: " << min << " max: " << max << " zipf const: " << zipfian_const << " zeta_n: " << zeta_n << std::endl;
    std::cout << "items: " << items_ << " base_: " << base_ << " theta: " << theta_ << std::endl;
    assert(items_ >= 2 && items_ < kMaxNumItems);

    zeta_2_ = Zeta(2, theta_);

    alpha_ = 1.0 / (1.0 - theta_);
    zeta_n_ = zeta_n;
    count_for_zeta_ = items_;
    eta_ = Eta();

    Next();
  }

  uint64_t Next(uint64_t num_items);

  uint64_t Next() { 
    uint64_t temp;
    temp = Next(items_);
    std::cout << "next from zipf_gen.h: " << temp << std::endl;
    return temp; 
  }

  uint64_t Last();

 private:
  double Eta() {
    return (1 - std::pow(2.0 / items_, 1 - theta_)) / (1 - zeta_2_ / zeta_n_);
  }

  ///
  /// Calculate the zeta constant needed for a distribution.
  /// Do this incrementally from the last_num of items to the cur_num.
  /// Use the zipfian constant as theta. Remember the new number of items
  /// so that, if it is changed, we can recompute zeta.
  ///
  static double Zeta(uint64_t last_num, uint64_t cur_num, double theta, double last_zeta) {
    double zeta = last_zeta;
    for (uint64_t i = last_num + 1; i <= cur_num; ++i) {
      zeta += 1 / std::pow(i, theta);
    }
    //ANCHOR
    std::cout << "___Zeta: " << zeta << std::endl; 
    return zeta;
  }

  static double Zeta(uint64_t num, double theta) {
    return Zeta(0, num, theta, 0);
  }

  uint64_t items_;
  uint64_t base_; /// Min number of items to generate

  // Computed parameters for generating the distribution
  double theta_, zeta_n_, eta_, alpha_, zeta_2_;
  uint64_t count_for_zeta_; /// Number of items used to compute zeta_n
  uint64_t last_value_;
  std::mutex mutex_;
  bool allow_count_decrease_;
};

inline uint64_t ZipfianGenerator::Next(uint64_t num) {
  //ANCHOR
  std::cout << "___Zipfian:Next" << std::endl; 
  assert(num >= 2 && num < kMaxNumItems);
  if (num != count_for_zeta_) {
    // recompute zeta and eta
    std::lock_guard<std::mutex> lock(mutex_);
    if (num > count_for_zeta_) {
      zeta_n_ = Zeta(count_for_zeta_, num, theta_, zeta_n_);
      count_for_zeta_ = num;
      eta_ = Eta();
    } else if (num < count_for_zeta_ && allow_count_decrease_) {
      // TODO
    }
  }

  double u = utils::ThreadLocalRandomDouble();
  double uz = u * zeta_n_;

  if (uz < 1.0) {
    //ANCHOR
    std::cout << "___last_value1: " << base_ << std::endl; 
    return last_value_ = base_;
  }

  if (uz < 1.0 + std::pow(0.5, theta_)) {
    //ANCHOR
    std::cout << "___last_value2: " << base_ + 1 << std::endl; 
    return last_value_ = base_ + 1;
  }

  //ANCHOR
  std::cout << "___last_value3: " << base_ + num * std::pow(eta_ * u - eta_ + 1, alpha_) << std::endl; 
  return last_value_ = base_ + num * std::pow(eta_ * u - eta_ + 1, alpha_);
}

inline uint64_t ZipfianGenerator::Last() {
  return last_value_;
}

}

#endif // YCSB_C_ZIPFIAN_GENERATOR_H_
