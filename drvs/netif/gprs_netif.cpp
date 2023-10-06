#include "errors.h"
#include "gprs_netif.hpp"
#include "syslog.hpp"
#include "tn_ip.h"


gprs_netif* gprs_netif::_gprs = 0;


gprs_netif::gprs_netif(TN_NET* tn, gsm* g)
{
  gprs_netif::_gprs = this;

  this->low = g;
  this->pnet= tn;

  this->tn_netif.next                 = 0;
  this->tn_netif.ip_addr.s__addr      = _INADDR_ANY;
  this->tn_netif.netmask.s__addr      = 0xFFFFFF00;
  this->tn_netif.ip_gateway.s__addr   = _INADDR_ANY;
  this->tn_netif.if_broadaddr.s__addr = 0xFFFFFFFF;
  this->tn_netif.if_flags             = IFF_POINTOPOINT | IFF_NOARP | IFF_SIMPLEX;
  this->tn_netif.if_mtu               = 1500;
  this->tn_netif.tcp_last_inpcb       = 0;
  this->tn_netif.task_rx_stack        = 0;
  this->tn_netif.queueIfaceRxMem      = 0;
  this->tn_netif.queueIfaceTxMem      = 0;
  this->tn_netif.drv_info             = 0;
  this->tn_netif.drv_wr               = gprs_netif::gprs_tx;
  this->tn_netif.drv_ioctl            = gprs_netif::gprs_ioctl;
  this->tn_netif.drv_init             = gprs_netif::gprs_init;
  this->tn_netif.drv_addr             = gprs_netif::gprs_addr;
  this->tn_netif.arp_arr              = 0;
  this->tn_netif.arp_arr_capacity     = 0;
  this->tn_netif.arp_sem              = 0;
  this->tn_netif.dup_own_ip_addr      = 0;
  this->tn_netif.dhi                  = 0;
  this->tn_netif.dhcp_task_rx_stack   = 0;
  memcpy(&this->tn_netif.hw_addr, "\x10\x0C\xCC\xBB\xAA\x99", 6);
  memset(&this->tn_netif.task_rx, 0, sizeof(TN_TCB));
  memset(&this->tn_netif.queueIfaceRx, 0, sizeof(TN_DQUE));
  memset(&this->tn_netif.queueIfaceTx, 0, sizeof(TN_DQUE));
  memset(&this->tn_netif.dhcp_task_rx, 0, sizeof(TN_TCB));
  memset(&this->tn_netif.dhcp_sem, 0, sizeof(TN_SEM));
};


int gprs_netif::gprs_rx(unsigned char* data, int sz)
{
  ipv4_input(this->pnet, &this->tn_netif, 0);

  return 0;
}

int gprs_netif::gprs_tx(TN_NET* tnet, struct tn_netif* ni, TN_MBUF* mb)
{
  return 0;
}

int gprs_netif::gprs_ioctl(TN_NET* tnet, struct tn_netif* ni, int req_type, void* par)
{
  return 0;
}

int gprs_netif::gprs_init(TN_NET* tnet, struct tn_netif* ni)
{
  if(!gprs_netif::_gprs->low)
    return ERR_PARAM;                 /* не указан драйвер gsm                          */

  //TODO: добавить чтение gprs-конфигурации
  //TODO: cid должен выбираться из диапазона, который возвращает модем
  int err = gprs_netif::_gprs->low->setPDP(1);
  if(err != ERR_OK)
  {
    g_writelog(LOG_LEVEL_CRITICAL, "gprs: setPDP() failed (err == %d)\r\n", err);
    return err;
  }

  return gprs_netif::_gprs->low->connectGPRS(1);
}

const char* gprs_netif::gprs_addr()
{
  return "0xFEDCBA98";
}
