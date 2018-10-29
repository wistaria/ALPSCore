/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <boost/graph/graph_utility.hpp>
#include "alps/lattice/lattice.hpp"

TEST(LatticeTest, SimpleDouble) {
  alps::lattice::lattice<> lattice;
  boost::print_graph(lattice);
}
