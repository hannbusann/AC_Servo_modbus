#include <ServoActuator.h>

#define PORTNAME "dev/ttyUSB0"
//#define YOUR_DEVICE_ID 1

ServoActuator::ServoActuator(int slave_num)
{
    modbus_t *m_ac;
	m_ac = modbus_new_rtu("/dev/ttyUSB0", 115200, 'O', 8, 1);
	modbus_set_debug(m_ac, slave_num); //设置为1将可以看到调试信息
    
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



}