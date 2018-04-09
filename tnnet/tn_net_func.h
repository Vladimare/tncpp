#ifndef _TN_NET_FUNC_H_
#define _TN_NET_FUNC_H_


#include "systimer.h"
#include "tn_netif.h"


#ifdef __cplusplus
extern "C"
{
#endif


void ip_init    (TN_NET* tnet);
void tcp_init   (TN_NET* tnet);
void udp_init   (TN_NET* tnet);
void icmp_input (TN_NET* tnet, TN_NETIF* ni, struct mbuf* m);
void ipv4_input (TN_NET* tnet, TN_NETIF* ni, struct mbuf* m);
void tcp_input  (TN_NET* tnet, TN_NETIF* ni, struct mbuf* m);
int  ip_output  (TN_NET* tnet, TN_NETIF* ni, TN_MBUF* m0);


#define tn_time_sec() systimerUptimeSec();

int tn_net_random_id(void);
int sockargs(TN_NET* tnet, struct mbuf** mp, unsigned char* buf, int buflen, int type);


#ifdef __cplusplus
}
#endif


#endif /* _TN_NET_FUNC_H_ */
