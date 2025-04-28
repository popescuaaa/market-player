from collections import deque
import random
import time

class Market:
    def __init__(self):
        # Order book: price levels and quantities
        self.buy_orders = deque([(100, 20), (99, 15), (98, 10)])  # [(price, quantity), ...]
        self.sell_orders = deque([(101, 10), (102, 15), (103, 20)])
        self.profit = 0  # Profit from completed trades
        self.running = True  # Control the simulation loop

    def match_orders(self):
        """Match overlapping buy and sell orders."""
        while self.buy_orders and self.sell_orders:
            buy_price, buy_qty = self.buy_orders[0]
            sell_price, sell_qty = self.sell_orders[0]

            if buy_price >= sell_price:  # Match condition
                traded_qty = min(buy_qty, sell_qty)
                trade_revenue = traded_qty * sell_price
                trade_cost = traded_qty * buy_price

                # Update profit
                self.profit += (trade_revenue - trade_cost)

                print(f"Trade executed: {traded_qty} units @ {sell_price}")

                # Adjust quantities
                if buy_qty > traded_qty:
                    self.buy_orders[0] = (buy_price, buy_qty - traded_qty)
                else:
                    self.buy_orders.popleft()

                if sell_qty > traded_qty:
                    self.sell_orders[0] = (sell_price, sell_qty - traded_qty)
                else:
                    self.sell_orders.popleft()
            else:
                break  # No more matches possible

    def handle_client_order(self, action, quantity, price):
        """Process a client's BUY or SELL command."""
        quantity = int(quantity)
        price = int(price)

        if action == "BUY":
            print(f"Client placed BUY order for {quantity} units @ {price}")
            self.buy_orders.appendleft((price, quantity))  # Higher priority for new buy
        elif action == "SELL":
            print(f"Client placed SELL order for {quantity} units @ {price}")
            self.sell_orders.append((price, quantity))  # Lower priority for new sell

        # Match orders after new client command
        self.match_orders()

    def add_random_orders(self):
        """Simulate random orders to mimic market activity."""
        action = random.choice(["BUY", "SELL"])
        quantity = random.randint(1, 20)
        base_price = 100  # Central price for fluctuations
        price = base_price + random.randint(-10, 10)

        print(f"Market simulation added {action} order: {quantity} units @ {price}")
        if action == "BUY":
            self.buy_orders.appendleft((price, quantity))
        elif action == "SELL":
            self.sell_orders.append((price, quantity))

        # Match orders after adding random order
        self.match_orders()

    def simulate_market_activity(self):
        """Continuously simulate market activity by adding random orders."""
        while self.running:
            time.sleep(random.uniform(0.5, 2))  # Random delay between orders
            self.add_random_orders()

    def stop_simulation(self):
        """Stop the simulation loop."""
        self.running = False

    def get_book_message(self):
        """Format the current order book as a BOOK message."""
        buy_side = " ".join(f"{price}:{qty}" for price, qty in self.buy_orders)
        sell_side = " ".join(f"{price}:{qty}" for price, qty in self.sell_orders)
        return f"BOOK BOND BUY {buy_side} SELL {sell_side}\n"

    def save_profit_to_file(self):
        """Save the total profit/loss to a file."""
        with open("market_profit.txt", "w") as file:
            file.write(f"Total profit: {self.profit}\n")
        print("Profit saved to market_profit.txt")
