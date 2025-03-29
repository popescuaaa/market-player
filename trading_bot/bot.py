from configuration import Configuration
from connection import Connection

def join(sep, strs):
    return sep.join(strs)

def split_string(line, delim):
    return line.split(delim)


def print_order(order):
    # order is a list with [operation, price, quantity]
    operation, price, quantity = order
    print(f"{operation}: {quantity} {price}")


def main():
    test_mode = True
    config = Configuration(test_mode)
    conn = Connection(config)

    # Initial data for testing the connection
    data = ["PYTHON Trading simulator", config.team_name]
    conn.send_to_exchange(join(" ", data))

    while True:
        line = conn.read_from_exchange()
        print("The exchange replied:", line)
        tokens = split_string(line, ' ')

        if len(tokens) >= 2 and tokens[0] == "BOOK" and tokens[1] == "BOND":
            print("Transaction....")
            buy = []
            sell = []

            operation_pos = 3
            operation = tokens[operation_pos]

            # BUY zone
            while operation != "SELL":
                buy_offer = split_string(operation, ':')
                price = int(buy_offer[0])
                quantity = int(buy_offer[1])
                buy.append((price, quantity))
                operation_pos += 1
                operation = tokens[operation_pos]

            operation_pos += 1  # Skip "SELL"

            for token in tokens[operation_pos:]:
                sell_offer = split_string(token, ':')
                price = int(sell_offer[0])
                quantity = int(sell_offer[1])
                sell.append((price, quantity))

            for p in buy:
                print("Someone wants to buy:", p[0], "@", p[1])
                price, quantity = p
                sell_order = ["SELL", str(quantity), str(price)]
                conn.send_to_exchange(join(" ", sell_order))

            for p in sell:
                print("Someone wants to sell:", p[1], "at", p[0])
                price, quantity = p
                buy_order = ["BUY", str(quantity), str(price)]
                conn.send_to_exchange(join(" ", buy_order))


if __name__ == "__main__":
    main()
