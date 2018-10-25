/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EVALUATE_HPP
#define ALPS_EXPRESSION_EVALUATE_HPP

#include <alps/params.hpp>
#include "expression_fwd.hpp"
#include "expression.hpp"
#include "evaluator.hpp"
#include "evaluate_helper.hpp"

namespace alps {

template<class T>
inline bool can_evaluate(const expression::evaluatable<T>& ex, const expression::evaluator<T>& ev) {
  return ex.can_evaluate(ev);
}

template<class T>
inline bool can_evaluate(const std::string& v, const expression::evaluator<T>& p) {
  return expression::expression<T>(v).can_evaluate(p);
}

inline bool can_evaluate(const std::string& v, const params& p = params()) {
  return can_evaluate(v, expression::evaluator<>(p));
}

template<class U>
inline bool can_evaluate(const std::string& v, const params& p, const U&) {
  return can_evaluate(v, expression::evaluator<U>(p));
}

template<class U, class T>
inline U evaluate(const expression::expression<T>& ex, const expression::evaluator<T>& ev = expression::evaluator<T>()) {
  return expression::evaluate_helper<U>::value(ex, ev);
}

template<class U, class T>
inline U evaluate(const expression::term<T>& ex, const expression::evaluator<T>& ev = expression::evaluator<T>()) {
  return expression::evaluate_helper<U>::value(ex, ev);
}

template<class U, class T>
inline U evaluate(const char* v, const expression::evaluator<T>& ev) {
  return expression::evaluate_helper<U>::value(expression::expression<T>(std::string(v)), ev);
}

template<class U, class T>
inline U evaluate(const std::string& v, const expression::evaluator<T>& ev) {
  return expression::evaluate_helper<U>::value(expression::expression<T>(v), ev);
}

template<class U>
inline U evaluate(const char* v) {
  return evaluate<U, U>(v, expression::evaluator<
    typename expression::evaluate_helper<U>::value_type>());
}
inline double evaluate(const char* v) {
  return evaluate<double, double>(v, expression::evaluator<
    expression::evaluate_helper<double>::value_type>());
}

template<class U>
inline U evaluate(const std::string& v) {
  return evaluate<U, U>(v, expression::evaluator<
    typename expression::evaluate_helper<U>::value_type>());
}
inline double evaluate(const std::string& v) {
  return evaluate<double, double>(v, expression::evaluator<
    expression::evaluate_helper<double>::value_type>());
}

template<class U>
inline U evaluate(const std::string& v, const params& p) {
  return evaluate<U, typename expression::evaluate_helper<U>::value_type>(v, expression::evaluator<typename expression::evaluate_helper<U>::value_type>(p));
}
inline double evaluate(const std::string& v, const params& p) {
  return evaluate<double, expression::evaluate_helper<double>::value_type>(v, expression::evaluator<expression::evaluate_helper<double>::value_type>(p));
}

} // end namespace alps

#endif
