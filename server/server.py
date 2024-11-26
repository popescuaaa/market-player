import socket
import time
import threading
from market import Market

HOST = '127.0.0.1'  # Localhost
PORT = 20001        # Match the port in the C++ Configuration

def run_server():
    # Initialize the market
    market = Market()

    # Start the market simulation in a separate thread
    simulation_thread = threading.Thread(target=market.simulate_market_activity)
    simulation_thread.start()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"Server running on {HOST}:{PORT}")

        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")

            # Respond to initial HELLO
            data = conn.recv(1024).decode('utf-8').strip()
            if data.startswith("HELLO"):
                print(f"Received: {data}")
                conn.sendall(b"HELLO from the server\n")

            # Continuously handle commands or send market data
            try:
                while True:
                    # Receive client commands
                    conn.settimeout(0.5)  # Avoid blocking too long
                    try:
                        client_data = conn.recv(1024).decode('utf-8').strip()
                        if client_data:
                            print(f"Received command: {client_data}")
                            tokens = client_data.split()
                            if tokens[0] in ["BUY", "SELL"] and len(tokens) == 3:
                                market.handle_client_order(tokens[0], tokens[1], tokens[2])
                            else:
                                print(f"Unknown or invalid command: {client_data}")
                    except socket.timeout:
                        pass

                    # Send updated order book to client
                    book_msg = market.get_book_message()
                    print(f"Sending: {book_msg.strip()}")
                    conn.sendall(book_msg.encode('utf-8'))
                    time.sleep(1)  # Adjust frequency as needed
            except BrokenPipeError:
                print("Connection closed by client.")
            except Exception as e:
                print(f"Error: {e}")
            finally:
                market.stop_simulation()
                simulation_thread.join()
                market.save_profit_to_file()

if __name__ == "__main__":
    run_server()
