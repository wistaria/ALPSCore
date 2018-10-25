/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_SYMBOL_HPP
#define ALPS_EXPRESSION_SYMBOL_HPP

#include "expression_fwd.hpp"
#include "evaluatable.hpp"

namespace alps {
namespace expression {

template<class T>
class symbol : public evaluatable<T> {
public:
  typedef T value_type;

  symbol(const std::string& n) : name_(n) {}
  value_type value(const evaluator<T>& = evaluator<T>()) const;
  bool can_evaluate(const evaluator<T>& ev = evaluator<T>()) const {
    return ev.can_evaluate(name_);
  }
  void output(std::ostream& os) const { os << name_; }
  evaluatable<T>* clone() const { return new symbol<T>(*this); }
  evaluatable<T>* partial_evaluate_replace(const evaluator<T>& =evaluator<T>());
  bool depends_on(const std::string& s) const;
private:
  std::string name_;
};

template<class T>
bool symbol<T>::depends_on(const std::string& s) const {
  return (name_==s);
}

template<class T>
typename symbol<T>::value_type symbol<T>::value(const evaluator<T>& eval) const {
  if (!eval.can_evaluate(name_))
    boost::throw_exception(std::runtime_error("Cannot evaluate " + name_ ));
  return eval.evaluate(name_);
}

template<class T>
evaluatable<T>* symbol<T>::partial_evaluate_replace(const evaluator<T>& p) {
  expression<T> e(p.partial_evaluate(name_));
  if (e == name_)
    return this;
  else
    return new block<T>(p.partial_evaluate(name_));
}

} // end namespace expression
} // end namespace alps

#endif
