/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_PROPERTY_HPP
#define ALPS_LATTICE_PROPERTY_HPP

#include <Eigen/Dense>

#ifndef LATTICE_MAX_DIM
# define LATTICE_MAX_DIM 3
#endif

namespace alps {
namespace lattice {

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, LATTICE_MAX_DIM, LATTICE_MAX_DIM> basis_t; // MatrixXd
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, 0, LATTICE_MAX_DIM, LATTICE_MAX_DIM> span_t; // MatrixXi
typedef Eigen::Matrix<int, Eigen::Dynamic, 1, 0, LATTICE_MAX_DIM, 1> extent_t; // VectorXi
typedef Eigen::Matrix<double, Eigen::Dynamic, 1, 0, LATTICE_MAX_DIM, 1> coordinate_t; // VectorXd
typedef Eigen::Matrix<int, Eigen::Dynamic, 1, 0, LATTICE_MAX_DIM, 1> offset_t; // VectorXi

} // end namespace lattice
} // end namespace alps

#endif
