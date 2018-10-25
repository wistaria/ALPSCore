/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_BLOCK_HPP
#define ALPS_EXPRESSION_BLOCK_HPP

#include "expression_fwd.hpp"
#include "evaluatable.hpp"
#include "parser.hpp"

namespace alps {
namespace expression {

template<class T>
class block : public expression<T> {
public:
  block(std::istream&);
  block(const expression<T>& e) : BASE_(e) {}
  void output(std::ostream&) const;
  evaluatable<T>* clone() const { return new block<T>(*this); }
  evaluatable<T>* partial_evaluate_replace(const evaluator<T>& =evaluator<T>());
private:
  typedef expression<T> BASE_;
};

//
// implementation of block<T>
//

template<class T>
block<T>::block(std::istream& in) : expression<T>(in) {
  char c;
  in >> c;
  if (c != ')' && c != ',')
    boost::throw_exception(std::runtime_error(") or , expected in expression"));
  if (c == ',') {
    // read imaginary part
    expression<T> ex(in);
    block<T> bl(ex);
    term<T> tm(bl);
    tm *= "I";
    *this += tm;
    check_character(in,')',") expected in expression");
  }
}

template<class T>
void block<T>::output(std::ostream& os) const {
  BASE_::output(os);
}

template<class T>
evaluatable<T>* block<T>::partial_evaluate_replace(const evaluator<T>& p) {
  expression<T>::partial_evaluate(p);
  return this;
}

} // end namespace expression
} // end namespace alps

#endif
