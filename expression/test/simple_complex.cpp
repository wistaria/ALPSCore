/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <alps/expression.hpp>

typedef std::complex<double> complex;

TEST(ExpressionTest, SimpleComplex) {
  EXPECT_DOUBLE_EQ(real(complex(3)), real(alps::evaluate<complex>("3")));
  EXPECT_DOUBLE_EQ(imag(complex(3)), imag(alps::evaluate<complex>("3")));
  EXPECT_DOUBLE_EQ(real(complex(2,3)), real(alps::evaluate<complex>("2+3*I")));
  EXPECT_DOUBLE_EQ(imag(complex(2,3)), imag(alps::evaluate<complex>("2+3*I")));
  EXPECT_DOUBLE_EQ(real(complex(2,3)), real(alps::evaluate<complex>("(2,3)")));
  EXPECT_DOUBLE_EQ(imag(complex(2,3)), imag(alps::evaluate<complex>("(2,3)")));
  EXPECT_DOUBLE_EQ(real(complex(1,2) * complex(3,4)), real(alps::evaluate<complex>("(1,2) * (3,4)")));
  EXPECT_DOUBLE_EQ(imag(complex(1,2) * complex(3,4)), imag(alps::evaluate<complex>("(1,2) * (3,4)")));
  EXPECT_DOUBLE_EQ(real(2.0 / complex(0,1)), real(alps::evaluate<complex>("2/I")));
  EXPECT_DOUBLE_EQ(imag(2.0 / complex(0,1)), imag(alps::evaluate<complex>("2/I")));
  EXPECT_DOUBLE_EQ(real(sqrt(-complex(2))), real(alps::evaluate<complex>("sqrt(-2)")));
  EXPECT_DOUBLE_EQ(imag(sqrt(-complex(2))), imag(alps::evaluate<complex>("sqrt(-2)")));
  EXPECT_DOUBLE_EQ(real(sin(complex(2,3))), real(alps::evaluate<complex>("sin(2+3*I)")));
  EXPECT_DOUBLE_EQ(imag(sin(complex(2,3))), imag(alps::evaluate<complex>("sin(2+3*I)")));
}
