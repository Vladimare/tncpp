#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include "bsd_socket.h"
#include "tn_ip.h"
#include "tn_net.h"
#include "tn_netif.h"
#include "tn_socket.h"
#include "tn_socket_func.h"


class socket
{
public:
  socket(int domain, int type, int protocol);
  ~socket();

  int bind    (const struct _sockaddr* name, int  namelen);
  int connect (      struct _sockaddr* name, int  namelen);
  int accept  (      struct _sockaddr* name, int* namelen);
  int listen  (int backlog);

  int recv    (unsigned char* buf, int bufsz, int flags);
  int recvfrom(unsigned char* buf, int bufsz, int flags, struct _sockaddr* from, int* fromlen);

  int send    (unsigned char* buf, int bufsz, int flags);
  int sendto  (unsigned char* buf, int bufsz, int flags, struct _sockaddr* to, int tolen);

  int shutdown(int how);

  int getpeername(struct _sockaddr* name, int* namelen);

  int setsockopt(int name, unsigned char* val, int  valsz);
  int getsockopt(int name, unsigned char* val, int* valsz);

  static int init_network(TN_NETIF* nif);
  static int close_network(TN_NETIF* nif);


  static TN_NET net;

private:
  int hsock;                          /* socket descriptor                              */

  static TN_NETINFO netinfo;
  static unsigned char mempool[(TNNET_HI_BUF_SIZE    * NUM_HI_BUF)        +
                               (TNNET_SMALL_BUF_SIZE * NUM_DRV_SMALL_BUF) +
                               (TNNET_MID1_BUF_SIZE  * NUM_DRV_MID1_BUF)];
  static const net_proto_timer_func g_net_proto_timer_functions[];


  static void net_timer(void* param);
};


#endif /*_SOCKET_HPP_*/
