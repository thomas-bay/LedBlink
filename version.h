#include <string>

using std::string;

class Version
{
  string _file;
  string _version;
  string _date;
  string _time;
  int i;

public:
  Version();
  string getFile() {return _file;}
  string getVersion() {return _version;}
  string getDate();
  string getTime();
};
