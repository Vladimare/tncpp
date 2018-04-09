#ifndef _M25P16_587_HPP_
#define _M25P16_587_HPP_

#include "m25p16.hpp"


class m25p16_587
  : public m25p16
{
public:
  m25p16_587(extstorageCreationDisposition* escd);
  virtual ~m25p16_587();

protected:
  virtual void chip_select(unsigned char ic, bool select = true);
};

#endif /*_M25P16_587_HPP_*/
