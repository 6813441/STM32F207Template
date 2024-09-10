/*
 * Copyright (c) 2023,杭可仪器第一研究所
 * All rights reserved.
 *
 * 文件名称：
 * 文件标识：见配置管理计划书
 * 摘 要：
 *
 * 当前版本：1.1
 * 作 者：
 * 完成日期：2023年4月8日
 *
 * 取代版本：1.0
 * 原作者 ：
 * 完成日期：2023年4月8日
 */
#include <lwip/opt.h>
#include <lwip/arch.h>
#include "tcpip.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "netif/ethernet.h"
#include "lwip/def.h"
#include "lwip/stats.h"
#include "lwip/etharp.h"
#include "lwip/ip.h"
#include "lwip/snmp.h"
#include "lwip/timeouts.h"
// #include "eth_bsp_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "api.h"

#include "user_tcp_server.h"
#include "tcp.h"

#if LWIP_SOCKET
#include <lwip/sockets.h>
#define RECV_DATA         (1024)
#endif 

struct netconn *remote_netconn; // 用于保存远端信息

// 是否应该建一个结构体使用指针连续保存？

extern ip4_addr_t ipaddr;
//struct netconn *user_netconn;   // 创建一个netconn连接的结构体指针

#if LWIP_SOCKET
/******************************************************************
函数名称：
函数功能:建立以太网线程
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
TaskHandle_t TCP_ServerSocket_handle;
void TCP_ServerSocket_Init(void)
{
  // sys_thread_new("TCP_Server_Thread",TCP_Server_Thread,NULL,1024,5);//优先级5
  xTaskCreate(TCP_ServerSocket_Thread,
              "TCP_Listen_Thread",
              1024,
              NULL,
              6,
              &TCP_ServerSocket_handle);
}

void TCP_ServerSocket_Thread (void *p)
{
  int sock = -1,connected;
  char *recv_data;
  struct sockaddr_in server_addr,client_addr;
  socklen_t sin_size;
  int recv_data_len;
  
//  printf("本地端口号是%d\n\n",LOCAL_PORT);
  
  recv_data = (char *)pvPortMalloc(RECV_DATA);
  if (recv_data == NULL)
  {
//      printf("No memory\n");
      goto __exit;
  }
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
//      printf("Socket error\n");
      goto __exit;
  }
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(LOCAL_PORT);
  memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
  
  if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
  {
//      printf("Unable to bind\n");
      goto __exit;
  }
  
  if (listen(sock, 5) == -1)
  {
//      printf("Listen error\n");
      goto __exit;
  }
  
  while(1)
  {
    sin_size = sizeof(struct sockaddr_in);

    connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);

//    printf("new client connected from (%s, %d)\n",
//            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    {
      int flag = 1;
      
      setsockopt(connected,
                 IPPROTO_TCP,     /* set option at TCP level */
                 TCP_NODELAY,     /* name of option */
                 (void *) &flag,  /* the cast is historical cruft */
                 sizeof(int));    /* length of option value */
    }
    
    while(1)
    {
      recv_data_len = recv(connected, recv_data, RECV_DATA, 0);
      
      if (recv_data_len <= 0) 
        break;
      
//      printf("recv %d len data\n",recv_data_len);
      
//      write(connected,recv_data,recv_data_len);
      
    }
    if (connected >= 0) 
      closesocket(connected);
    
    connected = -1;
  }
__exit:
  if (sock >= 0) closesocket(sock);
  if (recv_data) free(recv_data);

}
#endif 

