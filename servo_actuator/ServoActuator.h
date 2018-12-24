#include <modbus.h>
#include <iostream>
#include <time>
#include <unistd.h>

namespace ACservo
{
class ServoActuator
{
  public:
    std::vector<int> slave;

    ServoActuator(int slave_num);
    void setDebug(bool isdebug);
    
}

} // namespace ACservo