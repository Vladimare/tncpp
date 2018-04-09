#ifndef _M25P16_560_HPP_
#define _M25P16_560_HPP_

#include "m25p16.hpp"


class m25p16_560
  : public m25p16
{
public:
  m25p16_560(extstorageCreationDisposition* escd);
  virtual ~m25p16_560();

protected:
  virtual void chip_select(unsigned char ic, bool select = true);
};

#endif /*_M25P16_560_HPP_*/
