#include "bond.hpp"
#include "market.hpp"
#include <iostream>

int main() {
    // Initialize the market with a 5% interest rate.
    Market market(0.05);

    // Create some bonds.
    Bond bond1(0.05, 1000, 5);  // 5% coupon, 5-year bond
    Bond bond2(0.06, 1000, 10); // 6% coupon, 10-year bond

    // Add bonds to the market.
    market.addBond(bond1);
    market.addBond(bond2);

    // Simulate trading with the initial market rate.
    std::cout << "Initial market state:\n";
    market.simulateTrading();

    // Update market rate to 4% and re-simulate.
    market.updateMarketRate(0.04);
    std::cout << "\nAfter updating market rate:\n";
    market.simulateTrading();

    return 0;
}
