/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_FACTOR_HPP
#define ALPS_EXPRESSION_FACTOR_HPP

#include <boost/call_traits.hpp>
#include <boost/lexical_cast.hpp>
#include "expression_fwd.hpp"
#include "symbol.hpp"
#include "number.hpp"
#include "block.hpp"
#include "function.hpp"
#include "evaluator.hpp"

namespace alps {
namespace expression {

template<class T>
class simple_factor : public evaluatable<T> {
public:
  typedef T value_type;
  
  simple_factor(std::istream&);
  simple_factor(typename boost::call_traits<value_type>::param_type x)
    : term_(new number<T>(x)) {}
  simple_factor(const std::string& s) : term_(new symbol<T>(s)) {}

  simple_factor(const simple_factor& v)
    : evaluatable<T>(v), term_()
  {
    if (v.term_) term_.reset(v.term_->clone());
  }
  
  simple_factor(const evaluatable<T>& v) : evaluatable<T>(v), term_(v.clone()) {}
  virtual ~simple_factor() {}

  const simple_factor& operator=(const simple_factor& v);

  value_type value(const evaluator<T>& =evaluator<T>()) const;
  void output(std::ostream&) const;
  bool can_evaluate(const evaluator<T>& =evaluator<T>()) const;
  evaluatable<T>* clone() const { return new simple_factor<T>(*this); }
  void partial_evaluate(const evaluator<T>& =evaluator<T>());
  bool is_single_term() const { return term_ ? term_->is_single_term() : false; }
  term<T> get_term() const { return term_ ? term_->get_term() : term<T>(); }
  bool depends_on(const std::string& s) const
  {
    return term_ ? term_->depends_on(s) : false;
  }

protected:
  boost::shared_ptr<evaluatable<T> > term_;
};

template<class T>
class factor : public simple_factor<T> {
public:
  typedef T value_type;
  typedef simple_factor<T> super_type;
  
  factor(std::istream&, bool inverse = false);
  factor(typename boost::call_traits<value_type>::param_type x)
    : super_type(x), is_inverse_(false), power_(1.) {}
  factor(const std::string& s) : super_type(s), is_inverse_(false), power_(1.) {}
  factor(const evaluatable<T>& v) : super_type(v), is_inverse_(false), power_(1.) {}
  factor(const super_type& v) : super_type(v), is_inverse_(false), power_(1.) {}
  virtual ~factor() {}
  value_type value(const evaluator<T>& =evaluator<T>()) const;
  void output(std::ostream&) const;
  bool can_evaluate(const evaluator<T>& =evaluator<T>()) const;
  evaluatable<T>* clone() const { return new factor<T>(*this); }
  bool is_inverse() const { return is_inverse_; }
  void partial_evaluate(const evaluator<T>& =evaluator<T>());
  term<T> get_term() const { return unit_power() ? super_type::get_term() : (super_type::term_ ? term<T>(*this) : term<T>()); }
  bool depends_on(const std::string& s) const
  {
    return super_type::depends_on(s) || power_.depends_on(s);
  }
  bool unit_power() const { return power_.can_evaluate() && power_.value() ==1.;}
  bool is_single_term() const { return super_type::is_single_term() && unit_power(); }

private:
  bool is_inverse_;
  simple_factor<T> power_;
};

//
// implementation of factor<T>
//

template<class T>
simple_factor<T>::simple_factor(std::istream& in) : term_() {
  char c;
  in >> c;

  // read value
  if (std::isdigit(c) || c=='.' || c=='+' || c=='-') {
    in.putback(c);
    typename number<T>::real_type val;
    in >> val;
    if (!in)
      boost::throw_exception(std::runtime_error("Failed to parse number in factor"));
    term_.reset(new number<T>(value_type(val)));
  }
  else if (std::isalnum(c)) {
    in.putback(c);
    std::string name = parse_parameter_name(in);
    in>>c;
    if(in && c=='(')
      term_.reset(new function<T>(in,name));
    else  {
      if (in && !in.eof())
        in.putback(c);
      term_.reset(new symbol<T>(name));
    }
  }
  else if (c=='(')
    term_.reset(new block<T>(in));
  else
    boost::throw_exception(std::runtime_error("Illegal term in expression"));
}

template<class T>
factor<T>::factor(std::istream& in, bool inv) : super_type(in), is_inverse_(inv), power_(1.) {
  char c;
  in >> c;
  if (in) {
    if (c=='^') {
      simple_factor<T> p(in);
      power_=p;
    }
    else
      in.putback(c);
  }
}

template<class T>
void simple_factor<T>::partial_evaluate(const evaluator<T>& p) {
  if (!term_)
    boost::throw_exception(std::runtime_error("Empty value in expression"));
  evaluatable<T>* e=term_->partial_evaluate_replace(p);
  if(e!=term_.get()) term_.reset(e);
}

template<class T>
void factor<T>::partial_evaluate(const evaluator<T>& p) {
  super_type::partial_evaluate(p);
  power_.partial_evaluate(p);
}

template<class T>
const simple_factor<T>& simple_factor<T>::operator=(const simple_factor<T>& v) {
  if (v.term_)
    term_.reset(v.term_->clone());
  else
    term_.reset();
  return *this;
}

template<class T>
bool simple_factor<T>::can_evaluate(const evaluator<T>& p) const {
  if (!term_)
    boost::throw_exception(std::runtime_error("Empty value in expression"));
  return term_->can_evaluate(p);
}

template<class T>
bool factor<T>::can_evaluate(const evaluator<T>& p) const {
  return super_type::can_evaluate(p) && power_.can_evaluate(p);
}

template <class T>
typename simple_factor<T>::value_type simple_factor<T>::value(const evaluator<T>& p) const {
  if (!term_)
    boost::throw_exception(std::runtime_error("Empty value in expression"));
  return term_->value(p);
}

template <class T>
typename factor<T>::value_type factor<T>::value(const evaluator<T>& p) const {
  value_type val = super_type::value(p);
  if (is_inverse())
    val = 1./val;
  if (!unit_power())
    val = std::pow(evaluate_helper<T>::real(val),evaluate_helper<T>::real(power_.value(p)));
  return val;
}

template<class T>
void simple_factor<T>::output(std::ostream& os) const {
  if (!term_)
    boost::throw_exception(std::runtime_error("Empty value in expression"));
  term_->output(os);
}

template<class T>
void factor<T>::output(std::ostream& os) const {
  super_type::output(os);
  if (!unit_power())
    os << "^" << power_;
}

template<class T>
inline bool operator==(const alps::expression::factor<T>& ex1, const alps::expression::factor<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) ==
          boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator==(const alps::expression::factor<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) == s;
}

template<class T>
inline bool operator==(const std::string& s, const alps::expression::factor<T>& ex) {
  return ex == s;
}

template<class T>
inline bool operator<(const alps::expression::factor<T>& ex1, const alps::expression::factor<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) < boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator<(const alps::expression::factor<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) < s;
}

template<class T>
inline bool operator<(const std::string& s, const alps::expression::factor<T>& ex) {
  return s < boost::lexical_cast<std::string>(ex);
}

} // end namespace expression
} // end namespace alps

#endif
