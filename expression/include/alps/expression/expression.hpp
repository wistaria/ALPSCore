/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EXPRESSION_HPP
#define ALPS_EXPRESSION_EXPRESSION_HPP

#include <boost/call_traits.hpp>
#include <alps/numeric/is_zero.hpp>
#include "expression_fwd.hpp"
#include "term.hpp"

namespace alps {
namespace expression {

template<class T>
class expression : public evaluatable<T> {
public:
  typedef T value_type;
  typedef term<T> term_type;
  typedef typename std::vector<term<T> >::const_iterator term_iterator;

  expression() {}
  expression(const std::string& str) { parse(str); }
  expression(std::istream& in) { parse(in); }
  template<class U>
  expression(U val, typename boost::enable_if<boost::is_arithmetic<U> >::type* = 0) : terms_(1, term<T>(value_type(val))) {}
  expression(const evaluatable<T>& e) : terms_(1,term<T>(e)) {}
  expression(const term<T>& e) : terms_(1,e) {}
  virtual ~expression() {}

  value_type value(const evaluator<T>& = evaluator<T>()) const;
  value_type value(const params& p) const { return value(params_evaluator<T>(p)); }

  bool can_evaluate(const evaluator<T>& = evaluator<T>()) const;
  bool can_evaluate(const params& p) const { return can_evaluate(params_evaluator<T>(p)); }
  void partial_evaluate(const evaluator<T>& =evaluator<T>());
  void partial_evaluate(const params& p) { partial_evaluate(params_evaluator<T>(p)); }

  void output(std::ostream& os) const;

  evaluatable<T>* clone() const { return new expression<T>(*this); }

  std::pair<term_iterator,term_iterator> terms() const {
    return std::make_pair(terms_.begin(),terms_.end());
  }

  const expression& operator+=(const term<T>& term) {
    terms_.push_back(term);
    partial_evaluate(evaluator<T>());
    return *this;
  }

  const expression& operator-=(term<T> term) {
    term.negate();
    terms_.push_back(term);
    partial_evaluate(evaluator<T>());
    return *this;
  }

  const expression& operator+=(const expression& e) {
    std::copy(e.terms_.begin(),e.terms_.end(),std::back_inserter(terms_));
    partial_evaluate(evaluator<T>());
    return *this;
  }

  const expression& operator-=(expression const& e) {
    return operator+=(-e);
  }

  const expression& operator*=(const expression<T>& e) {
    term<T> newt(factor<T>(block<T>(*this)));
    newt *= factor<T>(block<T>(e));
    terms_.clear();
    newt.remove_superfluous_parentheses();
    terms_.push_back(newt);
    partial_evaluate(evaluator<T>());
    return *this;
  }
  
  void remove_superfluous_parentheses();

  bool is_single_term() const { return terms_.size() == 1; }
  term<T> get_term() const;
  bool depends_on(const std::string&) const;

  void parse(const std::string& str);
  bool parse(std::istream& is);

  expression operator-() const { expression e(*this); e.negate(); return e;}
  const expression& negate() {
    for (typename std::vector<term<T> >::iterator it=terms_.begin();it!=terms_.end();++it)
      it->negate();
    return *this;
  } 
private:
  std::vector<term<T> > terms_;
};

//
// implementation of expression<T>
//

template<class T>
bool expression<T>::depends_on(const std::string& s) const {
  for(term_iterator it=terms().first; it!=terms().second; ++it)
    if (it->depends_on(s))
      return true;
  return false;
}

template<class T>
void expression<T>::remove_superfluous_parentheses() {
  for (typename std::vector<term<T> >::iterator it=terms_.begin();
       it!=terms_.end(); ++it)
    it->remove_superfluous_parentheses();
}

template<class T>
term<T> expression<T>::get_term() const {
  if (!is_single_term())
    boost::throw_exception(std::logic_error("Called get_term() for multi-term expression"));
  return terms_[0];
}

template<class T>
void expression<T>::parse(const std::string& str) {
  std::istringstream in(str);
  if (!parse(in))
    boost::throw_exception(std::runtime_error("Did not parse to end of string '" + str + "'"));
}

template<class T>
bool expression<T>::parse(std::istream& is) {
  terms_.clear();
  bool negate=false;
  char c;
  is >> c;
  if (is.eof())
    return true;
  if (c=='-')
    negate=true;
  else if (c=='+')
    negate=false;
  else
    is.putback(c);
  terms_.push_back(term<T>(is,negate));
  while(true) {
    if(!(is >> c))
      return true;
    if (is.eof())
      return true;
    if (c=='-')
      negate=true;
    else if (c=='+')
      negate=false;
    else {
      is.putback(c);
      return false;
    }
    terms_.push_back(term<T>(is,negate));
  }
}

template<class T>
typename expression<T>::value_type expression<T>::value(const evaluator<T>& p) const {
  if (terms_.size()==0)
    return value_type(0.);
  value_type val=terms_[0].value(p);
  for (unsigned int i=1;i<terms_.size();++i)
    val += terms_[i].value(p);
  return val;
}

template<class T>
bool expression<T>::can_evaluate(const evaluator<T>& p) const {
  if (terms_.size()==0)
    return true;
  bool can=true;

  for (unsigned int i=0;i<terms_.size();++i)
    can = can && terms_[i].can_evaluate(p);
  return can;
}

template<class T>
void expression<T>::partial_evaluate(const evaluator<T>& p) {
  if (can_evaluate(p))
    (*this) = expression<T>(value(p));
  else {
    value_type val(0);
    for (unsigned int i=0; i<terms_.size(); ++i) {
      if (terms_[i].can_evaluate(p)) {
        val += terms_[i].value(p);
        terms_.erase(terms_.begin()+i);
        --i;
      } else {
        terms_[i].partial_evaluate(p);
      }
    }
    if (val != value_type(0.)) terms_.insert(terms_.begin(), term<T>(val));
  }
}

template<class T>
void expression<T>::output(std::ostream& os) const {
  if (terms_.size()==0)
    os <<"0";
  else {
    terms_[0].output(os);
    for (unsigned int i=1;i<terms_.size();++i) {
      if(!terms_[i].is_negative())
        os << " + ";
      terms_[i].output(os);
    }
  }
}

template<class T>
inline alps::expression::expression<T> operator+(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2) {
  alps::expression::expression<T> ex(ex1);
  ex += ex2;
  return ex;
}

template<class T>
inline std::istream& operator>>(std::istream& is, alps::expression::expression<T>& e) {
  std::string s;
  is >> s;
  e.parse(s);
  return is;
}

template<class T>
inline bool operator==(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) == boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator==(const alps::expression::expression<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) == s;
}

template<class T>
inline bool operator==(const std::string& s, const alps::expression::expression<T>& ex) {
  return ex == s;
}

template<class T>
inline bool operator!=(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) !=
          boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator!=(const alps::expression::expression<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) != s;
}

template<class T>
inline bool operator!=(const std::string& s, const alps::expression::expression<T>& ex) {
  return ex != s;
}

template<class T>
inline bool operator<(const alps::expression::expression<T>& ex1, const alps::expression::expression<T>& ex2) {
  return (boost::lexical_cast<std::string>(ex1) < boost::lexical_cast<std::string>(ex2));
}

template<class T>
inline bool operator<(const alps::expression::expression<T>& ex, const std::string& s) {
  return boost::lexical_cast<std::string>(ex) < s;
}

template<class T>
inline bool operator<(const std::string& s, const alps::expression::expression<T>& ex) {
  return s < boost::lexical_cast<std::string>(ex);
}

} // end namespace expression
} // end namespace alps

#endif
