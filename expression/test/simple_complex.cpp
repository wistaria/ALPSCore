/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <alps/expression.hpp>

typedef std::complex<double> complex;

TEST(ExpressionTest, SimpleComplex) {
  EXPECT_EQ(complex(3), alps::evaluate<complex>("3"));
  EXPECT_EQ(complex(2,3), alps::evaluate<complex>("2+3*I"));
  EXPECT_EQ(complex(2,3), alps::evaluate<complex>("(2,3)"));
  EXPECT_EQ(complex(1,2) * complex(3,4), alps::evaluate<complex>("(1,2) * (3,4)"));
  EXPECT_EQ(2.0 / complex(0,1), alps::evaluate<complex>("2/I"));
  EXPECT_EQ(sqrt(-complex(2)), alps::evaluate<complex>("sqrt(-2)"));
  EXPECT_EQ(sin(complex(2,3)), alps::evaluate<complex>("sin(2+3*I)"));
}
