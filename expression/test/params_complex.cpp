/*
 * Copyright (C) 1998-2018 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "gtest/gtest.h"
#include <complex>
#include <alps/params.hpp>
#include <alps/expression.hpp>

typedef std::complex<double> complex;

class ExpressionTest : public testing::Test {
public:
  ExpressionTest() {
    L = 10;
    T = complex(0, 0.1);
    p_["L"] = L;
    p_["T"] = "0.1 * I";
    p_["beta"] = "1/T";
    p_["error"] = "error";
  }
protected:
  alps::params p_;
  double L;
  complex T;
};

TEST_F(ExpressionTest, ParamsComplex) {
  EXPECT_DOUBLE_EQ(real(T), real(alps::evaluate<complex>("T", p_)));
  EXPECT_DOUBLE_EQ(imag(T), imag(alps::evaluate<complex>("T", p_)));
  EXPECT_DOUBLE_EQ(real(L + T), real(alps::evaluate<complex>("L+T", p_)));
  EXPECT_DOUBLE_EQ(imag(L + T), imag(alps::evaluate<complex>("L+T", p_)));
  EXPECT_DOUBLE_EQ(real(L - T), real(alps::evaluate<complex>("L-T", p_)));
  EXPECT_DOUBLE_EQ(imag(L - T), imag(alps::evaluate<complex>("L-T", p_)));
  EXPECT_DOUBLE_EQ(real(1.0/T + 10.0), real(alps::evaluate<complex>("1/T+10", p_)));
  EXPECT_DOUBLE_EQ(imag(1.0/T + 10.0), imag(alps::evaluate<complex>("1/T+10", p_)));
  EXPECT_DOUBLE_EQ(real(L/(1.0/T+10.0)), real(alps::evaluate<complex>("L/(1/T+10)", p_)));
  EXPECT_DOUBLE_EQ(imag(L/(1.0/T+10.0)), imag(alps::evaluate<complex>("L/(1/T+10)", p_)));
  EXPECT_DOUBLE_EQ(real(1.0/T), real(alps::evaluate<complex>("beta", p_)));
  EXPECT_DOUBLE_EQ(imag(1.0/T), imag(alps::evaluate<complex>("beta", p_)));
  EXPECT_DOUBLE_EQ(sqrt(L), real(alps::evaluate<complex>("sqrt(L)", p_)));
  EXPECT_DOUBLE_EQ(sin(2 * M_PI / L), real(alps::evaluate<complex>("sin(2*Pi/L)", p_)));
  EXPECT_DOUBLE_EQ(cos(2 * M_PI / L), real(alps::evaluate<complex>("cos(2*Pi/L)", p_)));
  EXPECT_DOUBLE_EQ(L * L, real(alps::evaluate<complex>("L^2", p_)));
  EXPECT_DOUBLE_EQ((L+1) * (L+1) * (L+1), real(alps::evaluate<complex>("(L+1)^3", p_)));
  EXPECT_DOUBLE_EQ(1.0 / (L+1) * 5, real(alps::evaluate<complex>("(L+1)^-1*5", p_)));
}

TEST_F(ExpressionTest, ParamsError) {
  EXPECT_ANY_THROW(alps::evaluate<complex>("error", p_));
}
