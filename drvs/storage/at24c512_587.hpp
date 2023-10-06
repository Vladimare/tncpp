#ifndef _AT24C512_587_HPP_
#define _AT24C512_587_HPP_

#include "at24c512.hpp"

 
class at24c512_587
  : public at24c512
{
public:
  at24c512_587(extstorageCreationDisposition* escd);
  virtual ~at24c512_587();

protected:
  virtual void wp_enable(bool enable = false);

};

#endif /*_AT24C512_587_HPP_*/
