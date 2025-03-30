import socket
import time

TRIGGER_MESSAGE = "Humidity = "
STATION_IP = '127.0.0.1'  # Đổi thành IP của trạm nếu khác
STATION_PORT = 1126  # Cổng trạm đang lắng nghe

def send_trigger():
    number = 1
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
       
        message = TRIGGER_MESSAGE + str(number)  # Chuyển number thành chuỗi
        sock.sendto(message.encode(), (STATION_IP, STATION_PORT))
        print(f"Sent: {message} to {STATION_IP}:{STATION_PORT}")
        number += 1
        time.sleep(1)  # Gửi dữ liệu cách nhau 1 giây (có thể thay đổi)

if __name__ == "__main__":
    send_trigger()
