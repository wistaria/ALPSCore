/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <alps/params.hpp>
#include <alps/expression.hpp>

class ExpressionTest : public testing::Test {
public:
  ExpressionTest() {
    L = 10;
    T = 0.1;
    p_["L"] = L;
    p_["T"] = T;
    p_["beta"] = "1/T";
    p_["missing"] = "depends";
    p_["error"] = "error";
  }
protected:
  alps::params p_;
  int L;
  double T;
};

TEST_F(ExpressionTest, ParamsDouble) {
  EXPECT_NEAR(0.1, alps::evaluate<double>("T", p_));
  EXPECT_NEAR(10.0, alps::evaluate<double>("beta", p_));
  EXPECT_NEAR(std::sqrt(4), alps::evaluate<double>("sqrt(4)", p_));
  EXPECT_NEAR(3 + 5, alps::evaluate<double>("3 + 5", p_));
  EXPECT_NEAR(L, alps::evaluate<double>("L", p_));
  EXPECT_NEAR(T, alps::evaluate<double>("T", p_));
  EXPECT_NEAR(1.0 / L, alps::evaluate<double>("1/L", p_));
  EXPECT_NEAR(2 * M_PI / L, alps::evaluate<double>("2*Pi/L", p_));
  EXPECT_NEAR(L + T, alps::evaluate<double>("L+T", p_));
  EXPECT_NEAR(L + 10, alps::evaluate<double>("L+10", p_));
  EXPECT_NEAR(L - T, alps::evaluate<double>("L-T", p_));
  EXPECT_NEAR(1/T + 10, alps::evaluate<double>("1/T+10", p_));
  EXPECT_NEAR(L/(1/T+10), alps::evaluate<double>("L/(1/T+10)", p_));
  EXPECT_NEAR(1/T, alps::evaluate<double>("beta", p_));
  EXPECT_NEAR(sqrt(1.0 * L), alps::evaluate<double>("sqrt(L)", p_));
  EXPECT_NEAR(sin(2 * M_PI / L), alps::evaluate<double>("sin(2*Pi/L)", p_));
  EXPECT_NEAR(cos(2 * M_PI / L), alps::evaluate<double>("cos(2*Pi/L)", p_));
  EXPECT_NEAR(L * L, alps::evaluate<double>("L^2", p_));
  EXPECT_NEAR((L+1) * (L+1) * (L+1), alps::evaluate<double>("(L+1)^3", p_));
  EXPECT_NEAR(1.0 / (L+1) * 5, alps::evaluate<double>("(L+1)^-1*5", p_));
}

TEST_F(ExpressionTest, ParamsError) {
  EXPECT_ANY_THROW(alps::evaluate<double>("undefined", p_));
  EXPECT_ANY_THROW(alps::evaluate<double>("missing", p_));
  EXPECT_ANY_THROW(alps::evaluate<double>("error", p_));
}
