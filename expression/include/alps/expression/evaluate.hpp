/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EVALUATE_HPP
#define ALPS_EXPRESSION_EVALUATE_HPP

#include <alps/params.hpp>
#include "expression.hpp"
#include "evaluator.hpp"
#include "evaluate_helper.hpp"

namespace alps {

template<class T>
inline T evaluate(const expression::expression<T>& ex, const expression::evaluator<T>& ev) {
  return expression::evaluate_helper<T>::value(ex, ev);
}

template<class T>
inline T evaluate(const std::string& v, const expression::evaluator<T>& ev) {
  return evaluate<T>(expression::expression<T>(v), ev);
}

template<class T>
inline T evaluate(const std::string& v, const params& p) {
  return evaluate<T>(v, expression::evaluator<T>(p));
}

inline double evaluate(const std::string& v, const params& p) {
  return evaluate<double>(v, expression::evaluator<double>(p));
}

template<class T>
inline T evaluate(const std::string& v) {
  return evaluate<T>(v, expression::evaluator<T>());
}

inline double evaluate(const std::string& v) {
  return evaluate<double>(v, expression::evaluator<double>());
}

} // end namespace alps

#endif
