/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_TERM_HPP
#define ALPS_EXPRESSION_TERM_HPP

#include <boost/call_traits.hpp>
#include <alps/numeric/is_zero.hpp>
#include "expression_fwd.hpp"
#include "factor.hpp"

namespace alps {
namespace expression {

template<class T>
class term : public evaluatable<T> {
public:
  typedef T value_type;
  typedef typename std::vector<factor<T> >::const_iterator factor_iterator;

  term(std::istream& is, bool negate = false);
  term() : is_negative_(false) {}
  term(typename boost::call_traits<value_type>::param_type x)
    : is_negative_(false), terms_(1,factor<T>(x)) {}
  term(const evaluatable<T>& e)
    : is_negative_(false), terms_(1,factor<T>(e)) {}
  term(const std::pair<T,term<T> >&);
  virtual ~term() {}

  value_type value(const evaluator<T>& =evaluator<T>()) const;

  bool can_evaluate(const evaluator<T>& =evaluator<T>()) const;
  void output(std::ostream&) const;
  evaluatable<T>* clone() const { return new term<T>(*this); }
  bool is_negative() const { return is_negative_;}
  void partial_evaluate(const evaluator<T>& =evaluator<T>());
  std::pair<T,term<T> > split() const;
  
  const term& operator*=(const factor<T>& v) {
    terms_.push_back(v);
    return *this;
  }

  friend term operator*(const factor<T>& v, term<T> t) {
      t.terms_.insert(t.terms_.begin(), v);
      return t;
  }

  const term& operator*=(const std::string& s) {
    return operator*=(factor<T>(s));
  }
  
  void remove_superfluous_parentheses();

  virtual std::pair<factor_iterator,factor_iterator> factors() const {
    return std::make_pair(terms_.begin(),terms_.end());
  }

  bool depends_on(const std::string&) const;
  bool depends_only_on(const std::string&) const;

  int num_factors() const {return terms_.size(); }
  void negate() { is_negative_ = !is_negative_;}

private:
  bool is_negative_;
  std::vector<factor<T> > terms_;
};

template <class T>
struct term_less {
  bool operator()(const term<T>& x, const term<T>& y) {
    return x.split().second < y.split().second;
  }
};

template<class T>
bool term<T>::depends_on(const std::string& s) const {
  for (factor_iterator it=factors().first; it!=factors().second; ++it)
    if(it->depends_on(s))
      return true;
  return false;
}

template<class T>
bool term<T>::depends_only_on(const std::string& s) const {
  if (!depends_on(s))
    return false;

  for (factor_iterator it=factors().first; it!=factors().second; ++it)
  {
    if (it->depends_on(s))
      continue;
    try {
      double tmp = boost::lexical_cast<double>(*it);
      static_cast<void>(tmp); // fix unused variable warning
      continue;
    }
    catch(...) {
      return false;
    }
  }

  return true;
}

template<class T>
void term<T>::remove_superfluous_parentheses() {
  std::vector<factor<T> > s;
  for (typename std::vector<factor<T> >::iterator it = terms_.begin();
       it != terms_.end(); ++it) {
    if (it->is_single_term()) {
      term<T> t = it->get_term();
      if (t.is_negative()) negate();
      std::copy(t.factors().first, t.factors().second,
                std::back_inserter(s));
    } else
      s.push_back(*it);
  }
  terms_ = s;
}

template<class T>
term<T>::term(std::istream& in, bool negate) : is_negative_(negate) {
  bool is_inverse=false;
  terms_.push_back(factor<T>(in,is_inverse));
  while (true) {
    char c;
    if (!(in>>c))
      break;
    if (in.eof())
      break;
    switch(c) {
      case '*':
        is_inverse=false;
        break;
      case '/':
        is_inverse=true;
        break;
      default:
        in.putback(c);
        return;
    }
    terms_.push_back(factor<T>(in,is_inverse));
  }
}

template<class T>
typename term<T>::value_type term<T>::value(const evaluator<T>& p) const {
  value_type val(1.);
  for (unsigned int i = 0; i < terms_.size() && (!numeric::is_zero(val)); ++i)
    val *= terms_[i].value(p);
  if (is_negative() && (!numeric::is_zero(val)))
    val = val*(-1.);
  return val;
}

template<class T>
void term<T>::partial_evaluate(const evaluator<T>& p) {
  if (can_evaluate(p)) {
    (*this) = term<T>(value(p));
  } else {
    value_type val(1);
    for (unsigned int i=0; i<terms_.size(); ++i) {
      if (terms_[i].can_evaluate(p)) {
        val *= terms_[i].value(p);
        if (numeric::is_zero(val))
          break;
        terms_.erase(terms_.begin()+i);
        --i;
      } else {
        terms_[i].partial_evaluate(p);
      }
    }
    if (numeric::is_zero(val))
      (*this) = term<T>(value_type(0.));
    else {
      if (evaluate_helper<T>::real(val) < 0.) {
        is_negative_=!is_negative_;
        val=-val;
      }
      if (val != value_type(1.))
        terms_.insert(terms_.begin(), factor<T>(val));
    }
  }
  remove_superfluous_parentheses();
}

template<class T>
bool term<T>::can_evaluate(const evaluator<T>& p) const {
  bool can = true;
  for (unsigned int i=0;i<terms_.size();++i)
    can = can && terms_[i].can_evaluate(p);
  return can;
}

template<class T>
void term<T>::output(std::ostream& os) const {
  if (terms_.empty()) {
    os << "0";
    return;
  }
  if(is_negative())
    os << " - ";
  terms_[0].output(os);
  for (unsigned int i=1;i<terms_.size();++i) {
    os << " " << (terms_[i].is_inverse() ? "/" : "*") << " ";
    terms_[i].output(os);
  }
}

template<class T>
term<T>::term(const std::pair<T,term<T> >& t) :  is_negative_(false),terms_(t.second.terms_) {
  terms_.insert(terms_.begin(), factor<T>(t.first));
  partial_evaluate(evaluator<T>());
}

template<class T>
std::pair<T,term<T> > term<T>::split() const {
  term<T> t(*this);
  t.partial_evaluate(evaluator<T>());
  T val=1.;
  if (t.terms_.empty())
    val=0.;
  else
    if (t.terms_[0].can_evaluate()) {
    val=t.terms_[0].value();
    t.terms_.erase(t.terms_.begin());
  }
  if (t.is_negative_) 
    val=-val;
  t.is_negative_=false;
  return std::make_pair(val,t);
}

template<class T>
inline bool operator==(const alps::expression::term<T>& ex1, const alps::expression::term<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) == boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator==(const alps::expression::term<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) == s;
}

template<class T>
inline bool operator==(const std::string& s, const alps::expression::term<T>& ex) {
  return ex == s;
}

template<class T>
inline bool operator<(const alps::expression::term<T>& ex1, const alps::expression::term<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) < boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator<(const alps::expression::term<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) < s;
}

template<class T>
inline bool operator<(const std::string& s, const alps::expression::term<T>& ex) {
  return s < boost::lexical_cast<std::string>(ex);
}

} // end namespace expression
} // end namespace alps

#endif
