


#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__




#define  LWIP_TCP_KEEPALIVE       1             //激活keepalive
#define  TCP_KEEPIDLE_DEFAULT     2000UL        //2秒内连接双方都无数据，则发起保活探测
#define  TCP_KEEPINTVL_DEFAULT    1000UL        //1秒发送一次保活探测
#define  TCP_KEEPCNT_DEFAULT      3UL           //3次保活探测无数据则进入错误回调函数


/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 * 系统相关
 */
#define SYS_LIGHTWEIGHT_PROT    1

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 * 使用系统
 */
#define NO_SYS                  0

/**
 * NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
 * Mainly for compatibility to old versions.
 * 无操作系统时使用
 */
#define NO_SYS_NO_TIMERS        0

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. 
   内存对齐*/
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. 
发送堆内存大小，如果发送很多数据设大*/
#define MEM_SIZE                (10*1024)//暂定5K

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. 
   从静态存储区发送较多数据时应设置较大*/
#define MEMP_NUM_PBUF           10
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". 
   UDP协议相关，可否为0？*/
#define MEMP_NUM_UDP_PCB        4
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections.
   TCP协议控制块数量*/
#define MEMP_NUM_TCP_PCB        4
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. 
   侦听TCP协议控制块数量*/
#define MEMP_NUM_TCP_PCB_LISTEN 2
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. 
   同时排队的tcp数量*/
#define MEMP_NUM_TCP_SEG        120
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. 
   模拟超时*/
#define MEMP_NUM_SYS_TIMEOUT    6


/* ---------- Pbuf options PBUF选项---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. 
pbuf缓冲区数*/
#define PBUF_POOL_SIZE          20
/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE       LWIP_MEM_ALIGN_SIZE(TCP_MSS+40+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)


/* ---------- TCP options 使用tcp---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. 
   内存小设为0*/
#define TCP_QUEUE_OOSEQ         0

/* TCP Maximum segment size. tcp段大小*/
#define TCP_MSS                 (1500 - 40)   /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes).发送缓冲区*/
#define TCP_SND_BUF             (11*TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. 
tcp发送缓冲空间(2 * TCP_SND_BUF/TCP_MSS)*/

#define TCP_SND_QUEUELEN        (8* TCP_SND_BUF/TCP_MSS)

/* TCP receive window. 
tcp接收窗口*/
#define TCP_WND                 16000//16000 or (2*TCP_MSS)


/* ---------- ICMP options arp相关---------- */
#define LWIP_ICMP                       1


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               0


/* ---------- UDP options 不使用udp---------- */
#define LWIP_UDP                0
#define UDP_TTL                 255


/* ---------- Statistics options 静态---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1

/* ---------- link callback options 链接中断回调---------- */
/* LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
#define LWIP_NETIF_LINK_CALLBACK        0
/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/


//#define CHECKSUM_BY_HARDWARE

//软件校验，速度快
#ifdef CHECKSUM_BY_HARDWARE
/* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
#define CHECKSUM_GEN_IP                 0
/* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP                0
/* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP                0
/* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
#define CHECKSUM_CHECK_IP               0
/* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP              0
/* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP              0
/* CHECKSUM_CHECK_ICMP==0: Check checksums by hardware for incoming ICMP packets.*/
#define CHECKSUM_GEN_ICMP               0
#else
/* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
#define CHECKSUM_GEN_IP                 1
/* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP                1
/* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP                1
/* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
#define CHECKSUM_CHECK_IP               1
/* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP              1
/* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP              1
/* CHECKSUM_CHECK_ICMP==1: Check checksums by hardware for incoming ICMP packets.*/
#define CHECKSUM_GEN_ICMP               1
#endif


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    1

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     0//使用netconn编程

/*
   ---------------------------------
   ---------- OS options lwip线程选项----------
   ---------------------------------
*/
//????

