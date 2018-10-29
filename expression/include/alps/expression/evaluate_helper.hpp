/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EVALUATE_HELPER_HPP
#define ALPS_EXPRESSION_EVALUATE_HELPER_HPP

#include <boost/call_traits.hpp>
#include <alps/numeric/is_zero.hpp>
#include "expression_fwd.hpp"

namespace alps {
namespace expression {

template<typename U, typename T>
struct numeric_cast_helper {
  static U value(typename boost::call_traits<T>::param_type x) {
    return x;
  }
};

template<typename U, typename T>
struct numeric_cast_helper<U, std::complex<T> > {
  static U value(const std::complex<T>& x) {
    if (!numeric::is_zero(x.imag()))
      boost::throw_exception(std::runtime_error("can not convert complex number into real one"));
    return x.real();
  }
};

template<typename U, typename T>
U numeric_cast(const T& x) {
  return numeric_cast_helper<U,T>::value(x);
}

template<class U>
struct evaluate_helper {
  typedef U value_type;
  template<class R>
  static value_type value(const term<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return alps::expression::numeric_cast<value_type>(ex.value(ev));
  }
  template<class R>
  static value_type value(const expression<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return alps::expression::numeric_cast<value_type>(ex.value(ev));
  }
  static value_type real(value_type u) { return u; }
  static value_type imag(value_type) { return 0; }
  static bool can_evaluate_symbol(const std::string& name) {
    return (name=="Pi" || name=="PI" || name == "pi");
  }
  static value_type evaluate_symbol(const std::string& name) {
    if (name=="Pi" || name=="PI" || name == "pi") return std::acos(value_type(-1.));
    boost::throw_exception(std::runtime_error("can not evaluate " + name));
    return 0.;
  }
};

template<class U>
struct evaluate_helper<std::complex<U> > {
  typedef U real_type;
  typedef std::complex<real_type> value_type;
  template<class R>
  static value_type value(const term<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return ex.value(ev);
  }
  template<class R>
  static value_type value(const expression<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return ex.value(ev);
  }
  static real_type real(const value_type& u) { return u.real(); }
  static real_type imag(const value_type& u) { return u.imag(); }
  static bool can_evaluate_symbol(const std::string& name) {
    return (name=="Pi" || name=="PI" || name == "pi" || name == "I");
  }
  static value_type evaluate_symbol(const std::string& name) {
    if (name=="Pi" || name=="PI" || name == "pi") return std::acos(real_type(-1.));
    if (name=="I") return value_type(0.,1.);
    boost::throw_exception(std::runtime_error("can not evaluate " + name));
    return 0.;
  }
};

} // end namespace expression
} // end namespace alps

#endif
