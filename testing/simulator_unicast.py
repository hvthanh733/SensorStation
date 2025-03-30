import socket
import time

TRIGGER_MESSAGE = "Humidity = "
STATION_IP = '127.0.0.1' 
STATION_PORT = 1126

def send_trigger():
    number = 1
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
       
        message = TRIGGER_MESSAGE + str(number)  
        sock.sendto(message.encode(), (STATION_IP, STATION_PORT))
        print(f"Sent: {message} to {STATION_IP}:{STATION_PORT}")
        number += 1
        time.sleep(1)

if __name__ == "__main__":
    send_trigger()
