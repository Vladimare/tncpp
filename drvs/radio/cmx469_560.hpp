#ifndef _CMX469_560_HPP_
#define _CMX469_560_HPP_

#include "cmx469.hpp"


/* реализация класса cmx469 на плате 560 (Титан-3) */
class cmx469_560
  : public cmx469
{
public:
  cmx469_560(stat_events* se);
  virtual ~cmx469_560();

protected:
                                      /* метод первоначальной настройки ног контроллера */
  virtual void pinSetup(bool setDefault = false);
                                      /* разрешение / запрет приемника микросхемы       */
  virtual void rxEnable(unsigned char enable = 1);
                                      /* разрешение / запрет передатчика микросхемы     */
  virtual void txEnable(unsigned char enable = 1);
                                      /* возвращает 0, если сигнала CARRIER_DETECT нет, *
                                       *   иначе возвращает 1                           */
  virtual unsigned char isCarrierDetected();
                                      /* выставляет на линию бит                        */
  virtual void txbit(unsigned int bit);
  virtual unsigned char rxbit();      /* считывает с линии бит                          */

private:
  static void txSyncIrq(void* instance);
  static void rxSyncIrq(void* instance);
};

#endif /*_CMX469_560_HPP_*/
