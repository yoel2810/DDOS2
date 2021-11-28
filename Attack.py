import sys
from scapy.all import TCP, IP, send
import time


def writeToFile(path, content):
    f = open(path, "a");
    f.write(content);
    f.close();


ipHeader = IP(dst = '192.168.14.4')
tcpHeader = TCP(dport = 80, flags = 'S')
tot = 0
for i in range(100):
    for j in range(10000):
        index = i * 10000 + j
        print("packet number: " + str(index))
        start_send = time.perf_counter()
        send(ipHeader/tcpHeader)
        end_send = time.perf_counter() - start_send
        tot += end_send
        writeToFile("./syns_results_p.txt", str(index) + " " + str(end_send) + "\n")
        print("Time: " + str(end_send))
avg = tot / 1000000
writeToFile("./syns_results_p.txt", tot)
writeToFile("./syns_results_p.txt", avg)
