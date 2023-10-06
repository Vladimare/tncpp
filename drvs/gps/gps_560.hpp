#ifndef _GPS560_HPP_
#define _GPS560_HPP_

#include "gps.hpp"


class gps_560
  : public gps
{
public:
  gps_560();
  virtual ~gps_560();

private:
  void pinSetup(bool setDefault = false);
};


#endif /*_GPS560_HPP_*/