#define DEFAULT_UDP_RECVMBOX_SIZE       10
#define DEFAULT_TCP_RECVMBOX_SIZE       10
#define DEFAULT_ACCEPTMBOX_SIZE         10
#define DEFAULT_THREAD_STACKSIZE        1024


#define TCPIP_THREAD_NAME              "lwip"
#define TCPIP_THREAD_STACKSIZE          2048
#define TCPIP_MBOX_SIZE                 8
#define TCPIP_THREAD_PRIO               10

//#define IPERF_SERVER_THREAD_NAME            "iperf_server"
//#define IPERF_SERVER_THREAD_STACKSIZE        1024
//#define IPERF_SERVER_THREAD_PRIO             0

//#define BLOCK_TIME                            250
//#define BLOCK_TIME_WAITING_FOR_INPUT  ( ( portTickType ) 100 )

/*
   ----------------------------------------
   ---------- Lwip Debug options ----------
   ----------------------------------------
*/
#define LWIP_DEBUG                      0//暂不开启debug

#endif /* __LWIPOPTS_H__ */



/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
//#ifndef __LWIPOPTS_H__
//#define __LWIPOPTS_H__

//#define TCPIP_MBOX_SIZE                6

//#define TCPIP_THREAD_NAME               "tcp/ip"
//#define TCPIP_THREAD_STACKSIZE          1500
//#define TCPIP_THREAD_PRIO               3

//#define DEFAULT_THREAD_STACKSIZE        1500
//#define DEFAULT_THREAD_PRIO             3

//#define ETH_PAD_SIZE                                      2
//#define LWIP_TCPIP_CORE_LOCKING         0 /// Initialize mutex

////#define LWIP_DEBUG
////#define DBG_TYPES_ON                    0xff

//#define ETHARP_DEBUG                    LWIP_DBG_OFF
//#define NETIF_DEBUG                     LWIP_DBG_ON
//#define PBUF_DEBUG                      LWIP_DBG_OFF
//#define API_LIB_DEBUG                   LWIP_DBG_OFF
//#define API_MSG_DEBUG                   LWIP_DBG_OFF
//#define SOCKETS_DEBUG                   LWIP_DBG_OFF
//#define ICMP_DEBUG                      LWIP_DBG_OFF
//#define IGMP_DEBUG                      LWIP_DBG_OFF
//#define INET_DEBUG                      LWIP_DBG_OFF
//#define IP_DEBUG                        LWIP_DBG_OFF
//#define IP_REASS_DEBUG                  LWIP_DBG_OFF
//#define RAW_DEBUG                       LWIP_DBG_OFF
//#define MEM_DEBUG                       LWIP_DBG_OFF
//#define MEMP_DEBUG                      LWIP_DBG_OFF
//#define SYS_DEBUG                       LWIP_DBG_OFF
//#define TCP_DEBUG                       LWIP_DBG_OFF
//#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
//#define TCP_FR_DEBUG                    LWIP_DBG_OFF
//#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
//#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
//#define TCP_WND_DEBUG                   LWIP_DBG_OFF
//#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
//#define TCP_RST_DEBUG                   LWIP_DBG_OFF
//#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
//#define UDP_DEBUG                       LWIP_DBG_OFF
//#define TCPIP_DEBUG                     LWIP_DBG_OFF
//#define PPP_DEBUG                       LWIP_DBG_OFF
//#define SLIP_DEBUG                      LWIP_DBG_OFF
//#define DHCP_DEBUG                      LWIP_DBG_OFF
//#define AUTOIP_DEBUG                    LWIP_DBG_OFF
//#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
//#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
//#define DNS_DEBUG                       LWIP_DBG_OFF
//#define LWIP_NOASSERT                   0

///**
// * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
// * critical regions during buffer allocation, deallocation and memory
// * allocation and deallocation.
// */
//#define SYS_LIGHTWEIGHT_PROT            1

///*
//   ------------------------------------
//   ---------- Memory options ----------
//   ------------------------------------
//*/