/******************************************************************
函数名称：
函数功能:建立以太网线程
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
TaskHandle_t TCP_Server_handle;
void TCP_Server_Init(void)
{
  // sys_thread_new("TCP_Server_Thread",TCP_Server_Thread,NULL,1024,5);//优先级5
  xTaskCreate(TCP_Server_Thread,
              "TCP_Server_Thread",
              1024,
              NULL,
              5,
              &TCP_Server_handle);
}
/******************************************************************
函数名称：
函数功能:以太网接收数据接口
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void TCP_Server_Thread(void *p)
{
	struct netbuf *buf;
	err_t err,err_rev;
  struct netconn *user_netconn;   // 创建一个netconn连接的结构体指针
	
  LWIP_UNUSED_ARG(p);                        // 消除警告	
//  while(1)
//  {
//    err = netconn_connect(user_netconn, &ipaddr, 5000); //正常连接这里只会执行一次，阻塞在下面的netconn_recv
////    netconn_listen(user_netconn);              // 进入侦听状态
//		/*
//      保活设定：保活的含义是让单片机能够知道网络断了，进入错误处理程序。

//      1.在tcpecho.h中进行如下设定
//      #define  LWIP_TCP_KEEPALIVE       1             //激活keepalive
//      #define  TCP_KEEPIDLE_DEFAULT     2000UL        //2秒内连接双方都无数据
//      #define  TCP_KEEPINTVL_DEFAULT    1000UL        //1秒发送一次保活探测
//      #define  TCP_KEEPCNT_DEFAULT      3UL           //3次保活探测无数据则进入错误回调函数
//      2.为tcp的so_options加上SOF_KEEPALIVE选项
//    */
////    remote_netconn->pcb.tcp->so_options |= SOF_KEEPALIVE;    //保活设定，实际上是tcp的一个选项
//    if (err == ERR_OK)
//    {
////      while ((err_rev = netconn_recv(xNetConn, &buf)) == ERR_OK) //阻塞在这里
//			while ((netconn_recv(remote_netconn, &buf)) == ERR_OK)
//      {
//        do
//        {
////          netbuf_data(buf, &data, &len);   //获取消息内容 消息长度
////          usb_printf("长度:%d 报文:%s",len,data);
////          netconn_write(xNetConn, data, len,  //将消息原样回复服务器
////                    NETCONN_NOCOPY);
//					SysCtrl.Ethernet.ETH_Length.data = netbuf_copy(buf, &SysCtrl.Ethernet.Data[0], 1536); // 接收此包全部数据到缓冲区，且获取该包字节数				 
//					EthernetInterfaceHandler();						
//        } while (netbuf_next(buf) >= 0);
//        netbuf_delete(buf);    //清空消息结构
//							
//      }
//    }
//    else
//    {
//      netconn_close(remote_netconn);                 //一定要关闭
//      netconn_delete(remote_netconn);                //一定要删除，否则在malloc xNetConn时候，4~6次就申请不到导致直接error退出
//      remote_netconn = netconn_new(NETCONN_TCP);     //重新建立新连接
//      vTaskDelay(2000);
//    }
//  }



