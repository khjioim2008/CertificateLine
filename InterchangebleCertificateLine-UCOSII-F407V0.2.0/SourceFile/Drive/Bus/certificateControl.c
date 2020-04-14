/**  
  *************************************************************
  * @file           
  * @author     
  * @version    
  * @date        
  * @brief        
  * @attention  
  *************************************************************
  */

#include "useCom.h"
#include "certificateControl.h"
#include "CRC16.h"
#include "Resources.h"
#include "flash.h"
#include "cpucrj.h"
#include "SerialPort.h"
#include "os.h"

#define		HEAD1	 0xA5
#define		HEAD2	 0xDB
#define   DATA_LENGHT 0x06


const u8 TEXT_Buffer[]={"1234567890"};
u16 datatemp;
//u16 heratVaule;
u8 m_data[6];
enum RECEIVE_STATE
{
	REC_STATE_HEAD1,
	REC_STATE_HEAD2,
	REC_STATE_CMD_LEN,
	REC_STATE_EQUIPMEN_TYPE,
	REC_STATE_FUNCTION_CODE,
	REC_STATE_DATA,
	REC_STATE_SOURCE_ADDRESS,
	REC_STATE_TARGET_ADDRESS,
};

enum ZIG_CONNECT_STATE
{
	ZIG_CONNECT_REQUEST,
	ZIG_CONNECT_ACTION,
	ZIG_CONNECT_COMPLETE,
	ZIG_CONNECT_RESEND,
};
//2018/6/14
typedef enum _tEQUIPMENT_TYPE//设备类型 3个
{
	UPPER_COMPUTER = 0x0001,
	SLAVE_COMPUTER = 0x0002,
	NODE           = 0x0003,
}EQUIPMENT_TYPE;

typedef enum _tFUNCTION_CODE//功能码 11个
{
	RECEIVE_CONTROL          = 0x0001,
	QUERY                    = 0X0002,
	NODE_CONTROL             = 0x0003,
	EXPAND                   = 0x02ff,//待修改
	RECEIVE_FEEDBACK         = 0x0301,
	QUERY_FEEDBACK   				 = 0x0302,
	COOR_QUERY_FEEDBACK      = 0x0304,
	NODE_CONTROL_FEEDBACK    = 0x0501,
	NODE_QUERY               = 0x0502,
	PROCESS_CONTROL          = 0x0503,
	PROCESS_CONTROL_FEEDBACK = 0x0504,
  CONTROL_REGISTER         = 0x0505,//2018.11.8
	UPLOAD_MESSAGE           = 0X0506,
	
}FUNCTION_CODE;

typedef enum _tSOURCE_TARGET_ADDRESS
{
	UPPER_ADDRESS           = 0x0001,
	UPPER_EXPAND_ADDRESS    = 0x0002,
	ZIG_COORDINATOR_ADDRESS = 0x0010,
	ZIG_END_FIRST_ADDRESS   = 0X0011,
	ZIG_END_SECOND_ADDRESS  = 0x0012,
	ZIG_END_THIRD_ADDRESS   = 0x0013,
	ZIG_END_FOURTH_ADDRESS  = 0x0014,
	ZIG_END_EXPAND_ADDRESS  = 0x00ff,
	MACHINE_FIRST_ADDRESS   = 0x0101,
	MACHINE_SECOND_ADDRESS  = 0x0102,
	MACHINE_THIRD_ADRESS    = 0x0103,
	MACHINE_FOURTH_ADRESS   = 0x0104,
	MACHINE_EXPAND_ADRESS   = 0x01ff,
	BROADCAST_ADRESS        = 0xfefe,
}SOURCE_TARGET_ADDRESS;

//2018/6/14
typedef struct{
 EQUIPMENT_TYPE equipmentType;
 FUNCTION_CODE functionCode;
 u8  lenZig;
 u8  cmdMSG[6];
 SOURCE_TARGET_ADDRESS addressIn;
 SOURCE_TARGET_ADDRESS addressOut;
 u8  crc16HB;
 u8  crc16LB;	
  
	u8 cmdLen;
	u8 cmd[64];
	u16 recOffset;
	u8 receiveStatus;
}ZIG_REC_MSG_INFO;

