/* HOW TO RUN
   1) Configure things in the Configuration class
   2) Compile: g++ -o bot.exe bot.cpp
   3) Run in loop: while true; do ./bot.exe; sleep 1; done
*/

/* C includes for networking things */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/* C++ includes */
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct MarketEvent {
  std::vector<std::pair<std::string, std::string>> buyOrders;
  std::vector<std::pair<std::string, std::string>> sellOrders;
};

/* The Configuration class is used to tell the bot how to connect
   to the appropriate exchange. The `test_exchange_index` variable
   only changes the Configuration when `test_mode` is set to `true`.
*/
class Configuration {
private:
  /*
      0 = prod-like
      1 = slower
      2 = empty
  */
  static int const test_exchange_index = 1;

public:
  std::string team_name;
  std::string exchange_hostname;
  int exchange_port;
  /* replace REPLACEME with your team name! */
  Configuration(bool test_mode) : team_name("andrei probe #1") {
    exchange_port = 20000; /* Default text based port */
    if (true == test_mode) {
      exchange_hostname = "127.0.0.1";
      exchange_port += test_exchange_index;
    } else {
      exchange_hostname = "production";
    }
  }
};

/* Connection establishes a read/write connection to the exchange,
   and facilitates communication with it */
class Connection {
private:
  FILE *in;
  FILE *out;
  int socket_fd;

public:
  Connection(Configuration configuration) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      throw std::runtime_error("Could not create socket");
    }
    std::string hostname = configuration.exchange_hostname;
    hostent *record = gethostbyname(hostname.c_str());
    if (!record) {
      throw std::invalid_argument("Could not resolve host '" + hostname + "'");
    }
    in_addr *address = reinterpret_cast<in_addr *>(record->h_addr);
    std::string ip_address = inet_ntoa(*address);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip_address.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(configuration.exchange_port);

    int res = connect(sock, ((struct sockaddr *)&server), sizeof(server));
    if (res < 0) {
      throw std::runtime_error("could not connect");
    }
    FILE *exchange_in = fdopen(sock, "r");
    if (exchange_in == NULL) {
      throw std::runtime_error("could not open socket for writing");
    }
    FILE *exchange_out = fdopen(sock, "w");
    if (exchange_out == NULL) {
      throw std::runtime_error("could not open socket for reading");
    }

    setlinebuf(exchange_in);
    setlinebuf(exchange_out);
    this->in = exchange_in;
    this->out = exchange_out;
    this->socket_fd = res;
  }

  /** Send a string to the server */
  void send_to_exchange(std::string input) {
    std::string line(input);
    /* All messages must always be uppercase */
    std::transform(line.begin(), line.end(), line.begin(), ::toupper);
    int res = fprintf(this->out, "%s\n", line.c_str());
    if (res < 0) {
      throw std::runtime_error("error sending to exchange");
    }
  }

  /** Read a line from the server, dropping the newline at the end */
  std::string read_from_exchange() {
    /* We assume that no message from the exchange is longer
       than 10,000 chars */
    const size_t len = 10000;
    char buf[len];
    if (!fgets(buf, len, this->in)) {
      throw std::runtime_error("reading line from socket");
    }

    int read_length = strlen(buf);
    std::string result(buf);
    /* Chop off the newline */
    result.resize(result.length() - 1);
    return result;
  }
};

/** Join a vector of strings together, with a separator in-between
    each string. This is useful for space-separating things */
std::string join(std::string sep, std::vector<std::string> strs) {
  std::ostringstream stream;
  const int size = strs.size();
  for (int i = 0; i < size; ++i) {
    stream << strs[i];
    if (i != (strs.size() - 1)) {
      stream << sep;
    }
  }
  return stream.str();
}

std::vector<std::string> split_string(std::string line, char delim) {

  std::vector<std::string> tokens;

  std::stringstream check1(line);

  std::string intermediate;

  while (getline(check1, intermediate, delim)) {
    tokens.push_back(intermediate);
  }
  return tokens;
}

MarketEvent displayAndProcessMarketActivity(std::string marketEvent) {

  std::cout << "\n------------ Market Event ------------\n";

  std::istringstream iss(marketEvent);
  std::string token;

  // Read header parts
  iss >> token;
  std::cout << "Message Type: " << token << "\n";
  iss >> token;
  std::cout << "Instrument: " << token << "\n";

  // Read buy side tokens until SELL token appears
  std::cout << "\n--- BUY Orders ---\n";
  iss >> token; // Should be "BUY"
  std::cout << "Side: " << token << "\n";

  std::vector<std::pair<std::string, std::string>> buyOrders;
  while (iss >> token && token != "SELL") {
    auto pos = token.find(':');
    if (pos != std::string::npos) {
      std::string price = token.substr(0, pos);
      std::string quantity = token.substr(pos + 1);
      buyOrders.emplace_back(price, quantity);
    }
  }
  for (const auto &order : buyOrders) {
    std::cout << "Price: " << order.first << "   Quantity: " << order.second
              << "\n";
  }

  // 'token' == "SELL", now process sell orders
  std::cout << "\n--- SELL Orders ---\n";
  std::vector<std::pair<std::string, std::string>> sellOrders;
  // The "SELL" token is already read
  std::cout << "Side: SELL\n";
  while (iss >> token) {
    auto pos = token.find(':');
    if (pos != std::string::npos) {
      std::string price = token.substr(0, pos);
      std::string quantity = token.substr(pos + 1);
      sellOrders.emplace_back(price, quantity);
    }
  }
  for (const auto &order : sellOrders) {
    std::cout << "Price: " << order.first << "   Quantity: " << order.second
              << "\n";
  }
}

int main(int argc, char *argv[]) {
  // Setup
  float spendableAmount = 600; // 600 EUR
  float intraEventRiskMargin = 0.01; // 1%

  bool test_mode = true;
  Configuration config(test_mode);
  Connection conn(config);

  // Initial data for testing the connection
  std::vector<std::string> data;
  data.push_back(std::string("C++ Trading simulator"));
  data.push_back(config.team_name);
  conn.send_to_exchange(join(" ", data));

  while (1) {
    std::string marketEvent = conn.read_from_exchange();
    MarketEvent marketEventProcessed =
        displayAndProcessMarketActivity(marketEvent);

    // Build the portofolio first by buying some bonds


    // Start the selling part


    // Reconstruct the portofolio
  }

  return 0;
}
