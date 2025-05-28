

#include <stdio.h>

unsigned short my_ntohs(unsigned short value)
{
	return ( (value&0x00FF) << 8 | (value&0xFF00) >>8 );
}

unsigned int my_ntohl(unsigned int value)
{
	return ( (value&0x000000FF) << 24 |
			 (value&0x0000FF00) << 8  |
			 (value&0x00FF0000) >> 8  |
			 (value&0xFF000000) >> 24
	        );
}

int main()
{
	unsigned short a=0x1234; // 2bytes
	// op : 34 12
	unsigned int b=0x12345678; // 4bytes
	// op : 78 56 34 12

	printf("size of unsigned short : %ld bytes \n",sizeof(a));
	printf("size of unsigned int : %ld bytes\n",sizeof(b));

    printf("before conversion : \n");
	printf("%x \n",a);
    printf("%x \n",b);

    printf("after conversion : \n");
	printf("%x \n",my_ntohs(a));
    printf("%x \n",my_ntohl(b));


}

/*NAME
       htonl, htons, ntohl, ntohs - convert values between host and network byte order

SYNOPSIS
       #include <arpa/inet.h>



       uint16_t ntohs(uint16_t netshort);
       uint32_t ntohl(uint32_t netlong);


       uint16_t htons(uint16_t hostshort);
       uint32_t htonl(uint32_t hostlong);





     #define  SERVICE_PORT_UDP_PORT       ntohs(0xBAC0);       //47808
     wUdpPort = ntohs(wUdpPort);

    126 #define  SERVICE_PORT_IP             ntohl(0xA9FE0101)    //Service port IP 169.254.1.1
    127 #define  SERVICE_PORT_SUBNET_MAST    ntohl(0xFFFFFFF8)    //255.255.255.248
    128 #define  SERVICE_PORT_GATE_WAY       ntohl(0xA9FE0101)
    934 ulIP = ntohl(from.sin_addr.s_addr);
    976 ulIP = ntohl(from.sin_addr.s_addr);
*/
