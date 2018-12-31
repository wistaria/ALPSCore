/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <sstream>
#include "gtest/gtest.h"
#include "alps/lattice/basis_io.hpp"

class BasisIoTest : public testing::Test {
public:
  BasisIoTest() {
    alps::params p_default;
    p_default["a"] = 1.0;

    { // chain lattice
      alps::lattice::basis_expression_t bs(1, 1);
      bs << "a";
      basis1 = alps::lattice::basis("chain lattice", bs, p_default);
    }

    { // square lattice
      alps::lattice::basis_expression_t bs(2, 2);
      bs << "a", "0",
            "0", "a";
      basis2 = alps::lattice::basis("square lattice", bs, p_default);
    }
  }
protected:
  alps::lattice::basis basis1, basis2;
};

TEST_F(BasisIoTest, WriteXML) {
  boost::property_tree::ptree pt;
  boost::property_tree::ptree& root = pt.put("LATTICES", "");

  root << basis1 << basis2;
  write_xml(std::cerr, pt,
    boost::property_tree::xml_writer_make_settings<std::string>(' ', 2));
}

TEST_F(BasisIoTest, ReadXML) {
  boost::property_tree::ptree pt;
  boost::property_tree::ptree& root = pt.put("LATTICES", "");

  root << basis1 << basis2;

  std::vector<alps::lattice::basis> lattices;
  for (auto& child : pt.get_child("LATTICES")) {
    if (child.first == "LATTICE") {
      alps::lattice::basis bs;
      child.second >> bs;
      lattices.push_back(bs);
    }
  }
}
