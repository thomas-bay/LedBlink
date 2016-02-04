#include "version.h"

Version::Version ()
{
  _file = __FILE__;
  _version = "LedBlink V1.01";
  _date = __DATE__;
  _time = __TIME__;
}

string Version::getDate()
{
  return _date;
}

string Version::getTime()
{
  return _time;
}
