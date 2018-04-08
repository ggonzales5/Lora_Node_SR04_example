from socket import *
import time
import sys

address = ( '192.168.1.118', 5000)
client_socket = socket(AF_INET, SOCK_DGRAM)
client_socket.settimeout(1)

#Specify the number of points using sysarg
points = int(sys.argv[2])

print "Gathering Data..."
for point in range(points):

    command = sys.argv[1]                   #Set coomand using sysarg
    client_socket.sendto(command, address) #send command to node
    try:
        rec_data, addr = client_socket.recvfrom(2048) #Read response from teh node
        print rec_data #Print the response from the node
        if rec_data == "ERROR: invalid request":
            break
    except:
        pass

