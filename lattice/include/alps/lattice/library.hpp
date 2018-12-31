/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_LIBRARY_HPP
#define ALPS_LATTICE_LIBRARY_HPP

#include <boost/graph/adjacency_list.hpp>
#include "property.hpp"

namespace alps {
namespace lattice {

class library {
public:
  library(const std::stirng& file) {}
  library(const graph_type& graph) : graph_type(graph) {}
  library(const library& cell, const extent_t& extent);
  library(const library& cell, const span_t& span);

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
      throw std::invalid_argument("library offset dimension mismatch");
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
unsigned int dimension(const library<GRAPH>& cell) {
  return cell.dimension();
}
  
} // end namespace lattice
} // end namespace alps

#endif
