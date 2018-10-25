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
  evaluator() {}
  virtual ~evaluator() {}

  virtual bool can_evaluate(const std::string&, bool=false) const;
  virtual bool can_evaluate_function(const std::string&, const expression<T>&, bool=false) const;
  virtual bool can_evaluate_function(const std::string&, const std::vector<expression<T> >&, bool=false) const;
  virtual value_type evaluate(const std::string&, bool=false) const;
  virtual value_type evaluate_function(const std::string&, const expression<T>&, bool=false) const;
  virtual value_type evaluate_function(const std::string&, const std::vector<expression<T> >&, bool=false) const;
  virtual expression<T> partial_evaluate(const std::string& name, bool=false) const;
  virtual expression<T> partial_evaluate_function(const std::string& name, const expression<T>&, bool=false) const;
  virtual expression<T> partial_evaluate_function(const std::string& name, const std::vector<expression<T> >&, bool=false) const;
  bool can_evaluate_expressions(const std::vector<expression<T> >&, bool=false) const;
  void partial_evaluate_expressions(std::vector<expression<T> >&, bool=false) const;
};


template<class T>
class params_evaluator : public evaluator<T> {
public:
  typedef evaluator<T> super_type;
  typedef T value_type;
  params_evaluator(const params& p) 
  : evaluator<T>(), parms_(p) {}
  virtual ~params_evaluator() {}

  bool can_evaluate(const std::string&, bool=false) const;
  value_type evaluate(const std::string&, bool=false) const;
  expression<T> partial_evaluate(const std::string& name, bool=false) const;
  const params& parameters() const { return parms_;}
private:
  params parms_;
};

template<class T>
bool evaluator<T>::can_evaluate(const std::string& name, bool isarg) const {
  return evaluate_helper<T>::can_evaluate_symbol(name, isarg);
}

template<class T>
bool evaluator<T>::can_evaluate_function(const std::string& name, const expression<T>& arg, bool) const
{
  return arg.can_evaluate(*this,true) &&
         (name=="sqrt" || name=="abs" ||
          name=="sin" || name=="cos" || name=="tan" ||
          name=="asin" || name=="acos" || name=="atan" ||
          name=="log" || name=="exp");
}


template<class T>
bool evaluator<T>::can_evaluate_expressions(const std::vector<expression<T> >& arg, bool f) const
{
  bool can=true;
  for (typename std::vector<expression<T> >::const_iterator it=arg.begin();it!=arg.end();++it)
    can = can && it->can_evaluate(*this,f);
  return can;
}

template<class T>
void evaluator<T>::partial_evaluate_expressions(std::vector<expression<T> >& arg, bool f) const
{
  for (typename std::vector<expression<T> >::iterator it=arg.begin();it!=arg.end();++it) {
   it->partial_evaluate(*this,f);
   // it->simplify();
  }
}


template<class T>
bool evaluator<T>::can_evaluate_function(const std::string& name, const std::vector<expression<T> >& arg, bool f) const
{
  bool can = can_evaluate_expressions(arg, true) &&
    (arg.size() == 1 && can_evaluate_function(name,arg[0], f));
  return can;
}

template<class T>
typename evaluator<T>::value_type evaluator<T>::evaluate(const std::string& name,bool isarg) const
{
  if (evaluate_helper<T>::can_evaluate_symbol(name, isarg))
    return evaluate_helper<T>::evaluate_symbol(name, isarg);
  return partial_evaluate(name, isarg).value();
}

template<class T>
typename evaluator<T>::value_type evaluator<T>::evaluate_function(const std::string& name, const expression<T>& arg,bool isarg) const
{
  return partial_evaluate_function(name,arg,isarg).value();
}

template<class T>
typename evaluator<T>::value_type evaluator<T>::evaluate_function(const std::string& name, const std::vector<expression<T> >& arg,bool isarg) const
{
  return partial_evaluate_function(name,arg,isarg).value();
}

template<class T>
expression<T> evaluator<T>::partial_evaluate(const std::string& name,bool) const
{
  return expression<T>(name);
}


template<class T>
expression<T> evaluator<T>::partial_evaluate_function(const std::string& name, const expression<T>& arg,bool) const
{
  if(!arg.can_evaluate(*this,true)) {
    expression<T> e(arg);
    e.partial_evaluate(*this,true);
    return expression<T>(function<T>(name,e));
  }
  value_type val=arg.value(*this,true);
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
expression<T> evaluator<T>::partial_evaluate_function(const std::string& name, const std::vector<expression<T> >& args,bool isarg) const
{
  if (args.size()==1)
    return partial_evaluate_function(name,args[0],isarg);
    
  std::vector<expression<T> > evaluated;
  bool could_evaluate = true;
  for (typename std::vector<expression<T> >::const_iterator it = args.begin(); it !=args.end();++it) {
    evaluated.push_back(*it);
    could_evaluate = could_evaluate && it->can_evaluate(*this,true);
    evaluated.rbegin()->partial_evaluate(*this,true);
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
