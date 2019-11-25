#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "ippacket.h"


#pragma pack(1)

struct IP_HEAD
{
	unsigned char m_cHeaderLen : 4,m_cVersion : 4;  //共8位，前4位是版本，后4为是IP包头长度（占几个32位）
	unsigned char m_cTypeOfService;  //服务类型
	unsigned short m_sTotalLenOfPacket; //整个IP包长度
	unsigned short m_sPacketID; //包标识
	unsigned short m_sSliceinfo; //分片是否可用
	unsigned char m_cTTL; //生存时间
	unsigned char m_cTypeOfProtocol; //所携带协议 0x11是UDP  0x06是TCP
	unsigned short m_sCheckSum; //校验和
	unsigned int m_uiSourIp; //源IP
	unsigned int m_uiDestIp;  //目的ip
};


/* UDP header*/
struct UdpHeader
{
	unsigned short sport; 			// Source port
	unsigned short dport; 			// Destination port
	unsigned short len; 			// Datagram length
	unsigned short crc; 			// Checksum
};


/* TCP header*/
struct TcpHeader
{
	unsigned short srcPort; 				//	Source port
	unsigned short dstPort; 				//  Destination port
	unsigned int	 sequenceNum; 		//	Sequence Number
	unsigned int	 acknowledgment; 	//  Acknowledgment
	unsigned char	 headerLen; 			//  header len
	unsigned char  flags; 				//  flags
	unsigned short windowSize; 		//
	unsigned short crc;
	unsigned short urg_ptr;
};

std::string getIpPacketHeadDesc(IP_HEAD *phead);
std::string getUdpPacketHeadDesc(UdpHeader *udp);

typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

unsigned short compute_tcp_checksum(struct IP_HEAD *pIph, unsigned short *ipPayload);
/* set tcp checksum: given IP header and UDP datagram */
unsigned short compute_udp_checksum(struct IP_HEAD *pIph, unsigned short *ipPayload);
unsigned short compute_ip_checksum(struct IP_HEAD* iphdrp);