typedef struct{
 EQUIPMENT_TYPE equipmentType;
 FUNCTION_CODE functionCode;
 u8  lenZig;
 u8  cmdMSG[6];
 SOURCE_TARGET_ADDRESS addressIn;
 SOURCE_TARGET_ADDRESS addressOut;
 u8  crc16HB;
 u8  crc16LB;	
}MACHINE1_INFO;

typedef struct _tZIG_CONNECTION_INFO{
	u8 zigConnectStatus;
	u8 zigMachineBusy;
	u8 zigHeartbeatOFF;
}ZIG_CONNECTION_INFO;	
//2018/6/14
u8 zFlag;
MACHINE1_INFO g_machine1;
ZIG_REC_MSG_INFO g_certificateCom;
ZIG_CONNECTION_INFO z_certiConnect;
/**
  * @brief  	
  * @param 校验正确返回1
  * @retval
  */
static u8 CertificateControl_CheckCRC(u8 *_pMsgInfo, u16 _len, u8 checkH, u8 checkL)
{
	u16 CurVal = 0;
	CurVal = CRC16((char *)_pMsgInfo,_len);
	if(((CurVal & 0x00ff) == checkL) && ((CurVal >> 8) == checkH))
		return 1;
	return 0;
}


/**
  * @brief  	
  * @param 
  * @retval
	* 解析控制
  */
