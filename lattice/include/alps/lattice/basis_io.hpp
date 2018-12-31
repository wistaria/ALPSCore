/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_BASIS_IO_HPP
#define ALPS_LATTICE_BASIS_IO_HPP

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "basis.hpp"

namespace alps {
namespace lattice {

using boost::property_tree::ptree;

ptree& operator>>(ptree& pt, basis& bs) {
  bs.clear();
  if (auto str = pt.get_optional<std::string>("<xmlattr>.name")) {
    bs.set_name(str.get());
  }
  alps::params p;
  for (auto& child : pt.get_child("")) {
    std::cerr << child.first << std::endl;
    p[child.second.get<std::string>("<xmlattr>.name")] =
      child.second.get<std::string>("<xmlattr>.default");
  }
  if (auto str = pt.get_optional<std::string>("<xmlattr>.dimension")) {
    std::cerr << "dimension = " << boost::lexical_cast<std::size_t>(str.get()) << std::endl;
  }
  return pt;
}

ptree& operator<<(ptree& pt, const basis& bs) {
  ptree& child = pt.add("LATTICE", "");
  child.put("<xmlattr>.name", bs.name());
  child.put("<xmlattr>.dimension", bs.dimension());
  for (auto v : bs.default_params()) {
    ptree& p = child.add("PARAMETER", "");
    p.put("<xmlattr>.name", v.first);
    std::ostringstream os;
    print(os, v.second, true);
    p.put("<xmlattr>.default", os.str());
  }
  ptree& basis = child.add("BASIS", "");
  for (std::size_t j = 0; j < bs.dimension(); ++j) {
    std::ostringstream os;
    for (std::size_t i = 0; i < bs.dimension(); ++i) {
      os << (i ? " " : "") << bs.vectors_expression()(i, j);
    }
    basis.add("VECTOR", os.str());
  }
  return pt;
}
  
} // end namespace lattice
} // end namespace alps

#endif
