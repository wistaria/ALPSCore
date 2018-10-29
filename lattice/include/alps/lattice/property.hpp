/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_LATTICE_PROPERTY_HPP
#define ALPS_LATTICE_PROPERTY_HPP

#include <vector>
#include <boost/graph/properties.hpp>

namespace alps {
namespace lattice {

typedef boost::vertex_index_t vertex_index_t;
typedef boost::vertex_index_t site_index_t;
typedef vertex_index_t site_index_t;
struct vertex_type_t { typedef boost::vertex_property_tag kind; };
typedef vertex_type_t site_type_t;
struct coordinate_t { typedef boost::vertex_property_tag kind; };
struct parity_t { typedef boost::vertex_property_tag kind; };

typedef boost::edge_index_t edge_index_t;
typedef boost::edge_index_t bond_index_t;
struct edge_type_t { typedef boost::edge_property_tag kind; };
typedef edge_type_t bond_type_t;
struct source_offset_t { typedef boost::edge_property_tag kind; };
struct target_offset_t { typedef boost::edge_property_tag kind; };
struct boundary_crossing_t { typedef boost::edge_property_tag kind; };
struct edge_vector_t { typedef boost::edge_property_tag kind; };
typedef edge_vector_t bond_vector_t;
struct edge_vector_relative_t { typedef boost::edge_property_tag kind; };
typedef edge_vector_relative_t bond_vector_relative_t;

struct dimension_t { typedef boost::graph_property_tag kind; };

typedef std::vector<double> coordinate_type;
typedef std::vector<int> offset_type;
typedef std::vector<int> distance_type;
typedef unsigned int type_type;

} // end namespace lattice
} // end namespace alps

#endif
