// raw_sock.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/ethernet.h>
#include<linux/if_arp.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>


int main() {
    // Structs that contain source IP addresses
    struct sockaddr_in source_socket_address, dest_socket_address;
    int packet_size;
    unsigned short iphdrlen;
    // Allocate string buffer to hold incoming packet data
    unsigned char *buffer = (unsigned char *)malloc(65536);
    // Open the raw socket
    int sock = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock == -1)
    {
        //socket creation failed, may be because of non-root privileges
        perror("Failed to create socket");
        exit(1);
    }
    while(1) {
      // recvfrom is used to read data from a socket
      packet_size = recvfrom(sock , buffer , 65536 , 0 , NULL, NULL);
      if (packet_size == -1) {
        printf("Failed to get packets\n");
        return 1;
      }
	struct ethhdr *eth = (struct ethhdr *)(buffer);
	printf("\nEthernet Header\n");
	printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
	printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
	printf("\t|-Protocol : %d\n\n",eth->h_proto);



      struct iphdr *ip_packet = (struct iphdr *)(buffer + sizeof(struct ethhdr));

      memset(&source_socket_address, 0, sizeof(source_socket_address));
      source_socket_address.sin_addr.s_addr = ip_packet->saddr;
      memset(&dest_socket_address, 0, sizeof(dest_socket_address));
      dest_socket_address.sin_addr.s_addr = ip_packet->daddr;

	printf("IP Packet: \n");
	printf("\t|-Packet Size (bytes): %d\n",ntohs(ip_packet->tot_len));
	printf("\t|-Source Address: %s\n", (char *)inet_ntoa(source_socket_address.sin_addr));
	printf("\t|-Destination Address: %s\n", (char *)inet_ntoa(dest_socket_address.sin_addr));
	printf("\t|-Identification: %d\n", ntohs(ip_packet->id));
	printf("\t|-Version: %d \n",(unsigned int)ip_packet->version);
	printf("\t|-Internet header Length: %d DWORDS or %d Bytes \n",(unsigned int)ip_packet->ihl,((unsigned int)(ip_packet->ihl))*4);
	printf("\t|-type of service: %d \n", (unsigned int)ip_packet->tos);
	printf("\t|-Time to Live: %d \n",(unsigned int)ip_packet->ttl);
	printf("\t|-Protocol: %d \n\n",(unsigned int)ip_packet->protocol);
	//printf("Header Checksum: %s \n", (char *)ntohs(ip_packet->check));

	/* getting actual size of IP header*/
	iphdrlen = ip_packet->ihl*4;
	/* getting pointer to udp header*/
	struct udphdr *udp=(struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	printf("UDP header\n");
	printf("\t|-Source Port : %d\n" , ntohs(udp->source));
	printf("\t|-Destination Port : %d\n", ntohs(udp->dest));
	printf("\t|-UDP Length : %d\n" , ntohs(udp->len));
	printf("\t|-UDP Checksum : %d\n\n" , ntohs(udp->check));

	/* getting pointer to tcp header*/
	struct tcphdr *tcp=(struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	printf("TCP header\n");
	printf("\t|-Source Port : %d\n", ntohs(tcp->source));
	printf("\t|-Destination Port : %d\n" , ntohs(tcp->dest));
	printf("\t|-TCP Length : %d\n" , ntohs(tcp->doff));
	printf("\t|-TCP Checksum : %d\n" , ntohs(tcp->check));
	printf("\t|-Sequence Number  : %u\n",ntohl(tcp->seq));
	printf("\t|-Acknowledge Number : %u\n",ntohl(tcp->ack_seq));
	printf("\t|-Urgent Flag  : %d\n",(unsigned int)tcp->urg);
	printf("\t|-Acknowledgement Flag : %d\n",(unsigned int)tcp->ack);
	printf("\t|-Push Flag  : %d\n",(unsigned int)tcp->psh);
	printf("\t|-Reset Flag   : %d\n",(unsigned int)tcp->rst);
	printf("\t|-Synchronise Flag  : %d\n",(unsigned int)tcp->syn);
	printf("\t|-Finish Flag   : %d\n",(unsigned int)tcp->fin);
	printf("\t|-Window   : %d\n",ntohs(tcp->window));
	printf("\t|-Urgent Pointer : %d\n\n",tcp->urg_ptr);





	unsigned char *data = (buffer + iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
	int i , buflen;
	struct sockaddr saddr;
	int saddr_len = sizeof (saddr);
	buflen=recvfrom(sock,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
	int remaining_data = buflen - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
 	printf("Data :%x\n",data);
	for(i=0;i<remaining_data;i++)
	{
	if(i!=0 && i%16==0)
	printf("\n");
	printf(" %02X ",data[i]);
	}






}

    return 0;
}