static void certificateControl_handle(u8 *_pRecMsg, u16 _len)
{
	u16 FuncCode;
	u16 cmdCode;
	u8 numCode;
	u16 ExControlValue1,ExControlValue2,ExControlValue3,ExControlValue4,ExControlValue5;
	u8 maction;
	FuncCode = (u16)(_pRecMsg[5] << 8) | _pRecMsg[6];
	switch(FuncCode)
	{
		case RECEIVE_CONTROL:
			break;
		case QUERY:
			break;
		case NODE_CONTROL:
			 numCode = _pRecMsg[7];
			//cmdCode = (u16)(_pRecMsg[7] << 8) | _pRecMsg[8];
			 if(numCode == MODULE_NUMBER){
				m_data[0] = MODULE_NUMBER; 
				cmdCode = (u16)(_pRecMsg[8] << 8) | _pRecMsg[9];
				if(cmdCode == ACTION_NUMBER){ 
					maction = 1;
					zFlag = 1;
				}else if(cmdCode == UNACTION_NUMBER){
					maction = 2;
					zFlag = 1;
				}					
			 }
			 else if(numCode == WIFI_CONNECT_T){//16
					maction = 3;
				 	zFlag = 1;
				 	z_certiConnect.zigHeartbeatOFF =1;	
					m_data[0] = WIFI_CONNECT_T;
				 
			 }
			 else if(numCode == ONE_MOTOR_NUMBER){//1
				maction = 10;
				zFlag = 1;
				m_data[0] = ONE_MOTOR_NUMBER;
				m_data[5] = NOT_DISPLAY;
				ExControlValue1 =  _pRecMsg[8];
				ExControlValue2 =  _pRecMsg[9];
				ExControlValue3 =  _pRecMsg[10];
				ExControlValue4 =  _pRecMsg[11];
				ExControlValue5 =  _pRecMsg[12];
				if((ExControlValue1 == 0xfe) && ((ExControlValue2 == 0xfe)) && ((ExControlValue3 == 0xfe)) && ((ExControlValue4 == 0xfe))&& ((ExControlValue5 == 0xfe)))
				{
						m_data[1] = FlashSave.flashSep.MembraneMotorDuty;
						m_data[2] = FlashSave.flashSep.MembraneRollerDuty;
						m_data[3] = FlashSave.flashSep.ReveresRollerDuty;
						m_data[4] = FlashSave.flashSep.PushMotorDuty;
						m_data[5] = FlashSave.flashSep.BrushRollerDuty;
				}
				if( (ExControlValue1 > 0)&&(ExControlValue1 <= PRECISION)){
				FlashSave.flashSep.MembraneMotorDuty  = ExControlValue1;
				}
				if( (ExControlValue2 > 0)&&(ExControlValue1 <= PRECISION)){
				FlashSave.flashSep.MembraneRollerDuty = ExControlValue2;
				}
				if( (ExControlValue3 > 0)&&(ExControlValue1 <= PRECISION)){
				FlashSave.flashSep.ReveresRollerDuty  = ExControlValue3;
				}
				if( (ExControlValue4 > 0)&&(ExControlValue1 <= PRECISION)){
				FlashSave.flashSep.PushMotorDuty      = ExControlValue4;
				}
				if( (ExControlValue5 > 0)&&(ExControlValue1 <= PRECISION)){
				FlashSave.flashSep.BrushRollerDuty    = ExControlValue5;
				}
			 }
				else if(numCode == TWO_MOTOR_NUMBER){//2
				maction = 10;
				zFlag = 1;
				m_data[0] = TWO_MOTOR_NUMBER;
				m_data[5] = NOT_DISPLAY;	
			  ExControlValue1 =  _pRecMsg[8];
				ExControlValue2 =  _pRecMsg[9];
				ExControlValue3 =  _pRecMsg[10];
				if((ExControlValue1 == 0xfe) && ((ExControlValue2 == 0xfe)) && ((ExControlValue3 == 0xfe)))
				{
					m_data[1] = FlashSave.flashSep.PaperRollerDuty;
					m_data[2] = FlashSave.flashSep.FlsamMotorDuty;
					m_data[3] = FlashSave.flashSep.EndRollerDuty;
					m_data[4] = 0x00;
					m_data[5] = 0x00;
				}
				if( (ExControlValue1 > 0)&&(ExControlValue1 <= PRECISION)){
					FlashSave.flashSep.PaperRollerDuty  = ExControlValue1;
				}
				if((ExControlValue2 > 0)&&(ExControlValue2 <= PRECISION)){
					FlashSave.flashSep.FlsamMotorDuty = ExControlValue2;
				}
				if( (ExControlValue3 > 0)&&(ExControlValue3 <= PRECISION)){
					FlashSave.flashSep.EndRollerDuty  = ExControlValue3;
				}
				}
				else if(numCode == THREE_MOTOR_NUMBER){//3
				maction = 10;
				zFlag = 1;
				m_data[0] = THREE_MOTOR_NUMBER;
				m_data[5] = NOT_DISPLAY;		
			  ExControlValue1 =  _pRecMsg[8];
				ExControlValue2 =  _pRecMsg[9];
				ExControlValue3 =  _pRecMsg[10];
				ExControlValue4 =  _pRecMsg[11];
//				ExControlValue5 =  _pRecMsg[12];
					//if(ExControlValue1 == 0xfe) && (ExControlValue2 == 0xfe))
					if((ExControlValue1 == 0xfe) && ((ExControlValue2 == 0xfe)))
					{
						m_data[1] = FlashSave.flashSep.F_DOUBLE_LEVEL;
						m_data[2] = FlashSave.flashSep.F_SINGLE_LEVEL;
						m_data[3] = FlashSave.flashSep.F_UP_LEVEL;
						m_data[4] = FlashSave.flashSep.F_DOWN_LEVEL;
						m_data[5] = 0x00;	
					}
					if((ExControlValue1 > 0)&& (ExControlValue1 <= PRECISION)){
					FlashSave.flashSep.F_DOUBLE_LEVEL = ExControlValue1;
					}
					if((ExControlValue2 > 0) && (ExControlValue2 <= PRECISION)){
					FlashSave.flashSep.F_SINGLE_LEVEL = ExControlValue2;
					}
					if((ExControlValue3 > 0) && (ExControlValue3 <= PRECISION)){
					FlashSave.flashSep.F_UP_LEVEL = ExControlValue3;
					}
					if((ExControlValue4 > 0) && (ExControlValue4 <= PRECISION)){
					FlashSave.flashSep.F_DOWN_LEVEL = ExControlValue4;
					}
				}
				else if(numCode == FOUR_MOTOR_NUMBER){//4
				maction = 10;
				zFlag = 1;
				m_data[0] = FOUR_MOTOR_NUMBER;
				m_data[5] = NOT_DISPLAY;		
				ExControlValue1 =  ((_pRecMsg[8]<< 8) |_pRecMsg[9]);
				ExControlValue2 =  ((_pRecMsg[10]<< 8) |_pRecMsg[11]);
					if((ExControlValue1 == 0xfefe) && ((ExControlValue2 == 0xfefe))){
					m_data[1] =  (FlashSave.flashSep.PosStepStick) >> 8;
					m_data[2] =  FlashSave.flashSep.PosStepStick;
					m_data[3] =  FlashSave.flashSep.posStepStickUp >> 8;
					m_data[4] =  FlashSave.flashSep.posStepStickUp;
					m_data[5] = 0x00;		
					}else{
					FlashSave.flashSep.PosStepStick = ExControlValue1;
					FlashSave.flashSep.posStepStickUp = ExControlValue2;
					}
				}
				else if(numCode == FIVE_MOTOR_NUMBER){//5
				maction = 10;
				zFlag = 1;
				m_data[0] = FIVE_MOTOR_NUMBER;
				m_data[5] = NOT_DISPLAY;		
				ExControlValue1 =  ((_pRecMsg[8]<< 8) |_pRecMsg[9]);
				ExControlValue2 =  ((_pRecMsg[10]<< 8) |_pRecMsg[11]);
					if((ExControlValue1 == 0xfefe) && ((ExControlValue2 == 0xfefe))){
					m_data[1] = FlashSave.flashSep.posBoxWaitIn >> 8;
					m_data[2] = FlashSave.flashSep.posBoxWaitIn;
					m_data[3] = FlashSave.flashSep.posBoxCheck >> 8;
					m_data[4] = FlashSave.flashSep.posBoxCheck;
					m_data[5] = 0x00;
					}else{
					FlashSave.flashSep.posBoxWaitIn = ExControlValue1;
					FlashSave.flashSep.posBoxCheck = ExControlValue2;
					}
				}
				else if(numCode == SIX_MOTOR_NUMBER){//6
				maction = 10;
				zFlag = 1;
				m_data[0] = SIX_MOTOR_NUMBER;
				m_data[5] = NOT_DISPLAY;	
				ExControlValue1 =  ((_pRecMsg[8]<< 8) |_pRecMsg[9]);
					if(ExControlValue1 == 0xfefe){
					m_data[1] = FlashSave.flashSep.posBoxInsert >> 8;
					m_data[2] = FlashSave.flashSep.posBoxInsert;
					m_data[5] = 0x00;	
					}else{
					FlashSave.flashSep.posBoxInsert = ExControlValue1;
					}
				}
				else if(numCode == ANALOG_DEBUG_IN){//0e
				maction = 11;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = ANALOG_DEBUG_IN;
				}
				else if(numCode == ANALOG_DEBUG_QUIT){//0f
				maction = 12;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =0;	
				m_data[0] = ANALOG_DEBUG_QUIT;	
				}
				else if(numCode == MAN_SEND_MEM_NUMBER){//手动发膜
				maction = 13;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = MAN_SEND_MEM_NUMBER;
				}
				else if(numCode == MAN_STICK_MEM_NUMBER){//手动粘膜
				maction = 14;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = MAN_STICK_MEM_NUMBER;
				}
				else if(numCode == MAN_TAKE_PAPER_NUMBER){//手动粘膜
				maction = 15;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = MAN_TAKE_PAPER_NUMBER;
				}
				else if(numCode == MAN_INSERT_PAPER_NUMBER){//手动塞纸
				maction = 16;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = MAN_INSERT_PAPER_NUMBER;
				}
				else if(numCode == MAN_FLASM_NUMBER){//手动塞纸
				maction = 17;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = MAN_FLASM_NUMBER;
				}
				else if(numCode == SET_IAP_FLAG){
				maction = 18;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = SET_IAP_FLAG;
				}
				else if(numCode == CLEAN_IAP_FLAG){
				maction = 19;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = CLEAN_IAP_FLAG;
				}
				else if(numCode == CHECK_WORKSPACE){
				maction = 22;
				zFlag = 1;
				z_certiConnect.zigHeartbeatOFF =1;	
				m_data[0] = CHECK_WORKSPACE;
				m_data[1] = WorkingSpaceValue;
				}
			ExternalControl(&ManualMotor,maction);
			break;
		case RECEIVE_FEEDBACK:
			break;
		case QUERY_FEEDBACK:
			break;
		case NODE_CONTROL_FEEDBACK:
			break;
		case NODE_QUERY:
    			zFlag = 1;
			break;
		case PROCESS_CONTROL:
			break;
		case PROCESS_CONTROL_FEEDBACK:
			break;
	}
}

