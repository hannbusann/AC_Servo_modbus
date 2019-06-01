#include <iostream>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include "std_msgs/Float64MultiArray.h"
#include <stdio.h>
#include <vector>
#include <modbus.h>
#include <cmath>

#define PORTNAME "dev/ttyUSB0"
#define YOUR_DEVICE_ID 1
#define TH1_INIT 3677
#define TH2_INIT 1744
using namespace std;

double th1_init = (double)TH1_INIT / 4096.0 * 360 / 57.3;
double th2_init = (double)(4096-TH2_INIT) / 4096.0 * 360 / 57.3;
double now_th1 = 0; //都需要是弧度制的
double now_th2 = 0;
double now_th1d = 0;
double now_th2d = 0;
double now_angle = 0;
int hh =0;

FILE *fpWrite = fopen("/home/fw/lab_2sensor_arduino/encoder_talker/src/encoder_talker/data/data1.txt", "w");


unsigned long gettimes()
{
  struct timespec time1 = {0, 0};
  clock_gettime(CLOCK_REALTIME, &time1);
  long nows = time1.tv_nsec + time1.tv_sec * 1000000000;
  return nows;
}
void Delay(int time) //time*1000为秒数
{
  clock_t now = clock();

  while (clock() - now < time)
    ;
}
void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr &msg)
{
  now_th1 = msg->data[0] / 57.3 - th1_init;
  now_th2 = msg->data[2] / 57.3 - th2_init;
  now_th1d = msg->data[1] / 57.3;
  now_th2d = msg->data[3] / 57.3;
  cout  << now_th1 << "\t" << now_th1d << "\t"
  << now_th2 << "\t" << now_th2d << endl;


    if (hh < 2)
    {
        hh++;
    }
    else
    {
        hh=0;
        fseek(fpWrite, 0, SEEK_END);
        fprintf(fpWrite, "%f    %f    %f    %ld \n", now_th2,now_th2d,now_angle,gettimes());   
    }

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "sine_listen");                                           //initialize the node
  ros::NodeHandle nh;                                                            //communicate with the master
  ros::Subscriber sub_channel = nh.subscribe("encoderInfo", 1, chatterCallback); //subcribe the wanted topic "chatter"
  ros::Rate loop_rate(100);

  modbus_t *m_ac;
  m_ac = modbus_new_rtu("/dev/ttyUSB0", 115200, 'O', 8, 1);
  ROS_FATAL("rtu new");
  modbus_set_debug(m_ac, 1);               //设置为1将可以看到调试信息
  modbus_set_response_timeout(m_ac, 1, 0); //设置等待延时时间

    double amplitude = 15 ;
	double circle = 1.5;
	double interval = 0.01; //以100ms为周期
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
	
	while(ros::ok())
	{
		modbus_write_register(m_ac, 71, 32511); 
		time+=interval;
		now_position = amplitude -amplitude*std::cos(time/circle*2*M_PI);
		cout << now_position<< endl;
		position_error = now_position - last_position;
		last_position = now_position;
		now_angle = now_position;
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
		loop_rate.sleep();  
		ros::spinOnce();                                        ///////////////////////hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
	};

return 0;
}
