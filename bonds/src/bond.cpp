#include "bond.hpp"
#include <cmath>

// A simple Bond class to simulate a fixed-rate bond
Bond::Bond(double couponRate, double faceValue, int maturityYears)
    : couponRate(couponRate), faceValue(faceValue),
      maturityYears(maturityYears) {}

double Bond::price(double marketRate) {
  double price = 0.0;
  // Discount coupon payments.
  for (int i = 1; i <= maturityYears; ++i) {
    price += (faceValue * couponRate) / std::pow(1 + marketRate, i);
  }
  // Discount the face value paid at maturity.
  price += faceValue / std::pow(1 + marketRate, maturityYears);
  return price;
}
