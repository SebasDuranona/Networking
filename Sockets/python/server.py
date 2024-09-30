
import socket

# Define Server constants
SERVER_NAME = "Server of Sebastian Gomez-Duranona"
SERVER_PORT = 5001
SERVER_NUMBER = 50

def start_server():

    # Create new socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('localhost', SERVER_PORT))
    server_socket.listen()
    print(f"Server started. Listening on port {SERVER_PORT}")

    try:
        while True: # Continuous listening loop ensures server persistence
            client_socket, addr = server_socket.accept() # Accept a new client connection
            print(f"Connected by {addr}")
            
            try:
                data = client_socket.recv(1024).decode() # Receive data fromt the client and decode using 1024 as the buffer size
                client_name, client_number = data.split('|')
                client_number = int(client_number)

                print(f"Received message from {client_name}")
                print(f"Server name: {SERVER_NAME}")
                print(f"Client's number: {client_number}")
                print(f"Server's number: {SERVER_NUMBER}")

                if 1 <= client_number <= 100:
                    total_sum = client_number + SERVER_NUMBER
                    print(f"Sum: {total_sum}")
                    response = f"{SERVER_NAME}|{SERVER_NUMBER}"
                    client_socket.sendall(response.encode())
                else:
                    print("Client number out of range. Terminating server.")
                    break
            finally:
                client_socket.close()
                print("Client socket closed")
    finally:
        server_socket.close()
        print("Server socket closed")

if __name__ == "__main__":
    start_server()