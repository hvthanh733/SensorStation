import socket
import time

test_unicast = ("127.0.0.1", 1126)

udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    message_count = 0

    while True:
        message = f"Message {message_count}"
        udp_socket.sendto(message.encode(), test_unicast)

        print(f"Sent: {message} to {test_unicast[0]}:{test_unicast[1]}")

        message_count += 1
        time.sleep(0.01)  

except KeyboardInterrupt:
    print("\nTerminating script...")
finally:
    udp_socket.close()
