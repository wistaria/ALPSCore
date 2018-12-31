/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <boost/graph/graph_utility.hpp>
#include "alps/lattice/basis.hpp"

TEST(LatticeTest, SimpleDouble) {
  std::string name = "square lattice";
  alps::lattice::basis_expression_t bs(2, 2);
  bs << "a", "0",
        "0", "a";
  alps::params p_default;
  p_default["a"] = 1.0;
  alps::lattice::basis basis(name, bs, p_default);
  std::cerr << basis.vectors() << std::endl;
  alps::params p;
  p["a"] = 1.5;
  std::cerr << basis.vectors(p) << std::endl;
}
