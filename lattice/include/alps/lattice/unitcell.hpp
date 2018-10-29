/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_UNITCELL_HPP
#define ALPS_LATTICE_UNITCELL_HPP

#include <boost/graph/adjacency_list.hpp>
#include "property.hpp"

namespace alps {
namespace lattice {

struct unitcell_vertex_property {
  unitcell_vertex_property() : index(0), vertex_type(0) {}
  std::size_t index;
  coordinate_t coordinate;
  int vertex_type;
};

struct unitcell_edge_property {
  unitcell_edge_property() : index(0), edge_type(0) {}
  std::size_t index;
  offset_t target_offset;
  int edge_type;
};

struct unitcell_bundle_property {
  basis_t basis_vector;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              unitcell_vertex_property,
                              unitcell_edge_property,
                              unitcell_bundle_property
                              > unitcell_graph;

template<class GRAPH = unitcell_graph>
class unitcell : public GRAPH {
public:
  typedef GRAPH graph_type;
  typedef typename graph_type::vertex_descriptor vertex_descriptor;
  typedef typename graph_type::edge_descriptor edge_descriptor;
  unitcell() : graph_type() {}
  unitcell(const basis_t& basis) : graph_type() {
    if (basis.rows() != basis.cols())
      throw std::invalid_argument("basis vector dimension mismatch");
    (*this)[boost::graph_bundle].basis_vector = basis;
  }
  unitcell(const graph_type& graph) : graph_type(graph) {}
  unitcell(const unitcell& cell, const extent_t& extent);
  unitcell(const unitcell& cell, const span_t& span);

  vertex_descriptor add_vertex(const coordinate_t& pos, int vertex_type) {
    if (pos.size() != dimension())
      throw std::invalid_argument("vertex coordinate dimension mismatch");
    for (std::size_t i = 0; i < dimension(); ++i) {
      if (pos[i] < 0 || pos[i] >= 1.0)
        throw std::invalid_argument("vertex coordinate out of range");
    }
    vertex_descriptor s = boost::add_vertex(*this);
    (*this)[s].index = num_vertices(*this) - 1;
    (*this)[s].coordinate = pos;
    (*this)[s].vertex_type = vertex_type;
    return s;
  }
    
  edge_descriptor add_edge(vertex_descriptor source, vertex_descriptor target,
                           const offset_t& target_offset, int edge_type) {
    if (source >= num_vertices(*this) || target >= num_vertices(*this))
      throw std::invalid_argument("vertex index out of range");
    if (target_offset.size() != dimension())
      throw std::invalid_argument("unitcell offset dimension mismatch");
    edge_descriptor b = std::get<0>(boost::add_edge(source, target, *this));
    (*this)[b].index = num_edges(*this) - 1;
    (*this)[b].target_offset = target_offset;
    (*this)[b].edge_type = edge_type;
    return b;
  }

  unsigned int dimension() const {
    return (*this)[boost::graph_bundle].basis_vector.cols();
  }
};

template<class GRAPH>
unsigned int dimension(const unitcell<GRAPH>& cell) {
  return cell.dimension();
}
  
} // end namespace lattice
} // end namespace alps

#endif
