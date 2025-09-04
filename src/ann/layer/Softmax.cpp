/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X)
{
  xt::xarray<double> exp_X = xt::exp(X - xt::amax(X, {m_nAxis}));
  xt::xarray<double> sum_exp_X = xt::sum(exp_X, {m_nAxis});
  m_aCached_Y = exp_X / sum_exp_X;

  return m_aCached_Y;
}

xt::xarray<double> Softmax::backward(xt::xarray<double> DY)
{
  xt::xarray<double> JACOBIAN = xt::diag(m_aCached_Y);
  xt::xarray<double> TICHNGOAI = xt::linalg::outer(m_aCached_Y, xt::transpose(m_aCached_Y));
  xt::xarray<double> DELTA_Z = xt::linalg::dot(JACOBIAN - TICHNGOAI, DY);

  return DELTA_Z;
}

string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}