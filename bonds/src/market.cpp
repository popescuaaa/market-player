#include "market.hpp"
#include <iostream>

Market::Market(double initialRate) : marketRate(initialRate) {}

// Update the current market rate.
void Market::updateMarketRate(double newRate) { marketRate = newRate; }

// Add a bond to the market's collection.
void Market::addBond(const Bond &bond) { bonds.push_back(bond); }

// Simulate pricing for all bonds under the current market rate.
void Market::simulateTrading() {
  std::cout << "Market rate: " << marketRate << "\n";
  for (size_t i = 0; i < bonds.size(); ++i) {
    double p = bonds[i].price(marketRate);
    std::cout << "Bond " << i + 1 << " price: " << p << "\n";
  }
}
