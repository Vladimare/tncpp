#ifndef _GPS587_HPP_
#define _GPS587_HPP_

#include "gps.hpp"


class gps_587
  : public gps
{
public:
  gps_587();
  virtual ~gps_587();

private:
  void pinSetup(bool setDefault = false);
};


#endif /*_GPS587_HPP_*/