///**
// * MEM_ALIGNMENT: should be set to the alignment of the CPU
// *    4 byte alignment -> #define MEM_ALIGNMENT 4
// *    2 byte alignment -> #define MEM_ALIGNMENT 2
// */
//#define MEM_ALIGNMENT                   4

///** STF (9*1024)
// * MEM_SIZE: the size of the heap memory. If the application will send
// * a lot of data that needs to be copied, this should be set high.
// */
//#define MEM_SIZE                        (8*1024)

///*
//   ------------------------------------------------
//   ---------- Internal Memory Pool Sizes ----------
//   ------------------------------------------------
//*/
///** STF 40
// * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
// * If the application sends a lot of data out of ROM (or other static memory),
// * this should be set high.
// */
//#define MEMP_NUM_PBUF                   20

///** STF 8
// * MEMP_NUM_TCP_PCB: the number of simulatenously active TCP connections.
// * (requires the LWIP_TCP option)
// */
//#define MEMP_NUM_TCP_PCB                10

///**
// * MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments.
// * (requires the LWIP_TCP option)
// */
//#define MEMP_NUM_TCP_SEG                8

///** STF 3
// * MEMP_NUM_SYS_TIMEOUT: the number of simulateously active timeouts.
// * (requires NO_SYS==0)
// */
//#define MEMP_NUM_SYS_TIMEOUT            5

///**
// * MEMP_NUM_NETBUF: the number of struct netbufs.
// * (only needed if you use the sequential API, like api_lib.c)
// */
//#define MEMP_NUM_NETBUF                 4

///** STF 8
// * PBUF_POOL_SIZE: the number of buffers in the pbuf pool.
// */
//#define PBUF_POOL_SIZE                  4


///*
//   ----------------------------------
//   ---------- Pbuf options ----------
//   ----------------------------------
//*/

///**
// * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
// * designed to accomodate single full size TCP frame in one pbuf, including
// * TCP_MSS, IP header, and link header.
// */
//#define PBUF_POOL_BUFSIZE               1500

///*
// ---------------------------------
// ---------- TCP options ----------
// ---------------------------------
//*/
///**
// * LWIP_TCP==1: Turn on TCP.
// */
//#define LWIP_TCP                        1

///* TCP Maximum segment size. */
//#define TCP_MSS                         1500

///* TCP sender buffer space (bytes). */
//#define TCP_SND_BUF                     2*TCP_MSS

///**
// * TCP_WND: The size of a TCP window.
// */
//#define TCP_WND                         2*TCP_MSS //1500

///**
// * TCP_SYNMAXRTX: Maximum number of retransmissions of SYN segments.
// */
//#define TCP_SYNMAXRTX                   4

///*
//   ---------------------------------
//   ---------- RAW options ----------
//   ---------------------------------
//*/
///**
// * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
// */
//#define LWIP_RAW                        0

///*
//   ----------------------------------------------
//   ---------- Sequential layer options ----------
//   ----------------------------------------------
//*/
///**
// * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
// */
//#define LWIP_NETCONN                    1
///*
//   ------------------------------------
//   ---------- Socket options ----------
//   ------------------------------------
//*/
///**
// * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
// */
//#define LWIP_SOCKET                     1


///*
//   ----------------------------------------
//   ---------- Statistics options ----------
//   ----------------------------------------
//*/
///**
// * LWIP_STATS==1: Enable statistics collection in lwip_stats.
// */
//#define LWIP_STATS                      0

///*
//   ----------------------------------
//   ---------- DHCP options ----------
//   ----------------------------------
//*/
///**
// * LWIP_DHCP==1: Enable DHCP module.
// */
//#define LWIP_DHCP                       0

///*
//   ----------------------------------
//   ---------- IGMP options ----------
//   ----------------------------------
//*/
///**
// * LWIP_IGMP==1: Turn on IGMP module.
// */
//#define LWIP_IGMP                       0


//#define LWIP_PROVIDE_ERRNO                1

////#define LWIP_SNMP 1

//#endif /* __LWIPOPTS_H__ */
