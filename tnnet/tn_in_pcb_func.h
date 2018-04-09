#ifndef _TN_IN_PCB_FUNC_H_
#define _TN_IN_PCB_FUNC_H_


#include "tn_netif.h"


int  in_pcballoc      (TN_NET* tnet, struct tn_socket* so, struct inpcb* head);
int  in_pcbbind       (TN_NET* tnet, struct inpcb* inp, struct mbuf* nam);
int  in_pcbconnect    (TN_NET* tnet, struct inpcb* inp, struct mbuf* nam);
int  in_broadcast     (struct in__addr in, TN_NETIF* ni);
void in_pcbdisconnect (TN_NET* tnet, struct inpcb* inp);
void in_pcbdetach     (TN_NET* tnet, struct inpcb* inp);
void in_setsockaddr   (struct inpcb* inp, struct mbuf* nam);
void in_setpeeraddr   (struct inpcb* inp, struct mbuf* nam);

struct inpcb* in_pcblookup(TN_NET* tnet, struct inpcb* head, unsigned int faddr,
                                                             unsigned int fport_arg,
                                                             unsigned int laddr,
                                                             unsigned int lport_arg,
                                                             int flags);


#endif /* _TN_IN_PCB_FUNC_H_ */