/**
  * @brief  	
  * @param 解析
  * @retval
  */
static void CertificateControl_DecodeComMsg(u8 *_pRecMsg, u16 _len)
{
	// 	if(CertificateControl_CheckCRC(_pRecMsg, _len - 2, _pRecMsg[_len - 2], _pRecMsg[_len - 1]))
	//	return;//校验不正确不动作·
		certificateControl_handle(_pRecMsg, _len);//正确动作
}
 /**
  * @brief
  * @param
  * @retval None
  */
static void CertificateLine_sendComMsg(EQUIPMENT_TYPE _equipmentType, FUNCTION_CODE _functionCode, u8 _len, u8 *_pMsgInfo, SOURCE_TARGET_ADDRESS _addressSource, SOURCE_TARGET_ADDRESS _addressTarget, u8 packNum)
{
	u8 i, k = 0;
	u8 m_data[64];
  u16 CurVal=0;

	m_data[k++] = HEAD1;//A5
	m_data[k++] = HEAD2;//DB
	m_data[k++] = _len;//0x14 ..20u
	m_data[k++] = _equipmentType >> 8;//00
	m_data[k++] = _equipmentType & 0xFF;//03
	m_data[k++] = (u8)(_functionCode >> 8);//05
	m_data[k++] = (u8)(_functionCode & 0xFF);//02
	for(i = 0; i < _len - 14; i++) //_len总长，len-14 有效内容长
		m_data[k++] = _pMsgInfo[i];

	m_data[k++] = (u8)(_addressSource >> 8);//01
	m_data[k++] = (u8)(_addressSource & 0xFF);//01
	m_data[k++] = (u8)(_addressTarget >> 8);//00
	m_data[k++] = (u8)(_addressTarget & 0xFF);//01
	m_data[k++] = packNum;//01
	
	CurVal = CRC16((char *)m_data,_len);
	m_data[k++] = (u8)(CurVal >> 8);//41
	m_data[k++] = (u8)(CurVal & 0xFF);//E2

	CertificateCom_SendMessage(m_data, k);
}
  /**
  * @brief  	
  * @param 
  * @retval None
  */	

