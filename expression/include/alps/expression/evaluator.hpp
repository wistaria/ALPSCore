/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EVALUATOR_HPP
#define ALPS_EXPRESSION_EVALUATOR_HPP

#include <boost/random.hpp>
#include <alps/params.hpp>
#include "expression_fwd.hpp"
#include "evaluate_helper.hpp"

namespace alps {
namespace expression {

template<class T>
class evaluator {
public:
  typedef T value_type;
  evaluator(const params& p = params()) : parms_(p) {}
  ~evaluator() {}

  bool can_evaluate(const std::string&) const;
  bool can_evaluate_function(const std::string&, const expression<T>&) const;
  bool can_evaluate_function(const std::string&, const std::vector<expression<T> >&) const;
  value_type evaluate(const std::string&) const;
  value_type evaluate_function(const std::string&, const expression<T>&) const;
  value_type evaluate_function(const std::string&, const std::vector<expression<T> >&) const;
  expression<T> partial_evaluate(const std::string& name) const;
  expression<T> partial_evaluate_function(const std::string& name, const expression<T>&) const;
  expression<T> partial_evaluate_function(const std::string& name, const std::vector<expression<T> >&) const;
  bool can_evaluate_expressions(const std::vector<expression<T> >&) const;
  void partial_evaluate_expressions(std::vector<expression<T> >&) const;
  const params& parameters() const { return parms_;}
private:
  params parms_;
};

template<class T>
bool evaluator<T>::can_evaluate(const std::string& name) const {
  if (evaluate_helper<T>::can_evaluate_symbol(name)) return true;
  if (!parms_.defined(name) || (parms_[name].isType<std::string>() && parms_[name] == "Infinite recursion check"))
    return false;
  if (parms_[name].isType<std::string>()) {
    params p(parms_);
    p[name] = "Infinite recursion check"; // set illegal to avoid infinite recursion
    bool can = expression<T>(parms_[name].as<std::string>()).can_evaluate(evaluator<T>(p));
    return can;
  } else {
    return true;
  }
}

template<class T>
bool evaluator<T>::can_evaluate_function(const std::string& name, const expression<T>& arg) const {
  return arg.can_evaluate(*this) &&
         (name=="sqrt" || name=="abs" ||
          name=="sin" || name=="cos" || name=="tan" ||
          name=="asin" || name=="acos" || name=="atan" ||
          name=="log" || name=="exp");
}

template<class T>
bool evaluator<T>::can_evaluate_expressions(const std::vector<expression<T> >& arg) const {
  bool can=true;
  for (typename std::vector<expression<T> >::const_iterator it = arg.begin(); it != arg.end(); ++it)
    can = can && it->can_evaluate(*this);
  return can;
}

template<class T>
void evaluator<T>::partial_evaluate_expressions(std::vector<expression<T> >& arg) const {
  for (typename std::vector<expression<T> >::iterator it = arg.begin(); it != arg.end(); ++it) {
   it->partial_evaluate(*this);
  }
}

template<class T>
bool evaluator<T>::can_evaluate_function(const std::string& name, const std::vector<expression<T> >& arg) const {
  bool can = can_evaluate_expressions(arg) &&
    (arg.size() == 1 && can_evaluate_function(name, arg[0]));
  return can;
}

template<class T>
typename evaluator<T>::value_type evaluator<T>::evaluate(const std::string& name) const {
  if (evaluate_helper<T>::can_evaluate_symbol(name))
    return evaluate_helper<T>::evaluate_symbol(name);
  if (parms_[name].isType<std::string>() && parms_[name] == "Infinite recursion check")
    boost::throw_exception(std::runtime_error("Infinite recursion when evaluating " + name));
  if (parms_[name].isType<std::string>()) {
    params p(parms_);
    p[name] = "Infinite recursion check";
    return evaluate_helper<T>::value(expression<T>(parms_[name]), evaluator<T>(p));
  } else {
    return value_type(parms_[name]);
  }
}

template<class T>
typename evaluator<T>::value_type evaluator<T>::evaluate_function(const std::string& name, const expression<T>& arg) const {
  return partial_evaluate_function(name,arg).value();
}

template<class T>
typename evaluator<T>::value_type evaluator<T>::evaluate_function(const std::string& name, const std::vector<expression<T> >& arg) const {
  return partial_evaluate_function(name, arg).value();
}

template<class T>
expression<T> evaluator<T>::partial_evaluate(const std::string& name) const {
  expression<T> e;
  if (can_evaluate(name)) {
    e = expression<T>(evaluate(name));
  } else if(!parms_.defined(name)) {
    e=expression<T>(name);
  } else {
    params p(parms_);
    p[name] = "";
    e = expression<T>(parms_[name]);
    e.partial_evaluate(evaluator<T>(p));
  }
  return e;
}


template<class T>
expression<T> evaluator<T>::partial_evaluate_function(const std::string& name, const expression<T>& arg) const {
  if(!arg.can_evaluate(*this)) {
    expression<T> e(arg);
    e.partial_evaluate(*this);
    return expression<T>(function<T>(name,e));
  }
  value_type val=arg.value(*this);
  if (name=="sqrt")
    val = std::sqrt(val);
  else if (name=="abs")
    val = std::abs(val);
  else if (name=="sin")
    val = std::sin(val);
  else if (name=="cos")
    val = std::cos(val);
  else if (name=="tan")
    val = std::tan(val);
  else if (name=="asin")
    val = std::asin(evaluate_helper<T>::real(val));
  else if (name=="acos")
    val = std::acos(evaluate_helper<T>::real(val));
  else if (name=="atan")
    val = std::atan(evaluate_helper<T>::real(val));
  else if (name=="exp")
    val = std::exp(val);
  else if (name=="log")
    val = std::log(val);
  else
    return expression<T>(function<T>(name,expression<T>(val)));
  return expression<T>(val);
}

template<class T>
expression<T> evaluator<T>::partial_evaluate_function(const std::string& name, const std::vector<expression<T> >& args) const {
  if (args.size()==1)
    return partial_evaluate_function(name,args[0]);
    
  std::vector<expression<T> > evaluated;
  bool could_evaluate = true;
  for (typename std::vector<expression<T> >::const_iterator it = args.begin(); it !=args.end();++it) {
    evaluated.push_back(*it);
    could_evaluate = could_evaluate && it->can_evaluate(*this);
    evaluated.rbegin()->partial_evaluate(*this);
  }
  if (evaluated.size()==2 && could_evaluate) {
    double arg1=evaluate_helper<T>::real(evaluated[0].value());
    double arg2=evaluate_helper<T>::real(evaluated[1].value());
    if (name=="atan2")
      return expression<T>(static_cast<T>(std::atan2(arg1,arg2)));
  }
  return expression<T>(function<T>(name,evaluated));
}

} // end namespace expression
} // end namespace alps

#endif
