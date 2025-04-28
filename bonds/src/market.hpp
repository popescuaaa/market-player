#ifndef MARKET_HPP
#define MARKET_HPP

#include "bond.hpp"
#include <vector>

class Market {
private:
  double marketRate;
  std::vector<Bond> bonds;

public:
  Market(double initialRate);

  // Update the current market rate.
  void updateMarketRate(double newRate);

  // Add a bond to the market's collection.
  void addBond(const Bond &bond);

  // Simulate pricing for all bonds under the current market rate.
  void simulateTrading();
};

#endif // MARKET_HPP