static void CertificateLine_ReceiveComMsg(u8 *_pMsgInfo, u32 _len)
{
	u32 i;

	for(i = 0; i < _len; i++)
	{
		switch(g_certificateCom.receiveStatus)
		{
			case REC_STATE_HEAD1:
				if(*_pMsgInfo == HEAD1){
					g_certificateCom.cmd[g_certificateCom.recOffset++] = *_pMsgInfo;
					g_certificateCom.receiveStatus = REC_STATE_HEAD2;
				}
				break;
			case REC_STATE_HEAD2:
				if(*_pMsgInfo == HEAD2){
					g_certificateCom.cmd[g_certificateCom.recOffset++] = *_pMsgInfo;
				  g_certificateCom.receiveStatus = REC_STATE_CMD_LEN;
				}
				else{
					g_certificateCom.recOffset = 0;
					g_certificateCom.receiveStatus = REC_STATE_HEAD1;
				}
				break;
			case REC_STATE_CMD_LEN:
				g_certificateCom.cmdLen = *_pMsgInfo;
				if(g_certificateCom.cmdLen > 20 || g_certificateCom.cmdLen <10){
					g_certificateCom.recOffset = 0;
					g_certificateCom.receiveStatus = REC_STATE_HEAD1;
					break;
				}
				g_certificateCom.cmd[g_certificateCom.recOffset++] = *_pMsgInfo;
				g_certificateCom.receiveStatus = REC_STATE_DATA;
				break;
			case REC_STATE_DATA:
				g_certificateCom.cmd[g_certificateCom.recOffset++] = *_pMsgInfo;
			 	if(g_certificateCom.recOffset >= g_certificateCom.cmdLen)
				{
					g_certificateCom.recOffset = 0;
					g_certificateCom.receiveStatus = REC_STATE_HEAD1;
					CertificateControl_DecodeComMsg(g_certificateCom.cmd, g_certificateCom.cmdLen);
					//	解析
				}
				break;
			default:
				break;
		}
		_pMsgInfo += 1;
	}
}

