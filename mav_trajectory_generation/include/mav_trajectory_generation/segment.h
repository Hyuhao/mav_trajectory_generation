/*
 * Copyright (c) 2016, Markus Achtelik, ASL, ETH Zurich, Switzerland
 * Copyright (c) 2016, Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright (c) 2016, Helen Oleynikova, ASL, ETH Zurich, Switzerland
 * Copyright (c) 2016, Rik Bähnemann, ASL, ETH Zurich, Switzerland
 * Copyright (c) 2016, Marija Popovic, ASL, ETH Zurich, Switzerland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MAV_TRAJECTORY_GENERATION_SEGMENT_H_
#define MAV_TRAJECTORY_GENERATION_SEGMENT_H_

#include <chrono>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <glog/logging.h>
#include <map>
#include <vector>

#include "mav_trajectory_generation/motion_defines.h"
#include "mav_trajectory_generation/polynomial.h"

namespace mav_trajectory_generation {

/**
 * \brief Class holding the properties of parametric segment of a path.
 *        Time of the segment and one polynomial for each dimension.
 *
 * \code
 *     X------------X---------------X
 *   vertex             segment
 * \endcode
 */
class Segment {
 public:
  typedef std::vector<Segment> Vector;

  Segment(int N, int D) : time_(0), N_(N), D_(D) {
    polynomials_.resize(D_, N_);
  }

  int D() const { return D_; }
  int N() const { return N_; }
  double getTime() const { return time_; }
  uint64_t getTimeNSec() const { return static_cast<uint64_t>(1.0e9 * time_); }

  void setTime(double _time) { time_ = _time; }
  void setTimeNSec(uint64_t time_ns) { time_ = time_ns * 1.0e-9; }

  Polynomial& operator[](size_t idx);

  const Polynomial& operator[](size_t idx) const;

  const std::vector<Polynomial, Eigen::aligned_allocator<Polynomial>>&
  getPolynomialsRef() const {
    return polynomials_;
  }

  Eigen::VectorXd evaluate(
      double t, int derivative_order = derivative_order::POSITION) const;

 protected:
  std::vector<Polynomial, Eigen::aligned_allocator<Polynomial>> polynomials_;
  double time_;

 private:
  int D_;  ///< Number of dimensions.
  int N_;  ///< Number of coefficients.
};

/**
 * \brief Prints the properties of the segment.
 *
 * Polynomial coefficients are printed with increasing powers, i.e. c_0 + c_1*t
 * ... c_{N-1} * t^{N-1}
 */
void printSegment(std::ostream& stream, const Segment& s, int derivative);

std::ostream& operator<<(std::ostream& stream, const Segment& s);

std::ostream& operator<<(std::ostream& stream,
                         const std::vector<Segment>& segments);

}  // namespace mav_trajectory_generation

#endif  // MAV_TRAJECTORY_GENERATION_SEGMENT_H_
