/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_LATTICE_HPP
#define ALPS_LATTICE_LATTICE_HPP

#include <boost/graph/adjacency_list.hpp>
#include "property.hpp"
#include "unitcell.hpp"
#include "boundary.hpp"

namespace alps {
namespace lattice {

struct lattice_vertex_property {
  std::size_t index;
  coordinate_type coordinate;
  int vertex_type;
  unsigned int parity;
};

struct lattice_edge_property {
  std::size_t index;
  coordinate_type bond_vector;
  type_type edge_type;
};

struct lattice_bundle_property {
  std::vector<std::vector<double> > lattice_vector;
  std::vector<std::vector<double> > reciprocal_vector;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              lattice_vertex_property,
                              lattice_edge_property,
                              lattice_bundle_property,
                              boost::vecS
                              > lattice_graph;

template<class GRAPH = lattice_graph>
class lattice : public GRAPH {
public:
  typedef GRAPH graph_type;
  lattice() : graph_type() {}
  lattice(const graph_type& graph) : graph_type(graph) {}
  template<class G>
  lattice(const unitcell<G>& cell, const std::vector<int>& extent,
          const std::vector<boundary_t>& boundary = std::vector<boundary_t>());
  template<class G>
  lattice(const unitcell<G>& cell, const std::vector<std::vector<int> >& supercell,
          const std::vector<boundary_t>& boundary = std::vector<boundary_t>());
};

} // end namespace lattice
} // end namespace alps

#endif // ALPS_LATTICE_LATTICE_HPP
