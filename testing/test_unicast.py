import socket
import time

# Địa chỉ IP và cổng của thiết bị nhận
test_unicast = ("127.0.0.1", 1126)

# Tạo socket UDP
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    message_count = 0

    while True:
        # Gửi dữ liệu dạng string (có thể thay đổi thành bytes nếu cần)
        message = f"Message {message_count}"
        udp_socket.sendto(message.encode(), test_unicast)

        print(f"Sent: {message} to {test_unicast[0]}:{test_unicast[1]}")

        message_count += 1
        time.sleep(0.01)  # Gửi liên tục mỗi 10ms

except KeyboardInterrupt:
    print("\nTerminating script...")
finally:
    udp_socket.close()