/** 
  * @brief
  * @param 发送心跳包
  * @retval None
  */
void heartBeatSend(u8 *data)
{
	CertificateLine_sendComMsg(NODE, UPLOAD_MESSAGE,20, data, MACHINE_THIRD_ADRESS, UPPER_ADDRESS,1);
}
/** 
  * @brief
  * @param 发送警告
  * @retval None
  */
void warningSend(u8 number,u8 status)
{
	u8 mdata[6];
	mdata[0] = number;
	if(status){
	mdata[1] = 0x02;
	}else{
	mdata[1] = 0x01;
	}
	CertificateLine_sendComMsg(NODE, UPLOAD_MESSAGE,20, mdata, MACHINE_THIRD_ADRESS, BROADCAST_ADRESS,1);

}
/** 
  * @brief
  * @param 发送故障重开
  * @retval None
  */
void 	faultResetSend(u8 number)
{
		u8 mdata[6];
	  mdata[0] = 0x17;
		mdata[1] = 0x02;
	
	CertificateLine_sendComMsg(NODE, UPLOAD_MESSAGE,20, mdata, MACHINE_THIRD_ADRESS, BROADCAST_ADRESS,1);
}

void analogSend(u16 ADCValue,u8 percent)
{
	u8 mdata[6];
	mdata[1] = (u8)(ADCValue >> 8);//01
	mdata[2] = (u8)(ADCValue & 0xFF);
	mdata[3] = percent;
	CertificateLine_sendComMsg(NODE, UPLOAD_MESSAGE,20, mdata, MACHINE_THIRD_ADRESS, UPPER_EXPAND_ADDRESS,1);
}

//-------------------------- @ external function --------------------------
/** CRJ 2018/6/14
  * @brief
  * @param 机器控制参数初始化
  * @retval None
  */
void certificateLineControl_Init(void)
{
//	g_certificateCom.equipmentType = NODE;
//	g_certificateCom.functionCode = NODE_QUERY;
//	g_certificateCom.lenZig = 6;
//	g_certificateCom.addressIn = MACHINE_FIRST_ADDRESS;//目标
//	g_certificateCom.addressOut = UPPER_ADDRESS;//上位机
	
	g_machine1.equipmentType = NODE;
	g_machine1.functionCode = NODE_QUERY;
	g_machine1.lenZig = 6;
	g_machine1.addressIn = MACHINE_FIRST_ADDRESS;
	g_machine1.addressOut = UPPER_ADDRESS;
	CertificateCom_RecMsgCallback(CertificateLine_ReceiveComMsg);
	CertificateCom_Init();
}

/**
  * @brief	3000ms scan
  * @param 
  * @retval
  */
void certificateLine_ChangeScan(void)
{
	CPU_SR_ALLOC();
  if(z_certiConnect.zigMachineBusy == 0){
		CertificateCom_Scan(); //扫描
	if(zFlag == 1){
	z_certiConnect.zigConnectStatus = ZIG_CONNECT_COMPLETE;
	OS_CRITICAL_ENTER();//进入临界区
	CertificateLine_sendComMsg(NODE, NODE_QUERY,20, m_data, MACHINE_THIRD_ADRESS, UPPER_ADDRESS,2);
	zFlag =0;
	OS_CRITICAL_EXIT();	
	}
}
	
}

void manualControl_ChangeScan(void)
{
		CPU_SR_ALLOC();
  if(z_certiConnect.zigMachineBusy == 0){
		CertificateCom_Scan(); //扫描
			if(zFlag == 1){
			z_certiConnect.zigConnectStatus = ZIG_CONNECT_COMPLETE;
	OS_CRITICAL_ENTER();//进入临界区
			CertificateLine_sendComMsg(NODE, NODE_QUERY,20, m_data, MACHINE_THIRD_ADRESS, UPPER_ADDRESS,1);
			zFlag =0;
	OS_CRITICAL_EXIT();	
		}
}
	
}

void manualControl_Send(u8* manualData)
{
	CertificateLine_sendComMsg(NODE, UPLOAD_MESSAGE,20, manualData, MACHINE_THIRD_ADRESS, UPPER_ADDRESS,1);	
}
