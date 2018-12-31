/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_BASIS_HPP
#define ALPS_LATTICE_BASIS_HPP

#include "property.hpp"

namespace alps {
namespace lattice {

class basis {
public:
  typedef Eigen::Matrix<std::string> MatrixXs;
  basis() {}
  basis(const std::string& name, const MatrixXs& basis) {
    if (basis_.rows() != basis_.cols())
      throw std::invalid_argument("basis vector dimension mismatch");
    name_ = name;
    basis_ = basis;
  }
  unsigned int dimension() const { return basis_.rows(); }
  alps::lattice::basis_t basis_vectors
private:
  std::string name_;
      basis_t basis_;
};

unsigned int dimension(const basis& bs) {
  return bs.dimension();
}
  
} // end namespace lattice
} // end namespace alps

#endif
