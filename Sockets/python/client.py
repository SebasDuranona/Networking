

import socket

# Define client constants
CLIENT_NAME = "Client of Sebastian Gomez-Duranona"
SERVER_HOST = 'localhost'
SERVER_PORT = 5001

def start_client():
    while True:
        try:
            client_number = int(input("Enter a number between 1 and 100: "))
            break

        except ValueError:
            print("Invalid input. Please enter a valid integer.")

    # Create a new socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        # Establish server connection
        client_socket.connect((SERVER_HOST, SERVER_PORT))
        print(f"Connected to server at {SERVER_HOST}:{SERVER_PORT}")

        # Prepare the message
        message = f"{CLIENT_NAME}|{client_number}"
        # Encode and send the message to the server
        client_socket.sendall(message.encode())
        print(f"Sent message: {message}")


        data = client_socket.recv(1024).decode() # Receive the server's response and decode using 1024 as the buffer size
        server_name, server_number = data.split('|')
        server_number = int(server_number)

        print(f"Client name: {CLIENT_NAME}")
        print(f"Server name: {server_name}")
        print(f"Client's number: {client_number}")
        print(f"Server's number: {server_number}")
        print(f"Sum: {client_number + server_number}")

    # Error handling
    except (ValueError, ConnectionRefusedError) as e:
        if isinstance(e, ValueError):
            print("There was an error processing the server response...")
        elif isinstance(e, ConnectionRefusedError):
            print("Connection Refused. Please make sure the server is running...")
        else:
            print(f"Oops, looks like an error occurred: {e}")
    finally:
        client_socket.close()
        print("Client socket closed")

if __name__ == "__main__":
    start_client()