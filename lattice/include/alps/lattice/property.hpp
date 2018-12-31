/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_PROPERTY_HPP
#define ALPS_LATTICE_PROPERTY_HPP

#include <string>
#include <Eigen/Dense>

#ifndef LATTICE_MAX_DIM
# define LATTICE_MAX_DIM 4
#endif

namespace alps {
namespace lattice {

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> basis_t;

// basis set that has expressions as elements
typedef Eigen::Matrix<std::string, Eigen::Dynamic, Eigen::Dynamic> basis_expression_t;
  
// set of spanning vectors
typedef Eigen::Matrix<long, Eigen::Dynamic, Eigen::Dynamic> span_t;

// extent vector = diagonal spanning matrix
typedef Eigen::Matrix<long, Eigen::Dynamic, 1> extent_t;

// coordinate vector has fixed capacity for efficiency
typedef Eigen::Matrix<double, Eigen::Dynamic, 1, 0, LATTICE_MAX_DIM, 1> coordinate_t;

typedef Eigen::Matrix<long, Eigen::Dynamic, 1> offset_t;

} // end namespace lattice
} // end namespace alps

#endif
