#include <modbus.h>
#include <iostream>
#include <time.h>
#include <unistd.h>

#define PORTNAME "dev/ttyUSB0"
#define YOUR_DEVICE_ID 1

using namespace std;

unsigned int crc_chk(unsigned char *data, unsigned char length)
{
	int i, j;
	unsigned int crc_reg = 0xFFFF;
	while (length--)
	{
		crc_reg ^= *data++;
		for (j = 0; j < 8; j++)
		{
			if (crc_reg & 0x01)
			{
				crc_reg = (crc_reg >> 1) ^ 0xA001;
			}
			else
			{
				crc_reg = crc_reg >> 1;
			}
		}
	}
	return crc_reg;
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

	// int nb = 8;
	// unsigned int chk;
	// uint8_t *a;
	// a = new uint8_t[nb];

	// uint8_t sp[8] = {0x01, 0x06, 0x00, 0x4a, 0x00, 0x28};
	// for(int i =0; i<8;i++)
	// {
	// 	a[i] =sp[i];
	// }
	// chk =crc_chk(a,6);
	// a[6] = uint16_t(chk);
	// a[7] = (uint8_t)((int(chk) - int(a[6]))/int(256));

	uint16_t *dest = new uint16_t;
	// modbus_read_registers(m_ac,117,1,dest);  cout << *dest << endl;
	modbus_write_register(m_ac, 70, 32690);
	modbus_write_register(m_ac, 71, 32511);
	modbus_write_register(m_ac, 71, 31487);
	sleep(3);
	modbus_write_register(m_ac, 71, 32691);

	//std::cout <<hex<< a[0] << a[1] << a[2] << a[3] << a[4] << a[5] << a[6] << a[7] <<endl;

	// for (int i =0; i<8;i++)
	// {
	// 	printf("%02X \n",a[i]);
	// }

	//位置控制 Pn002      2
	//使用內部位置指令0    POS1 和POS2都是0
	//使用內部速度指令     Pn168   爲1
	//使用內部位置指令1    Pn122 2
	//					Pn123 0
	//一共两万个脉冲,2圈
	//Pn117 使用内部位置指令 1
	//前馈增益 Pn113 随便设置成为了 1

	return 0;
}