
#include "net-checksum.h"


 /* Compute checksum for count bytes starting at addr, using one's complement of one's complement sum*/
unsigned short compute_checksum(unsigned short *addr, unsigned int count) 
{
	register unsigned long sum = 0;
	while (count > 1)
	{
		sum += *addr++;
		count -= 2;
	}

	//if any bytes left, pad the bytes and add
	if (count > 0)
	{
		sum += ((*addr)&htons(0xFF00));
	}

	//Fold sum to 16 bits: add carrier to result
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}

	//one's complement
	sum = ~sum;

	return ((unsigned short)sum);
}


unsigned short compute_ip_checksum(struct IP_HEAD* iphdrp)
{
	unsigned short crc = 0;
	iphdrp->m_sCheckSum = 0;
	crc = compute_checksum((unsigned short*)iphdrp, iphdrp->m_cHeaderLen << 2);
	//iphdrp->m_sCheckSum = crc;
	return crc;
}

/* set tcp checksum: given IP header and UDP datagram */
unsigned short compute_udp_checksum(struct IP_HEAD *pIph, unsigned short *ipPayload)
{
	register unsigned long sum = 0;
	struct UdpHeader *udphdrp = (struct UdpHeader*)(ipPayload);
	unsigned short udpLen = htons(udphdrp->len);

	//add the pseudo header 
	
	//the source ip
	sum += (pIph->m_uiSourIp >> 16) & 0xFFFF;
	sum += (pIph->m_uiSourIp) & 0xFFFF;

	//the dest ip
	sum += (pIph->m_uiDestIp >> 16) & 0xFFFF;
	sum += (pIph->m_uiDestIp) & 0xFFFF;

	//protocol and reserved: 17
	sum += htons(IPPROTO_UDP);

	//the length
	sum += udphdrp->len;

	//add the IP payload
	//initialize checksum to 0

	udphdrp->crc = 0;
	while (udpLen > 1)
	{
		sum += *ipPayload++;
		udpLen -= 2;
	}

	//if any bytes left, pad the bytes and add
	if (udpLen > 0)
	{
		//printf("+++++++++++++++padding: %dn", udpLen);
		sum += ((*ipPayload)&htons(0xFF00));
	}

	//Fold sum to 16 bits: add carrier to result
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}

	sum = ~sum;

	//set computation result
	unsigned short udpcrc = ((unsigned short)sum == 0x0000) ? 0xFFFF : (unsigned short)sum;
	udphdrp->crc = udpcrc;
	return udpcrc;
}


/* set tcp checksum: given IP header and tcp segment */
unsigned short compute_tcp_checksum(struct IP_HEAD *pIph, unsigned short *ipPayload)
{
	register unsigned long sum = 0;
	unsigned short tcpLen = ntohs(pIph->m_sTotalLenOfPacket) - (pIph->m_cHeaderLen << 2);
	struct TcpHeader *tcphdrp = (struct TcpHeader*)(ipPayload);

	//add the pseudo header 

	//the source ip
	sum += (pIph->m_uiSourIp >> 16) & 0xFFFF;
	sum += (pIph->m_uiSourIp) & 0xFFFF;

	//the dest ip
	sum += (pIph->m_uiDestIp >> 16) & 0xFFFF;
	sum += (pIph->m_uiDestIp) & 0xFFFF;
	//protocol and reserved: 6
	sum += htons(IPPROTO_TCP);
	//the length
	sum += htons(tcpLen);

	//add the IP payload
	//initialize checksum to 0
	tcphdrp->crc = 0;

	while (tcpLen > 1)
	 {
		sum += *ipPayload++;
		tcpLen -= 2;
	}

	//if any bytes left, pad the bytes and add
	if (tcpLen > 0)
	 {
		sum += ((*ipPayload)&htons(0xFF00));
	}

	//Fold 32-bit sum to 16 bits: add carrier to result
	while (sum >> 16) 
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	sum = ~sum;

	//set computation result
	tcphdrp->crc = sum;

	return sum;
}



std::string getIpPacketHeadDesc(IP_HEAD *phead)
{
	if (NULL == phead)
	{
		return "";
	}
	char message[2048] = { 0 };

	char src_ipstr[20] = { 0 },
		dst_ipstr[20] = { 0 };
	inet_ntop(AF_INET,
		&phead->m_uiSourIp,
		src_ipstr,
		sizeof(src_ipstr));


	inet_ntop(AF_INET,
		&phead->m_uiDestIp,
		dst_ipstr,
		sizeof(dst_ipstr));

	sprintf(message,
		"Version: 0x%X | " \
		"Service Type: 0x%X | " \
		"Header Length: %d | " \
		"TotalLenOfPacket: %d | " \
		"PacketID: %d | " \
		"Sliceinfo: 0x%X | " \
		"TTL: 0x%X | " \
		"TypeOfProtocol: 0x%x | " \
		"CheckSum: %d | " \
		"Source IP: %s | " \
		"Dest IP: %s |",
		phead->m_cVersion,
		phead->m_cTypeOfService,
		phead->m_cHeaderLen,
		ntohs(phead->m_sTotalLenOfPacket),
		ntohs(phead->m_sPacketID),
		phead->m_sSliceinfo,
		phead->m_cTTL,
		phead->m_cTypeOfProtocol,
		phead->m_sCheckSum,
		src_ipstr,
		dst_ipstr);

	return message;
}


std::string getUdpPacketHeadDesc(UdpHeader *udp)
{
	if (NULL == udp)
	{
		return "";
	}
	char message[2048] = { 0 };

	sprintf(message,
		"Source Port: %d | " \
		"Dest Port: %d | " \
		"Packet Length: %d | " \
		"CheckSum: %d | ",
		ntohs(udp->sport),
		ntohs(udp->dport),
		ntohs(udp->len),
		udp->crc);

	return message;
}