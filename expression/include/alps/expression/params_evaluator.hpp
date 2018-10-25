/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_PARAMS_EVALUATOR_HPP
#define ALPS_EXPRESSION_PARAMS_EVALUATOR_HPP

#include <alps/params.hpp>
#include "evaluator.hpp"
#include "evaluate.hpp"

namespace alps {
namespace expression {

//
// implementation of params_evaluator<T>
//

template<class T>
bool params_evaluator<T>::can_evaluate(const std::string& name, bool isarg) const {
  if (evaluate_helper<T>::can_evaluate_symbol(name, isarg)) return true;
  if (!parms_.defined(name) || (parms_[name].isType<std::string>() && parms_[name] == "Infinite recursion check"))
    return false;
  if (parms_[name].isType<std::string>()) {
    params p(parms_);
    p[name] = "Infinite recursion check"; // set illegal to avoid infinite recursion
    bool can = expression<T>(parms_[name].as<std::string>()).can_evaluate(params_evaluator<T>(p),isarg);
    return can;
  } else {
    return true;
  }
}

template<class T>
expression<T> params_evaluator<T>::partial_evaluate(const std::string& name, bool isarg) const {
  expression<T> e;
  if (params_evaluator<T>::can_evaluate(name,isarg))
    e=expression<T>(params_evaluator<T>::evaluate(name,isarg));
  else if(!parms_.defined(name))
    e=expression<T>(name);
  else {
    params p(parms_);
    p[name]="";
    e=expression<T>(parms_[name]);
    e.partial_evaluate(params_evaluator<T>(p),isarg);
  }
  return e;
}

template<class T>
typename params_evaluator<T>::value_type params_evaluator<T>::evaluate(const std::string& name, bool isarg) const {
  if (evaluate_helper<T>::can_evaluate_symbol(name,isarg))
    return evaluate_helper<T>::evaluate_symbol(name,isarg);
  if (parms_[name].isType<std::string>() && parms_[name] == "Infinite recursion check")
    boost::throw_exception(std::runtime_error("Infinite recursion when evaluating " + name));
  if (parms_[name].isType<std::string>()) {
    params p(parms_);
    p[name] = "Infinite recursion check";
    typename params_evaluator<T>::value_type res = alps::evaluate<value_type>(parms_[name], p);
    return res;
  } else {
    return value_type(parms_[name]);
  }
}

} // namespace expression
} // namespace alps

#endif
