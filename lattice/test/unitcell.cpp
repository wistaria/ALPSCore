/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <boost/graph/graph_utility.hpp>
#include "alps/lattice/unitcell.hpp"

TEST(UnitcellTest, SimpleDouble) {
  std::vector<std::vector<double> > basis(2);
  basis[0] = std::vector<double>({1.0,0.0});
  basis[1] = std::vector<double>({0.0,1.0});
  alps::lattice::unitcell<> unitcell(basis);
  auto s0 = unitcell.add_vertex({0.0,0.0}, 0);
  auto b0 = unitcell.add_edge(s0, s0, {0,0}, {1,0}, 0);
  auto b1 = unitcell.add_edge(s0, s0, {0,0}, {0,1}, 0);

  boost::print_graph(unitcell);
  EXPECT_EQ(2, dimension(unitcell));
  EXPECT_EQ(1, num_vertices(unitcell));
  EXPECT_EQ(2, num_edges(unitcell));
  EXPECT_EQ(0, unitcell[b0].index);
  EXPECT_EQ(1, unitcell[b1].index);
  EXPECT_ANY_THROW(unitcell.add_edge(0, 1, {0,0}, {0,1}, 0));
}
