import socket
import time
import random

HOST = '127.0.0.1'  # Localhost
PORT = 20001        # Match the port in the C++ Configuration

def generate_book_message():
    """Generate a random BOOK message for the symbol BOND."""
    num_buy_orders = random.randint(1, 5)
    num_sell_orders = random.randint(1, 5)
    
    buy_orders = " ".join(
        f"{random.randint(90, 110)}:{random.randint(1, 20)}"
        for _ in range(num_buy_orders)
    )
    sell_orders = " ".join(
        f"{random.randint(111, 130)}:{random.randint(1, 20)}"
        for _ in range(num_sell_orders)
    )
    
    return f"BOOK BOND BUY {buy_orders} SELL {sell_orders}\n"

def run_server():
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
                conn.sendall(b"HELLO Andrei...the server is running\n")
            
            # Continuously send data
            try:
                while True:
                    book_msg = generate_book_message()
                    print(f"Sending: {book_msg.strip()}")
                    conn.sendall(book_msg.encode('utf-8'))
                    time.sleep(1)  # Adjust frequency as needed
            except BrokenPipeError:
                print("Connection closed by client.")
            except Exception as e:
                print(f"Error: {e}")

if __name__ == "__main__":
    run_server()
