#include "serial.hpp"

const portSettings serialPort::defUARTSettings= {UARTSIZE_8, STOPBITS_1, PARITY_NONE, BR_9600,    12000000ul};
const portSettings serialPort::defSSPSettings = {SSPSIZE_8,  STOPBITS_1, PARITY_NONE, BR_6000000, 12000000ul};
const portSettings serialPort::defI2CSettings = {I2CSIZE_8,  STOPBITS_1, PARITY_NONE, BR_100000,  12000000ul};
