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

	double amplitude = 15 ;
	double circle = 1.5;
	double interval = 0.03; //以100ms为周期
	int delay_time =(int)(interval*1000000);
    double time=0;
	int pulse_count = (int)(2000000.0/120.0*interval);
	double last_position =0;
	double now_position;
	double position_error;
	double velocity;
	int numerator;
	modbus_write_register(m_ac,123,pulse_count);
	modbus_write_register(m_ac, 98, 100); //电子齿轮比调大
	
	modbus_write_register(m_ac, 70, 32690); //son 为0时是ON，电机上电
	modbus_write_register(m_ac, 71, 32511);//选取内部位置指令1
	
	while(1)
	{
		modbus_write_register(m_ac, 71, 32511); 
		time+=interval;
		now_position = amplitude -amplitude*std::cos(time/circle*2*M_PI);
		cout << now_position<< endl;
		position_error = now_position - last_position;
		last_position = now_position;
		if(position_error>0)
		{
			modbus_write_register(m_ac,123,pulse_count);
			//  cout << "++++++++++++++"<< pulse_count<< endl;
		}
		else
		{
			modbus_write_register(m_ac,123,-pulse_count);
			//  cout << "--------------"<< pulse_count<< endl;
			
		}
		velocity = position_error/interval;
		// cout << "velocity" << velocity <<endl;
		numerator = (int)(velocity/27.0*100.0);
		if(numerator<0)
		{
			numerator = -numerator;
		}
		else if(numerator ==0)
		{
			numerator = 1;
		}
		// cout << "numerator"<< numerator <<endl;
		modbus_write_register(m_ac, 98, numerator); //电子齿轮比调节   ///////////////////////hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
		modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
		// cout << "delay_time"<< delay_time << endl;
		Delay(delay_time);   ///////////////////////hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
	};


		// pulse_count  =833;
 	    // Delay(1000000);
	    // modbus_write_register(m_ac,123,-pulse_count);
		// modbus_write_register(m_ac, 98, 10); //电子齿轮比调大
		// modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
		// Delay(50000);

		// modbus_write_register(m_ac, 71, 32511); 
		// modbus_write_register(m_ac,123,	pulse_count);
		// modbus_write_register(m_ac, 98, 10); //电子齿轮比调大
		// modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
		// Delay(50000);

		// modbus_write_register(m_ac, 71, 32511); 
		// modbus_write_register(m_ac,123,-pulse_count);
		// modbus_write_register(m_ac, 98, 10); //电子齿轮比调大
		// modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
		// Delay(50000);

		// modbus_write_register(m_ac, 71, 32511); 
		// modbus_write_register(m_ac,123,	pulse_count);
		// modbus_write_register(m_ac, 98, 10); //电子齿轮比调大
		// modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
		
		// Delay(2000000);

	 	// modbus_write_register(m_ac, 70, 32691);	//son 为0时是ON，电机断电




	// uint16_t *dest = new uint16_t;
	// // modbus_read_registers(m_ac,117,1,dest);  cout << *dest << endl;
	// 	long now=gettimes();
	// 	modbus_write_register(m_ac, 70, 32690); //son 为0时是ON，电机上电
	// 	long now0=gettimes();
	// 	modbus_write_register(m_ac, 71, 32511);//选取内部位置指令1
	// 	long now1=gettimes();
	// 	modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
	// 	long now2=gettimes();
	// 	cout <<"now "<<now<<endl;
	// 	cout <<"now0 "<<now<<endl;
	// 	cout <<"now1 "<<now<<endl;
	// 	cout <<"now2 "<<now<<endl;

	// 	cout << "here" << endl;
	// 	sleep(10);
	// 	modbus_write_register(m_ac, 98, 18); //电子齿轮比调大
	// 	modbus_write_register(m_ac, 71, 31487); //ptriger触发内部指令
	// 	sleep(10000);
	// 	cout << "here" << endl;	
	// 	cout << "time pass" << endl;
	// 	modbus_write_register(m_ac, 71, 32767); //全部默认
	// 	modbus_write_register(m_ac, 70, 32691);	//son 为0时是ON，电机断电
		


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