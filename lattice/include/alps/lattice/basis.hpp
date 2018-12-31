/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_BASIS_HPP
#define ALPS_LATTICE_BASIS_HPP

#include "alps/params.hpp"
#include "alps/expression.hpp"
#include "property.hpp"

namespace alps {
namespace lattice {

class basis {
public:
  basis() {}
  basis(const std::string& name, const basis_expression_t& bs,
        const alps::params& p = alps::params()) :
    name_(name), basis_(bs), default_(p) {
    if (basis_.rows() != basis_.cols())
      throw std::invalid_argument("basis dimension mismatch");
  }
  void clear() { *this = basis(); }
  
  void set_name(const std::string& name) { name_ = name; }
  void set_basis(const basis_expression_t& bs) {
    basis_ = bs;
    if (basis_.rows() != basis_.cols())
      throw std::invalid_argument("basis dimension mismatch");
  }
  void set_default_params(const alps::params& p) {
    default_ = p;
  }
  
  const std::string& name() const { return name_; }
  std::size_t dimension() const { return basis_.rows(); }
  const basis_expression_t& vectors_expression() const {
    return basis_;
  }
  const alps::params& default_params() const { return default_; }

  basis_t vectors(const alps::params& p = alps::params()) const {
    alps::params q = default_;
    for (auto v : p) q[v.first] = v.second;
    basis_t bev(basis_.rows(), basis_.cols());
    for (int j = 0; j < basis_.cols(); ++j)
      for (int i = 0; i < basis_.rows(); ++i)
        bev(i, j) = evaluate<double>(basis_(i, j), q);
    return bev;
  }
private:
  std::string name_;
  basis_expression_t basis_;
  alps::params default_;
};

std::size_t dimension(const basis& bs) {
  return bs.dimension();
}
  
} // end namespace lattice
} // end namespace alps

#endif
