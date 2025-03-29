"""
The socket connection to the market server.

"""

import socket

class Connection:
    def __init__(self, configuration):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except Exception as e:
            raise RuntimeError("Could not create socket") from e

        try:
            self.sock.connect((configuration.exchange_hostname,
                               configuration.exchange_port))
        except Exception as e:
            raise RuntimeError("Could not connect") from e

        # Make file-like objects for reading and writing with line buffering
        try:
            self.sock_in = self.sock.makefile('r', buffering=1, encoding='utf-8', newline='\n')
            self.sock_out = self.sock.makefile('w', buffering=1, encoding='utf-8', newline='\n')
        except Exception as e:
            raise RuntimeError("Could not open socket files") from e

    def send_to_exchange(self, input_str):
        # All messages must always be uppercase
        line = input_str.upper() + "\n"
        try:
            self.sock_out.write(line)
        except Exception as e:
            raise RuntimeError("Error sending to exchange") from e

    def read_from_exchange(self):
        line = self.sock_in.readline()
        if not line:
            raise RuntimeError("Reading line from socket")
        return line.rstrip('\n')