//  while (1)
//  {
//		user_netconn = netconn_new(NETCONN_TCP);   // 创建一个本机的tcp netconn结构体
//		remote_netconn ->pcb.tcp->so_options |= SOF_KEEPALIVE;
//    netconn_bind(user_netconn, &ipaddr, 5000); // 绑定本机ip和端口号
//		err = netconn_listen(user_netconn);              // 进入侦听状态
//		
//		netconn_accept(user_netconn, &remote_netconn);
//		if(err==ERR_OK)
//		{
//			//程序到这里说明已经有一个client连接成功
//	    //加入下面这2句话的原因：已经有一个client连接到server后防止其他client再连接到server上。
//	    //不关闭这个连接的话，其他client可以成功连接到tcp_serverconn上，但是无法收发数据
//	    //在netconn_listen()设置TCP_DEFAULT_LISTEN_BACKLOG 0x01 也不能阻止其他端口过来连接，还是下面两句代码后实在，直接关闭端口
//				netconn_close(user_netconn);  //关闭tcp_serverconn连接
//				netconn_delete(user_netconn); //删除tcp_serverconn连接
//			
//				while(1)
//				{
//						if ((err = netconn_recv(remote_netconn, &buf)) == ERR_OK)
//						{
//							SysCtrl.Ethernet.ETH_Length.data = netbuf_copy(buf, &SysCtrl.Ethernet.Data[0], 1536); // 接收此包全部数据到缓冲区，且获取该包字节数						 
//							EthernetInterfaceHandler();
//							netbuf_delete(buf);
//							
//						}else
//						{
//								//发现如果因为设置newconn.recv_timeout=5000;   如果5s内未发送数据给到服务端，会因为超时跳出：err=ERR_TIMEOUT报错，后正常netconn_close();netconn_delete()都无法释放资源
//								netconn_close(remote_netconn); //关闭 newconn 连接
//								netconn_delete(remote_netconn);//释放 newconn 资源
////							  vTaskDelay(200);
//								break;                  //退出内while1，去外层while1,
//						}	
//				
//				}
//					
//		}
//		
//		else
//		{
//	 		  netconn_close(remote_netconn); //关闭 newconn 连接
//	    	netconn_delete(remote_netconn);//释放 newconn 资源			
//		}
//	}


		user_netconn = netconn_new(NETCONN_TCP);   // 创建一个本机的tcp netconn结构体
		netconn_bind(user_netconn, &ipaddr, 5000); // 绑定本机ip和端口号
		netconn_listen(user_netconn);              // 进入侦听状态
    // 开始接收连接，如果没有连接则阻塞（直接调用函数阻塞本线程吗？线程阻塞不需要句柄吗？还是说阻塞另一个线程才需要该线程的句柄？）
    while (netconn_accept(user_netconn, &remote_netconn) == ERR_OK) // 如果未连接上，则阻塞，不影响其他线程工作，如果由远端主动挥手关闭连接会如何操作？
    {
      struct netbuf *buf;
      remote_netconn->pcb.tcp->so_options |= SOF_KEEPALIVE;  //开启keep_live功能
      while ((netconn_recv(remote_netconn, &buf)) == ERR_OK) // 已连接上，等待接收一个数据包（1500字节），如无数据则阻塞 如何在阻塞之后执行程序？
      {
        // TODO 使用len来判断是否结束，因为结束那一包长度小于1460，也有可能是1448（真正调试时进行读写），所以结束那一包不能等于1460
        SysCtrl.Ethernet.ETH_Length.data = netbuf_copy(buf, &SysCtrl.Ethernet.Data[0], 1536); // 接收此包全部数据到缓冲区，且获取该包字节数
       
				EthernetInterfaceHandler();
				netbuf_delete(buf);                                // 接收buf完毕后释放全部接收内存
      }
      // 当远端主动挥手时达到这
      netconn_close(remote_netconn);  // 与远端挥手
      netconn_delete(remote_netconn); // 与远端关闭，等待下次连接
    }
	

	
}

#if 0

/*-----------------------------------------------------------------------------------*/
static void 
tcpecho_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err;
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  /* Bind connection to well known port number 7. */
#if LWIP_IPV6
  conn = netconn_new(NETCONN_TCP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, LOCAL_PORT);
#else  /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_TCP);
  netconn_bind(conn, IP_ADDR_ANY, LOCAL_PORT);
#endif /* LWIP_IPV6 */
  LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

  printf("本地端口号是%d\n\n",LOCAL_PORT);
  
  /* Tell connection to go into listening mode. */
  netconn_listen(conn);

  while (1) {

    /* Grab new connection. */
    err = netconn_accept(conn, &newconn);//阻塞，等待连接,只建立一个新的连接newconn用于开启删除操作？？？？
    /*printf("accepted new connection %p\n", newconn);*/
    /* Process the new connection. */
    if (err == ERR_OK) {
      struct netbuf *buf;
      void *data;//一级void指针
      u16_t len;
      
      while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {//一次数据接收完毕(一个数据包？是一次数据还是一个buf包？？) 
      //数据存储到netbuf链表 输入参数为buf的二级指针，将netbuf的首地址传递给buf

        /*printf("Recved\n");*/
        do {
             netbuf_data(buf, &data, &len);//获取一个netbuf 的数据区指针和长度

             memcpy(recv_arr,data,len);//令一个指针指向每次仅需要偏移recv_arr的指针即可
             /*
              在此copy，结束后再进行处理
             */
             err = netconn_write(newconn, data, len, NETCONN_COPY);//直接从buf源地址再发回去再发回去
#if 0
            if (err != ERR_OK) {
              printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
            }
#endif
        } while (netbuf_next(buf) >= 0);//这里的NEXT指的是数据区指向下一个buf的数据区
        netbuf_delete(buf);//释放全部buf的内存
      }
      /*printf("Got EOF, looping\n");*/ 
      /* Close connection and discard connection identifier. */
      netconn_close(newconn);
      netconn_delete(newconn);
    }
  }
}
/*-----------------------------------------------------------------------------------*/
void
tcpecho_init(void)
{
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, 512, 8);
}

#endif
