#include "BasicTypes.h" 
#include "iic.h"


typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	        0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	        0x3B
#define	ACCEL_XOUT_L	        0x3C
#define	ACCEL_YOUT_H	        0x3D
#define	ACCEL_YOUT_L	        0x3E
#define	ACCEL_ZOUT_H	        0x3F
#define	ACCEL_ZOUT_L	        0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	        0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ



uchar dis[4];							//��ʾ����(-511��512)���ַ�����
int	dis_data;						//����
int	Temperature,Temp_h,Temp_l;	//�¶ȼ��ߵ�λ����


void setMPU6050()
{

}
N8 InitializeMPU6050(void)
{
        N8 Error = 0,Value = 0;
        Error = IIC(0X0D,READ,117 ,1,(char *)&Value);
        if(Value == 104) {
              
                Value = 0x00;Error = IIC(0X0D,WRITE,PWR_MGMT_1  ,1,(char *)&Value);
                             //Error = IIC(0X0D,READ ,PWR_MGMT_1  ,1,(char *)&Value);
                Value = 0x07;Error = IIC(0X0D,WRITE,SMPLRT_DIV  ,1,(char *)&Value);
                             //Error = IIC(0X0D,READ ,SMPLRT_DIV  ,1,(char *)&Value);
                Value = 0x06;Error = IIC(0X0D,WRITE,CONFIG      ,1,(char *)&Value);
                             //Error = IIC(0X0D,READ ,CONFIG      ,1,(char *)&Value);
                Value = 0x18;Error = IIC(0X0D,WRITE,GYRO_CONFIG ,1,(char *)&Value);
                             //Error = IIC(0X0D,READ ,GYRO_CONFIG ,1,(char *)&Value);
                Value = 0x01;Error = IIC(0X0D,WRITE,ACCEL_CONFIG,1,(char *)&Value);
                             //Error = IIC(0X0D,READ ,ACCEL_CONFIG,1,(char *)&Value);     
              
        }
        return Error;
}                  //N8 stm32spi(N8 Name,N8 RW,N8 Wait,N16 Size,char *BufferAddr)


static void Acceleration()
{

}
static void Inclination()
{

}
void runMPU6050(void)//20ms
{ 

//        Error = IIC(0X0D,READ,ACCEL_XOUT_H,14,(char *)outD);
//        Error = IIC(0X0D,READ,ACCEL_XOUT_L,1,(char *)&Value1); 
//        Error = IIC(0X0D,READ,0 ,118,(char *)&Value);  



        static N8 Error = 0,outD[14],outD2[14],con,con1;

        Error = IIC(0X0D,READ,0x3B,14,(char *)outD2);


        outD[1] = outD2[0];
        outD[0] = outD2[1];
        outD[3] = outD2[2];
        outD[2] = outD2[3];
        outD[5] = outD2[4];
        outD[4] = outD2[5];
        outD[7] = outD2[6];
        outD[6] = outD2[7];
        outD[9] = outD2[8];
        outD[8] = outD2[9];
        outD[11] = outD2[10];
        outD[10] = outD2[11];
        outD[13] = outD2[12];
        outD[12] = outD2[13];

}



