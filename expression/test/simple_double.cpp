/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <alps/expression.hpp>

TEST(ExpressionTest, SimpleDouble) {
  EXPECT_NEAR(3, alps::evaluate<double>("3"));
  EXPECT_NEAR(3 + 1, alps::evaluate<double>("3 + 1"));
  EXPECT_NEAR(3.5 * 2.1, alps::evaluate<double>("3.5 * 2.1"));
  EXPECT_NEAR(3.5 / 2.1, alps::evaluate<double>("3.5 / 2.1"));
  EXPECT_NEAR((3.5 + 0.3) / 2.1, alps::evaluate<double>("(3.5 + 0.3) / 2.1"));
  EXPECT_NEAR(2 * M_PI, alps::evaluate<double>("2 * Pi"));
  EXPECT_NEAR(2 * std::sin(M_PI/4), alps::evaluate<double>("2 * sin(Pi/4)"));
  EXPECT_NEAR(1 / std::sqrt(3), alps::evaluate<double>("1 / sqrt(3)"));
  EXPECT_NEAR(std::pow(3.2, 1.5), alps::evaluate<double>("3.2^1.5"));
}
