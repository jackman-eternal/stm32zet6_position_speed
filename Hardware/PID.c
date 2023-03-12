#include "PID.h"               
//p=3.5,i=0.4   
//position best p =1.3  ii=0.008 d=0.2    
float Err=0,last_err=0,next_err=0,pwm=0,add=0,p=2,i=0.002,d=0,ind=0;
float Err2=0,last_err2=0,next_err2=0,pwm2=0,add2=0,p2=5,i2=0,d2=0,ind2=0;
float actual_speed = 0;  /*实际测得速度*/
//串级PID位置控制
int16_t myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

int pwm_control(int pwm)
{
	    pwm=myabs(pwm);
      if(pwm>99)
         pwm=99;
      if(pwm<0)
         pwm=0;
			return pwm;
}
int speed_val_protect(int val)
{
		/*目标速度上限处理*/
	if (val > 80)
	{
		val = 80;
	}
	else if (val < -80)
	{
		val = -80;
	}	
	return val;
}
float pid1(int16_t speed1,float tar1)
{
    speed1=myabs(speed1);
    Err=tar1-speed1;
    add=p*(Err-last_err)+i*(Err)+d*(Err+next_err-2*last_err);
    pwm+=add;
    pwm_control(pwm);
    next_err=last_err;
    last_err=Err;
    return pwm;
}
const int target =50;
int control_val_int=0;
float control_val = 0;
void autocallback(void)
{
 
	static int encodernow;
	static int encoderlast;
	int res_pwm;/*PID计算得到的PWM值*/
	int encodedelta; //encoder change
	encodernow = read_encoder()+overtime*65536;
	encodedelta = encodernow - encoderlast;
	encoderlast = encodernow;
	actual_speed = (float)encodedelta/30*4*11;
	
	control_val = (int)PID_realize(encodernow,30*4*11/4); 
	res_pwm=(int)speed_pid_realize(actual_speed, control_val/5);
	res_pwm=pwm_val_protect(res_pwm);
	set_motor_rotate2(res_pwm);
	printf("angle:%d,%d \r\n",encodernow,30*4*11/4);
	//printf("angle:%d,%d \r\n",encodedelta,0);
	//res_pwm =(int)PID_realize(encodernow,30*4*11);
	
	// set_motor_rotate(res_pwm/9);
	  
	// res_pwm=pid1(actual_speed,50);
	//printf("angle:%d,%d \r\n",encodedelta,50);
	//printf("angle:%d,%d,%d \r\n",encodernow,30*4*11,3);	
}

float PID_realize(float actual_val,float tar1)
{
	/*计算目标值与实际值的误差*/
	Err = tar1 - actual_val;
		if(Err>-3.0 && Err<3.0)
	{
		Err = 0;
		ind = 0;
		last_err = 0;
	}
	/*积分项*/
  ind+= Err;
	/*PID算法实现*/
	add = p*Err+i*ind+d*(Err-last_err);
	/*误差传递*/
	last_err=Err;

	/*返回当前实际值*/
	return add;
}

float speed_pid_realize(float actual_val,float tar1)
{
	Err2 = tar1 - actual_val;
	//设置闭环死区
	if(Err2>-10.0 && Err2<10.0)
	{
		Err2 = 0;
		ind2 = 0;
		last_err2 = 0;
	}
	/*积分项，积分分离，偏差较大时去掉积分作用*/
	if(Err2>-100 && Err2<100)
	{
			ind2+= Err2;
		  /*积分范围限定，防止积分饱和*/
		if(ind2> 200)
		{
			ind2 = 200;
		}
		else if(ind2<-200)
		{
			ind2 = -200;;
		}
	}
	add2 = p2*Err2+i2*ind2+d2*(Err2-last_err2);
	last_err2=Err2;
	return add2;
}