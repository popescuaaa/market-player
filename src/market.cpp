#include <iostream>
#include <vector>
#include <cmath>

// A simple Bond class to simulate a fixed-rate bond
class Bond {
    private:
        double couponRate;
        double faceValue;
        int maturityYears;
    public:
        Bond(double couponRate, double faceValue, int maturityYears)
            : couponRate(couponRate), faceValue(faceValue), maturityYears(maturityYears) {}

        // Return the price of the bond using a basic discounting formula.
        double price(double marketRate) const {
            double price = 0.0;
            // Discount coupon payments.
            for (int i = 1; i <= maturityYears; ++i) {
                price += (faceValue * couponRate) / std::pow(1 + marketRate, i);
            }
            // Discount the face value paid at maturity.
            price += faceValue / std::pow(1 + marketRate, maturityYears);
            return price;
        }
};

// The Market class simulating a simple bond market.
class Market {
    private:
        double marketRate;
        std::vector<Bond> bonds;

    public:
        Market(double initialRate)
            : marketRate(initialRate) {}

        // Update the current market rate.
        void updateMarketRate(double newRate) {
            marketRate = newRate;
        }

        // Add a bond to the market's collection.
        void addBond(const Bond& bond) {
            bonds.push_back(bond);
        }

        // Simulate pricing for all bonds under the current market rate.
        void simulateTrading() const {
            std::cout << "Market rate: " << marketRate << "\n";
            for (size_t i = 0; i < bonds.size(); ++i) {
                double p = bonds[i].price(marketRate);
                std::cout << "Bond " << i + 1 << " price: " << p << "\n";
            }
        }
};

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
