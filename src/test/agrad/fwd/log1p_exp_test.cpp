#include <gtest/gtest.h>
#include <stan/agrad/fvar.hpp>
#include <stan/math/functions/log1p_exp.hpp>
#include <stan/agrad/var.hpp>
#include <test/agrad/util.hpp>

TEST(log1p_exp,AgradFvar) {
  using stan::agrad::fvar;
  using stan::math::log1p_exp;
  using std::exp;

  fvar<double> x(0.5,1.0);
  fvar<double> y(1.0,2.0);
  fvar<double> z(2.0,3.0);

  fvar<double> a = log1p_exp(x);
  EXPECT_FLOAT_EQ(log1p_exp(0.5), a.val_);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), a.d_);

  fvar<double> b = log1p_exp(y);
  EXPECT_FLOAT_EQ(log1p_exp(1.0), b.val_);
  EXPECT_FLOAT_EQ(2.0 * exp(1.0) / (1 + exp(1.0)), b.d_);
}

TEST(log1p_exp,AgradFvarVar_1stderiv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p_exp;
  using std::exp;

  fvar<var> x(0.5,1.3);
  fvar<var> a = log1p_exp(x);

  EXPECT_FLOAT_EQ(log1p_exp(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(1.3 * exp(0.5) / (1 + exp(0.5)), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), g[0]);
}
TEST(log1p_exp,AgradFvarVar_2ndderiv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p_exp;
  using std::exp;

  fvar<var> x(0.5,1.3);
  fvar<var> a = log1p_exp(x);

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.d_.grad(y,g);
  EXPECT_FLOAT_EQ(1.3 * exp(0.5) / (1 + exp(0.5)) / (1 + exp(0.5)), g[0]);
}
TEST(log1p_exp,AgradFvarFvarDouble) {
  using stan::agrad::fvar;
  using stan::math::log1p_exp;
  using std::exp;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = log1p_exp(x);

  EXPECT_FLOAT_EQ(log1p_exp(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);

  fvar<fvar<double> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  a = log1p_exp(y);
  EXPECT_FLOAT_EQ(log1p_exp(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(0, a.val_.d_);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}
TEST(log1p_exp,AgradFvarFvarVar_1stderiv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p_exp;
  using std::exp;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = log1p_exp(x);

  EXPECT_FLOAT_EQ(log1p_exp(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.val_.grad(p,g);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = log1p_exp(y);
  EXPECT_FLOAT_EQ(log1p_exp(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.val_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)), r[0]);
}
TEST(log1p_exp,AgradFvarFvarVar_2ndderiv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p_exp;
  using std::exp;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = log1p_exp(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)) / (1 + exp(0.5)), g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = log1p_exp(y);

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.d_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(exp(0.5) / (1 + exp(0.5)) / (1 + exp(0.5)), r[0]);
}

