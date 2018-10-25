/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "alps/expression/parser.hpp"
#include <iostream>
#include <boost/throw_exception.hpp>

namespace alps {
namespace expression {
namespace {

bool is_identifier_char(char c)  { 
  return std::isalnum(c) || c=='_' || c==':' || c=='#';
}

} // end namespace

std::string parse_parameter_name(std::istream& in) {
  char c;
  in >> c;
  std::string name;
  while (in && !in.eof() && (is_identifier_char(c) || c=='\''  || c=='[')) {
    name+=c;
        if (c=='[') 
          do {
            c=in.get();
                name+=c;
          } while (c!=']');
    c=in.get();
  }
  if (!in.eof())
    in.putback(c);
  return name;          
}

void check_character(std::istream& in, char test, const std::string& err) {
  char c;
  in >> c;
  if (c!=test)
    boost::throw_exception(std::runtime_error(err));
}

} // end namespace expression
} // end namespace alps
