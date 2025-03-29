class Configuration:
    test_exchange_index = 1

    def __init__(self, test_mode):
        self.team_name = "andrei probe #1"
        self.exchange_port = 20000  # Default text based port
        if test_mode:
            self.exchange_hostname = "127.0.0.1"
            self.exchange_port += self.test_exchange_index
        else:
            self.exchange_hostname = "production"
