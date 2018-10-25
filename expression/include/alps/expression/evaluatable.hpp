/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_EXPRESSION_EVALUATABLE_HPP
#define ALPS_EXPRESSION_EVALUATABLE_HPP

#include <boost/shared_ptr.hpp>
#include "expression_fwd.hpp"

namespace alps {
namespace expression {

template<class T>
class evaluatable {
public:
  typedef T value_type;

  evaluatable() {}
  virtual ~evaluatable() {}
  virtual value_type value(const evaluator<T>& =evaluator<T>()) const = 0;
  virtual bool can_evaluate(const evaluator<T>& =evaluator<T>()) const = 0;
  virtual void output(std::ostream&) const = 0;
  virtual evaluatable* clone() const = 0;
  virtual evaluatable* partial_evaluate_replace(const evaluator<T>& =evaluator<T>()) { return this; }
  virtual bool is_single_term() const { return false; }
  virtual term<T> get_term() const;
  virtual bool depends_on(const std::string&) const { return false; }
};

template<class T>
inline term<T> evaluatable<T>::get_term() const { return term<T>(); }

template<class T>
inline std::ostream& operator<<(std::ostream& os, const alps::expression::evaluatable<T>& e) {
  e.output(os);
  return os;
}

} // end namespace expression
} // end namespace alps

#endif
