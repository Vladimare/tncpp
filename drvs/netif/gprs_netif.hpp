#ifndef _GPRSNETIF_HPP_
#define _GPRSNETIF_HPP_


#include "gsm.hpp"
#include "tn_net_func.h"
#include "tn_netif.h"


class gprs_netif
{
public:
  gprs_netif(TN_NET* tn, gsm* g);
  ~gprs_netif();


  TN_NETIF tn_netif;                  /* управляющий блок tn net                        */

protected:
  int gprs_rx(unsigned char* data, int sz);

private:
  gsm*    low;                        /* gsm-подложка                                   */
  TN_NET* pnet;                       /* верхний уровень, на который работает данный    */
                                      /*   интерфейс                                    */

  static gprs_netif* _gprs;

  /* эти методы доступны только через tn net, напрямую их вызвать нельзя */
  static int gprs_tx    (TN_NET* tnet, struct tn_netif* ni, TN_MBUF* mb);
  static int gprs_ioctl (TN_NET* tnet, struct tn_netif* ni, int req_type, void* par);
  static int gprs_init  (TN_NET* tnet, struct tn_netif* ni);
  static const char* gprs_addr(void);
};


#endif /* _GPRSNETIF_HPP_ */
