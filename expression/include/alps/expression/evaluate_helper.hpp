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
  static U value(typename boost::call_traits<T>::param_type x)
  {
    return x;
  }
};

template<typename U, typename T>
struct numeric_cast_helper<U, std::complex<T> > {
  static U value(const std::complex<T>& x) {
    if (! numeric::is_zero(x.imag()))
      boost::throw_exception(std::runtime_error("can not convert complex number into real one"));
    return x.real();
  }
};

#ifndef BOOST_NO_SFINAE
template<typename U, typename T>
U numeric_cast(T x, typename boost::enable_if<boost::is_arithmetic<T> >::type* = 0)
{
  return numeric_cast_helper<U,T>::value(x);
}
template<typename U, typename T>
U numeric_cast(const T& x, typename boost::disable_if<boost::is_arithmetic<T> >::type* = 0)
{
  return numeric_cast_helper<U,T>::value(x);
}
#else
template<typename U, typename T>
U numeric_cast(const T& x)
{
  return numeric_cast_helper<U,T>::value(x);
}
#endif

template<class U>
struct evaluate_helper
{
  typedef U value_type;
  template<class R>
  static U value(const term<R>& ex, const evaluator<R>& =evaluator<R>()) { return ex; }
  template<class R>
  static U value(const expression<R>& ex, const evaluator<R>& =evaluator<R>()) { return ex; }
  static U real(typename boost::call_traits<U>::param_type u) { return u; }
};

template<class U>
struct evaluate_helper<expression<U> >
{
  typedef U value_type;
  static expression<U> value(const term<U>& ex, const evaluator<U>& ev=evaluator<U>()) {
    term<U> t(ex);
    t.partial_evaluate(ev);
    return t;
  }
  static expression<U> value(const expression<U>& ex, const evaluator<U>& ev=evaluator<U>()) {
    expression<U> e(ex);
    e.partial_evaluate(ev);
    return e;
  }
};

template<>
struct evaluate_helper<double>
{
  typedef double value_type;
  template<class R>
  static double value(const term<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return alps::expression::numeric_cast<double>(ex.value(ev));
  }
  template<class R>
  static double value(const expression<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return alps::expression::numeric_cast<double>(ex.value(ev));
  }
  static double real(double u) { return u; }
  static double imag(double) { return 0; }
  static bool can_evaluate_symbol(const std::string& name) {
    return (name=="Pi" || name=="PI" || name == "pi");
  }
  static double evaluate_symbol(const std::string& name) {
    if (name=="Pi" || name=="PI" || name == "pi") return std::acos(-1.);
    boost::throw_exception(std::runtime_error("can not evaluate " + name));
    return 0.;
  }
};

template<>
struct evaluate_helper<float>
{
  typedef float value_type;
  template<class R>
  static float value(const term<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return numeric_cast<float>(ex.value(ev));
  }
  template<class R>
  static float value(const expression<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return numeric_cast<float>(ex.value(ev));
  }
  static float real(float u) { return u; }
  static float imag(float) { return 0; }
  static bool can_evaluate_symbol(const std::string& name) {
    return (name=="Pi" || name=="PI" || name == "pi");
  }
  static float evaluate_symbol(const std::string& name) {
    if (name=="Pi" || name=="PI" || name == "pi") return static_cast<float>(std::acos(-1.));
    boost::throw_exception(std::runtime_error("can not evaluate " + name));
    return 0.;
  }
};

template<>
struct evaluate_helper<long double>
{
  typedef long double value_type;
  template<class R>
  static long double value(const term<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return numeric_cast<long double>(ex.value(ev));
  }
  template<class R>
  static long double value(const expression<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return numeric_cast<long double>(ex.value(ev));
  }
  static long double real(long double u) { return u; }
  static long double imag(long double) { return 0; }
  static bool can_evaluate_symbol(const std::string& name) {
    return (name=="Pi" || name=="PI" || name == "pi");
  }
  static long double evaluate_symbol(const std::string& name) {
    if (name=="Pi" || name=="PI" || name == "pi") return std::acos(-1.);
    boost::throw_exception(std::runtime_error("can not evaluate " + name));
    return 0.;
  }
};

template<class U>
struct evaluate_helper<std::complex<U> >
{
  typedef std::complex<U> value_type;
  template<class R>
  static std::complex<U> value(const term<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return ex.value(ev);
  }
  template<class R>
  static std::complex<U> value(const expression<R>& ex, const evaluator<R>& ev=evaluator<R>()) {
    return ex.value(ev);
  }
  static U real(const std::complex<U>& u) { return u.real(); }
  static U imag(const std::complex<U>& u) { return u.imag(); }
  static bool can_evaluate_symbol(const std::string& name) {
    return (name=="Pi" || name=="PI" || name == "pi" || name == "I");
  }
  static std::complex<U> evaluate_symbol(const std::string& name) {
    if (name=="Pi" || name=="PI" || name == "pi") return std::acos(-1.);
    if (name=="I") return std::complex<U>(0.,1.);
    boost::throw_exception(std::runtime_error("can not evaluate " + name));
    return 0.;
  }
};

} // end namespace expression
} // end namespace alps

#endif
