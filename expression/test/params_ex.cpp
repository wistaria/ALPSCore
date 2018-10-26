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
  EXPECT_DOUBLE_EQ(0.1, alps::evaluate("T", p_));
  EXPECT_DOUBLE_EQ(10.0, alps::evaluate("beta", p_));
  EXPECT_DOUBLE_EQ(std::sqrt(4), alps::evaluate("sqrt(4)", p_));
  EXPECT_DOUBLE_EQ(3 + 5, alps::evaluate("3 + 5", p_));
  EXPECT_DOUBLE_EQ(L, alps::evaluate("L", p_));
  EXPECT_DOUBLE_EQ(T, alps::evaluate("T", p_));
  EXPECT_DOUBLE_EQ(1.0 / L, alps::evaluate("1/L", p_));
  EXPECT_DOUBLE_EQ(2 * M_PI / L, alps::evaluate("2*Pi/L", p_));
  EXPECT_DOUBLE_EQ(L + T, alps::evaluate("L+T", p_));
  EXPECT_DOUBLE_EQ(L + 10, alps::evaluate("L+10", p_));
  EXPECT_DOUBLE_EQ(L - T, alps::evaluate("L-T", p_));
  EXPECT_DOUBLE_EQ(1/T + 10, alps::evaluate("1/T+10", p_));
  EXPECT_DOUBLE_EQ(L/(1/T+10), alps::evaluate("L/(1/T+10)", p_));
  EXPECT_DOUBLE_EQ(1/T, alps::evaluate("beta", p_));
  EXPECT_DOUBLE_EQ(sqrt(1.0 * L), alps::evaluate("sqrt(L)", p_));
  EXPECT_DOUBLE_EQ(sin(2 * M_PI / L), alps::evaluate("sin(2*Pi/L)", p_));
  EXPECT_DOUBLE_EQ(cos(2 * M_PI / L), alps::evaluate("cos(2*Pi/L)", p_));
  EXPECT_DOUBLE_EQ(L * L, alps::evaluate("L^2", p_));
  EXPECT_DOUBLE_EQ((L+1) * (L+1) * (L+1), alps::evaluate("(L+1)^3", p_));
  EXPECT_DOUBLE_EQ(1.0 / (L+1) * 5, alps::evaluate("(L+1)^-1*5", p_));
}

TEST_F(ExpressionTest, ParamsError) {
  EXPECT_ANY_THROW(alps::evaluate("undefined", p_));
  EXPECT_ANY_THROW(alps::evaluate("missing", p_));
  EXPECT_ANY_THROW(alps::evaluate("error", p_));
  EXPECT_ANY_THROW(alps::evaluate("3 + 2 * I", p_));
}
