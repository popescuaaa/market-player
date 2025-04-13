#include <string>
#include <iostream>
#include <vector>
#include <sstream>

// Helpers
struct Bond {
    std::string bondId;
    float price;
    int quantity;
    std::string actionType; // buy, sell
    std::string status; // open ?
    std::string orderId;
};

class Trader {
    private:
        std::string traderName;
        std::vector<Bond> portofolio;

        float emaBondPrice = 0;

        std::vector<std::string> splitString(std::string line, char delim) {

            std::vector<std::string> tokens;

            std::stringstream check1(line);

            std::string intermediate;

            while(getline(check1, intermediate, delim))
            {
                tokens.push_back(intermediate);
            }
            return tokens;
        }


    public:
        Trader(std::string traderName) {
            std::cout << "Trader created";
            this->traderName = traderName;
        }

        void trade(std::string marketAction) {
            /**
                Basically this class assumes that the market contains different actions
                for bonds: sell, buy, sold* -> the action itself with who sold and who bought.
            */
            std::vector<std::string> tokens = splitString(
                marketAction, ' ');

        }

        float calculateRisk() {
            return 0.20;
        }
};
