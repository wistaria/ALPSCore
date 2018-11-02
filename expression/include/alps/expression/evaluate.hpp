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
T evaluate(const std::string& v, const params& p) {
  return expression::evaluate_helper<T>::value(expression::expression<T>(v), expression::evaluator<T>(p));
}

double evaluate(const std::string& v, const params& p) {
  return expression::evaluate_helper<double>::value(expression::expression<std::complex<double> >(v), expression::evaluator<std::complex<double> >(p));
}

template<class T>
T evaluate(const std::string& v) {
  return expression::evaluate_helper<T>::value(expression::expression<T>(v), expression::evaluator<T>());
}

double evaluate(const std::string& v) {
  return expression::evaluate_helper<double>::value(expression::expression<std::complex<double> >(v), expression::evaluator<std::complex<double> >());
}

} // end namespace alps

#endif
