/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EXPRESSION_FWD_HPP
#define ALPS_EXPRESSION_EXPRESSION_FWD_HPP

#include <complex>
#include <boost/mpl/bool.hpp>

namespace alps {
namespace expression {

template<class T = std::complex<double> > class expression;
template<class T = std::complex<double> > class term;
template<class T = std::complex<double> > class factor;
template<class T = std::complex<double> > class evaluator;
template<class T = std::complex<double> > class params_evaluator;
template <class T> class block;
template <class T> class function;
template <class T> class number;
template <class T> class symbol;

}
}

#endif
