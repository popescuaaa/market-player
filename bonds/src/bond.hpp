#ifndef BOND_HPP
#define BOND_HPP

class Bond {
private:
  double couponRate;
  double faceValue;
  int maturityYears;

public:
  Bond(double couponRate, double faceValue, int maturityYears);
  double price(double marketRate);
};

#endif // BOND_HPP
