/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <boost/graph/graph_utility.hpp>
#include "alps/lattice/unitcell.hpp"

TEST(UnitcellTest, SimpleDouble) {
  alps::lattice::basis_t basis(2, 2);
  basis << 1.0, 0.0,
           0.0, 1.0;
  alps::lattice::unitcell<> unitcell(basis);
  alps::lattice::coordinate_t coord(2); coord << 0.0, 0.0;
  auto s0 = unitcell.add_vertex(coord, 0);
  alps::lattice::offset_t offset_x(2); offset_x << 1, 0;
  auto b0 = unitcell.add_edge(s0, s0, offset_x, 0);
  alps::lattice::offset_t offset_y(2); offset_y << 0, 1;
  auto b1 = unitcell.add_edge(s0, s0, offset_y, 0);

  boost::print_graph(unitcell);
  EXPECT_EQ(2, dimension(unitcell));
  EXPECT_EQ(1, num_vertices(unitcell));
  EXPECT_EQ(2, num_edges(unitcell));
  EXPECT_EQ(0, unitcell[b0].index);
  EXPECT_EQ(1, unitcell[b1].index);
  EXPECT_ANY_THROW(unitcell.add_edge(0, 1, offset_x, 0));
}
