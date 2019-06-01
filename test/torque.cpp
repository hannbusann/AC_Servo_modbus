#include <modbus.h>
#include <cmath>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>

#define PORTNAME "dev/ttyUSB0"
#define YOUR_DEVICE_ID 1

using namespace std;


long gettimes()
{
struct timespec time1 = {0, 0};
    clock_gettime(CLOCK_REALTIME, &time1);
    long nows = time1.tv_nsec+ time1.tv_sec *1000000000;
    return nows;
}
void Delay(int time)//time*1000为秒数 
{ 
  clock_t now = clock(); 

 while( clock() - now < time); 
} 


int main(int argc, char **argv)
{
	modbus_t *m_ac;
	m_ac = modbus_new_rtu("/dev/ttyUSB0", 115200, 'O', 8, 1);
	modbus_set_debug(m_ac, 1); //设置为1将可以看到调试信息

	modbus_set_response_timeout(m_ac, 1, 0); //设置等待延时时间

	if (m_ac == NULL)
	{
		fprintf(stderr, "Unable to create the libmodbus context\n");
		return -1;
	}

	int rc = modbus_set_slave(m_ac, YOUR_DEVICE_ID);
	if (rc == -1)
	{
		fprintf(stderr, "Invalid slave ID\n");
		modbus_free(m_ac);
		return -1;
	}

	if (modbus_connect(m_ac) == -1)
	{
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(m_ac);
		return -1;
	}

	
	

	return 0;
}