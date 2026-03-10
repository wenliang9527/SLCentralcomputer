#include "usermain.h"


#define CYCTIME     6       //周期时间s
  #define R1_00     4       //周期时间s
  #define R1_SS     (9+R1_00)       //周期时间s
  #define R1_R2     (31+R1_00)      //周期时间s
	
	#define CD_SSNo 28  //样本盘扫描补偿
	#define CD_R1No 15  //R1盘扫描补偿
	#define CD_R2No 0   //R2盘扫描补偿
	#define AB_SSNo 0   //样本盘暂停位置补偿
	#define AB_R1No 0   //R1盘暂停位置补偿
	#define AB_R2No 0   //R2盘暂停位置补偿
	#define REAGNo  70   //试剂盘一圈数量


TSyscommand Syscommand;
TCommSentList TXD_UpBufList;
cyc41instruction  cyc_41instru={0x00};
		//			 SysCycin SysCycbuf;
			//	    STCycin STCycbuf;
         ItemTParam TestItem[ITEMNUMBER];			//	 TItemParam ItemParam;
	//			  TRealTime RealTime;        //错误列表
// SaveItemResultList TADList;
        //    Results ADdata;
          TByteYear Timeday;
          u16 dddd=0;
          u8 cupid=0;
TCommDataPacket buff2;

TSampleBuf SampleBuf; 

  bufusart outbufusart4;
  DirtyCup ErrSout;//

bool QCmodeCMD=true;
bool QCcmdbreak=false;
u32  QCCMDautotime=0;
TWordRec QCsendCMD;



static u8 InvertUint8(u8 data)
{
  u16 i;
  u8 newtemp8 = 0;
  for (i = 0; i < 8; i++)
  {
    if ( (data & (1 << i) ) != 0)
			newtemp8 |= (u8)(1 << (7 - i));
  }
  return newtemp8;
}
///////////////////////////////////////////////////
static u16 InvertUint16(u16 data)
{
  u16 i;
  u16 newtemp16 = 0;
  for (i = 0; i < 16; i++)
  {
    if ( (data & (1 << i) ) != 0)
			newtemp16 |= (u16)(1 << (15 - i));
  }
  return newtemp16;
}
//////////////////////////////////////////////

u16 CRC16_MODBUS(u8* data, int lenth)
{
  u16 i;
  u16 wCRCin = 0xFFFF;
  u16 wCPoly = 0x8005;
  u16 wChar = 0;
  while (lenth > 0)
  {
    wChar = *data;
    data++;
    wChar = InvertUint8( (u8)wChar);
    wCRCin ^= (u16)(wChar << 8);
    for (i = 0; i < 8; i++)
    {
      if ((wCRCin & 0x8000) != 0)
      {
        wCRCin = (u16)( (wCRCin << 1) ^ wCPoly);
      }else
      {
        wCRCin = (u16)(wCRCin << 1);
      }
    }
    lenth=lenth-1;
  }
  wCRCin = InvertUint16(wCRCin);
  return (wCRCin);
}

void Out_DataLise(TCommDataPacket buf)//上传数据列队
{
	if(Syscommand.Barcode)
	{
		if(buf.FrameBuf[3]==0x05 || buf.FrameBuf[3]==0x02)
			{
			if(buf.FrameBuf[4]==0x00&&buf.FrameBuf[5]==0xcd)
				{
				if(TXD_UpBufList.W_Point<Queuelen)
					{
						TXD_UpBufList.Field[TXD_UpBufList.W_Point]=buf;
						TXD_UpBufList.W_Point++;
					}
			}
		}
	}else
	{
		if(TXD_UpBufList.W_Point<Queuelen)
			 TXD_UpBufList.Field[TXD_UpBufList.W_Point]=buf;
			 TXD_UpBufList.W_Point++;
	}


  if(TXD_UpBufList.W_Point>=Queuelen)
		{
			TXD_UpBufList.W_Point=0;
	  }
}

void uart_send_byte_it(TCommDataPacket buf)//
{
	if(THeartbuf.Type==0)
	{
		if(buf.FrameBuf[3]==0x02)
			{
				if(buf.FrameBuf[buf.FramLen-2])
				{
					ScreencuperrdataEsc(0,buf.FrameBuf[buf.FramLen-2]); //数据上传给串口屏
				}	
			}else if(buf.FrameBuf[3]!=0x01)
			{
		    SCREEN_DataLise(buf);        //数据上传给串口屏
			}
	}
	if(THeartbuf.Type==1&&buf.FrameBuf[3]!=0x04)
	{
		if(buf.FrameBuf[3]==0x03)
		{
			if(!ErrSout.sentout)//未连接上位机前，有错误信息的时候写入列表，最大48条，上电开机自检时的错误保存
			{
				ErrSout.Errid[ErrSout.Errpoint].Err2=buf.FrameBuf[buf.FramLen-2];
				ErrSout.Errid[ErrSout.Errpoint].Err1=buf.FrameBuf[buf.FramLen-3];
				ErrSout.Errpoint++;
				if(ErrSout.Errpoint>48)ErrSout.Errpoint=0;
			}
			if(buf.FrameBuf[buf.FramLen-2])
			{
				ScreencuperrdataEsc(buf.FrameBuf[buf.FramLen-5],buf.FrameBuf[buf.FramLen-2]); //数据上传给串口屏
			}else if(buf.FrameBuf[buf.FramLen-3])
			{
				ScreencuperrdataEsc(buf.FrameBuf[buf.FramLen-6],buf.FrameBuf[buf.FramLen-3]); //数据上传给串口屏
			}else if(buf.FrameBuf[buf.FramLen-4])
			{
				ScreencuperrdataEsc(buf.FrameBuf[buf.FramLen-7],buf.FrameBuf[buf.FramLen-4]); //数据上传给串口屏
			}
		}else
		{
		   SCREEN_DataLise(buf);        //数据上传给串口屏
		}
	}

	
	switch(Syscommand.UserNumber)
	{
		case 0: 
		        PC232_Usart5(buf);  //数据上传给Pc232
		        USART1_DataLise(buf); //数据上传给usb
            USART8_Usart485(buf); //数据上传给rj45网口
		        
		     break;
		case 1: 
				   USART1_DataLise(buf);  //数据上传给USB
					PC232_Usart5(buf);  //数据上传给Pc232
		     break;
		case 2: 

		        PC232_Usart5(buf);          //数据上传给Pc232
		        USART1_DataLise(buf);            //数据上传给usb
            USART8_Usart485(buf);       //数据上传给rj45网口
			  break;
		case 3: 

			break;
		case 5: 
		     PC232_Usart5(buf);  //数据上传给Pc
					 USART1_DataLise(buf);  //数据上传给USB
			break;
		case 8: 
          USART8_Usart485(buf); //数据上传给rj45网口

			break;
	}
}

void SysErrEsc(void)//数据返回
{
	TCommDataPacket buf;
	  buf.FramLen=9;
		buf.FrameBuf[0]=0x53;
		buf.FrameBuf[1]=0x48;
		buf.FrameBuf[2]=5;
		buf.FrameBuf[3]=0x03;
		buf.FrameBuf[4]=Syscommand.SysHByte;               //
	  buf.FrameBuf[5]=Syscommand.SysLByte;               //
	  buf.FrameBuf[6]=0;               //
	  buf.FrameBuf[7]=Syscommand.SysErr;            //
	  
	  buf.FrameBuf[8]=SenloSum(buf);
	  Syscommand.SysErr=0;
		SCREEN_DataLise(buf); //数据上传给串口屏
} 

void ScreencuperrdataEsc(u8 Err1,u8 Err2)//数据返回 //出错
{
	TCommDataPacket buf;
	  buf.FramLen=9;
		buf.FrameBuf[0]=0x53;
		buf.FrameBuf[1]=0x48;
		buf.FrameBuf[2]=5;
		buf.FrameBuf[3]=0x03;
		buf.FrameBuf[4]=Syscommand.SysHByte;               //
	  buf.FrameBuf[5]=Syscommand.SysLByte;               //
	  buf.FrameBuf[6]=Err1;               // //出错
	  buf.FrameBuf[7]=Err2;            //
	  buf.FrameBuf[8]=SenloSum(buf);
	  buf.UserID=0;
		SCREEN_DataLise(buf); //
} 




void Screen_dataEsc(TCommDataPacket buf,u8 Type,u8 Err1,u8 Err2)//数据返回
{
		buf.FramLen=9;
		buf.FrameBuf[2]=0x05;
		buf.FrameBuf[3]=Type;
		buf.FrameBuf[6]=Err1;         // 出错类型
		buf.FrameBuf[7]=Err2;         //出错
		buf.FrameBuf[8]=SenloSum(buf);
	  Out_DataLise(buf);//数据上传
} 

void Sent_0113_Commd(TCommDataPacket buf)
{
	u16 iWord;
		buf.FramLen=15;
		buf.FrameBuf[2]=0x05;
		buf.FrameBuf[3]=0x02;
		iWord=CRC16_MODBUS(&buf.FrameBuf[0],6);   //  CRC16校验  
		buf.FrameBuf[6]=iWord>>8;
		buf.FrameBuf[7]=iWord;
		buf.FrameBuf[8]=0;
		buf.FrameBuf[9]=0;
		buf.FrameBuf[10]=0;
		buf.FrameBuf[11]=0;
		buf.FrameBuf[12]=0x11;
	  iWord=CRC16_MODBUS(&buf.FrameBuf[8],buf.FrameBuf[2]);  //  CRC16校验  
		buf.FrameBuf[13]=iWord>>8;
		buf.FrameBuf[14]=iWord;
	  Out_DataLise(buf);//数据上传
}

void TimeHeartbuf(void )//心跳包，每10秒发送一次
{
	TCommDataPacket buf; 
//    it_ADCProcessSampleData();  //
	  
	  buf.FramLen=19;
		buf.FrameBuf[0]=0x53;
		buf.FrameBuf[1]=0x48;
		buf.FrameBuf[2]=15;
		buf.FrameBuf[3]=0x02;
		buf.FrameBuf[4]=0x00;
	  buf.FrameBuf[5]=0x4d;
		buf.FrameBuf[6]=THeartbuf.Type;                               //机器工作状态
		buf.FrameBuf[7]=THeartbuf.State;                              //机器测试状态
		buf.FrameBuf[8]=THeartbuf.sampleloc;                                //预留
	  
//    THeartbuf.HomeTemp.Value=0;
//		if(THeartbuf.ColdTemp.Value==0xffff)
//		{
//      THeartbuf.ColdTemp.Value=0;
//		}
//	Pump_off;
	 
	 buf.FrameBuf[10]=THeartbuf.ColdTemp.Value*TEMPFACTOR;          //冷藏室温度    12


	//	if(buf.FrameBuf[10]>2)buf.FrameBuf[10]-=2;
	  
//	  buf.FrameBuf[11]=(THeartbuf.ReacTemp.HighByte<<4)|(THeartbuf.ReacTemp.LowByte>>4);
//	  buf.FrameBuf[12]=THeartbuf.ReacTemp.LowByte&0x0f;                  //反应盘温度低字节  *0.0625=℃
	//  THeartbuf.ReacTemp.Value=0x258;
	  buf.FrameBuf[11]=THeartbuf.ReacTemp.HighByte;                 //反应盘温度高字节    37
	  buf.FrameBuf[12]=THeartbuf.ReacTemp.LowByte;                  //反应盘温度低字节  *0.0625=℃
		buf.FrameBuf[13]=THeartbuf.Pressure1.Value;                   //冲洗高压压力
		buf.FrameBuf[14]=THeartbuf.Pressure2.Value;                   //冲洗低压压力
	  buf.FrameBuf[9]=IO_pD04;                     //
		buf.FrameBuf[15]=IO_pD05;                   //压力
		buf.FrameBuf[16]=IO_pD06;
		buf.FrameBuf[17]=IO_pD07;
//		if(!IO_pD04&IO_pD05&IO_pD06&IO_pD07)
//					  InsideSwitch(0x2fffff,1);   //，蜂鸣器控制响一声
		buf.FrameBuf[18]=SenloSum(buf);
	  buf.UserID=0;
//	  Out_DataLise(buf);//数据上传
		//PC232_Usart5(buf);  //数据上传给Pc232
	//	SCREEN_DataLise(buf); //数据上传给串口屏
	SCREEN_DataLise(buf); //数据上传给串口屏
}
void QCCMD_break1( TWordRec CMD)
{
	TCommDataPacket buf;
	buf.FramLen =9;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = 0x05;
	buf.FrameBuf[3] = 0x01;
  buf.FrameBuf[4] = CMD.HighByte;
  buf.FrameBuf[5] = CMD.LowByte;
  buf.FrameBuf[6] = 0x00;
	buf.FrameBuf[7] = 0x00;
	buf.FrameBuf[8]=SenloSum(buf);
	PC232_Usart5(buf);
  USART8_Usart485(buf);
	USART1_DataLise(buf);
	QCCMDautotime=0;
	QCcmdbreak=true;
}
void QCCMD_break2( TWordRec CMD)
{
	TCommDataPacket buf;
	buf.FramLen =9;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = 0x05;
	buf.FrameBuf[3] = 0x02;
  buf.FrameBuf[4] = CMD.HighByte;
  buf.FrameBuf[5] = CMD.LowByte;
  buf.FrameBuf[6] = 0x00;
	buf.FrameBuf[7] = 0x00;
	buf.FrameBuf[8]=SenloSum(buf);
	PC232_Usart5(buf);
  USART8_Usart485(buf);
	USART1_DataLise(buf);
}
void ItemADdata(TCommDataPacket inbuf)//单个项目ID查询AD
{
	u8 i;
	u8 cup=inbuf.FrameBuf[6];
	TCommDataPacket buf;
	Results AD;
	
//	   SpiFlash_Read((void *)&TADList, W25QITEMID, sizeof(TADList));
	   buf.UserID=inbuf.UserID;
//     AD=TADList.Cup_AD[cup-1];
   	 for(i=0;i<32;i++)
	   {
				buf.FramLen=22;
				buf.FrameBuf[0]=0x53;
				buf.FrameBuf[1]=0x48;
				buf.FrameBuf[2]=18;
				buf.FrameBuf[3]=0x04;
				buf.FrameBuf[4]=0x00;
				buf.FrameBuf[5]=0xee;
				buf.FrameBuf[16]=cup;
				buf.FrameBuf[17]=AD.TestType;
				buf.FrameBuf[18]=i+1;
				buf.FrameBuf[19]=AD.WaveLen;
				buf.FrameBuf[20]=AD.Endpoint;
				buf.FrameBuf[21]=SenloSum(buf);
				Out_DataLise(buf);//数据上传
				Delay_ms(10);
		 }
}
void TimeVerSyc(u16 iword) //版本号
{
	TCommDataPacket buf;
	buf.FramLen=12;
	buf.FrameBuf[0]=0x53;
	buf.FrameBuf[1]=0x48;
	buf.FrameBuf[2]=0x08;
	buf.FrameBuf[3]=0x02;
	buf.FrameBuf[4]=iword>>8;
	buf.FrameBuf[5]=iword;
	buf.FrameBuf[6]=SysSave.Unit403;   //主控403版本号
	buf.FrameBuf[7]=SysSave.Unit103;   //主控103版本号
	buf.FrameBuf[8]=SysSave.Unit579;   //网口579版本号
	buf.FrameBuf[9]=0;                 //预留版本号位
	buf.FrameBuf[10]=0;                //预留版本号位
	buf.FrameBuf[11]=SenloSum(buf);
//	buf.UserID=Syscommand.UserNumber;
	Out_DataLise(buf);//数据上传
}


void Screen_AD(void)
{
  TCommDataPacket buf;
	buf.FramLen =7;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = 0x03;
	buf.FrameBuf[3] = 0x00;
  buf.FrameBuf[4] = 0x73;
  buf.FrameBuf[5] = 0x00;
	buf.FrameBuf[6]=SenloSum(buf);
	Usart4out(buf);
}



void it_CoreProcessor1ms(void)
{
	if(outbufusart4.outoff==false)
		{
			outbufusart4.timeouttx++;
		}

	if(Usa232Head.Time1ms)Usa232Head.Time1ms--;

		if(QCcmdbreak)
			{
			 QCCMDautotime++;
			if(QCCMDautotime>2000)
				{
					QCCMDautotime=0;
					QCcmdbreak=false;
					QCCMD_break2(QCsendCMD);
				}
			}
	if(ScreenHead.Time1ms)
			{
				ScreenHead.Time1ms--;
			}
		if(USART3j45Head.Time1ms)
			{
				USART3j45Head.Time1ms--;
			}
		if(Usart1UsbHead.Time1ms)
			{
				Usart1UsbHead.Time1ms--;
			}	
	Sensor_detection();   //传感器检测
	
	if(Syscommand.TimeErr)
	{
		Syscommand.TimeErr--;
	  if(Syscommand.TimeErr==1)
		{
			Syscommand.Systray=88;
	    Syscommand.SysErr=128;
		}
	}
	if(Syscommand.TimeErr2)
	{
		Syscommand.TimeErr2--;
	  if(Syscommand.TimeErr2==1)
		{
//			Syscommand.Bar02code=true ;
	    Syscommand.SysErr=129;
		}
	}
	
	if(Syscommand.AD7308inst)
	{
		Syscommand.flag7308AD++;
		if(Syscommand.flag7308AD>=1000)
		{
	//		Screen_AD();
		}
	}
	
	Syscommand.Time5s++;
	if(Syscommand.Time5s>=5000)
	{
		Syscommand.Time5s =0;
		Syscommand.TimeHeart=true ;
		SENsotimeInterval++;
	if(SENsotimeInterval>=3)
		{
			SENsotimeInterval=0;
		if(Syscommand.SysErr)
			{
				SysErrEsc();//数据返回
			}
		}
	
	}
}

void DataUpTXDout(void)   //数据上传发送
{
//  u16 i;
	TCommDataPacket buf;
	
	if(TXD_UpBufList.W_Point!=TXD_UpBufList.R_Point&&!Syscommand.Stup_pc232&&!Syscommand.Stup_usb&&!Syscommand.Stup_rj45)
	{
		
      buf=TXD_UpBufList.Field[TXD_UpBufList.R_Point];
      TXD_UpBufList.R_Point++;
			uart_send_byte_it(buf);      //数据上传给对应的终端

		Usa232Head.Time1ms=50;
		if(TXD_UpBufList.R_Point>=Queuelen)
			{
				 TXD_UpBufList.R_Point=0;
			}
	}
}

void ItemID_List(TCommDataPacket buf)//单个项目基本参数
{
		ItemTParam Item;
    u8 ItemID,i; 
	ItemID=buf.FrameBuf[6];
	Item.SampType=buf.FrameBuf[7];
	Item.AnalysisType=(TAnalysisType)buf.FrameBuf[8];
	Item.CalType=buf.FrameBuf[9];
	Item.WaveLen=buf.FrameBuf[10];
	Item.ResultPreci=buf.FrameBuf[11];
	Item.Readtime=buf.FrameBuf[12];
	Item.ResulUnit=(TResultUnit)buf.FrameBuf[13];
	
	Item.CalibParam[0].CalibSolvalue.HighByte=buf.FrameBuf[14];
	Item.CalibParam[0].CalibSolvalue.LowByte=buf.FrameBuf[15];
	Item.CalibParam[0].CalibFactor.Byte1=buf.FrameBuf[16];
	Item.CalibParam[0].CalibFactor.Byte2=buf.FrameBuf[17];
	Item.CalibParam[0].CalibFactor.Byte3=buf.FrameBuf[18];
	Item.CalibParam[0].CalibFactor.Byte4=buf.FrameBuf[19];
	Item.CalibParam[0].bFactor=buf.FrameBuf[20];
	Item.Range.MinValue.HighByte=buf.FrameBuf[21];
	Item.Range.MinValue.LowByte=buf.FrameBuf[22];
	Item.LinearityRange.MinValue.HighByte=buf.FrameBuf[23];
	Item.LinearityRange.MinValue.LowByte=buf.FrameBuf[24];
//	buf.FrameBuf[25];   预
//	buf.FrameBuf[26];   预
	for(i=0;i<NAMESOFONE;i++)
	{
		Item.ItemName[i]=buf.FrameBuf[27+i];
	}
  Item.activation=true ;
  TestItem[ItemID-1]=Item;
	SpiFlash_Write((void *)&TestItem, W25QADDR1, sizeof(ItemTParam));
}


void EscItemIDparameter(u8 ItemID)////单个项目上传 
{
	TCommDataPacket buf;
	     ItemTParam Item;
               u8 i; 
	Item=TestItem[ItemID-1];

	  buf.FramLen=36;
		buf.FrameBuf[0]=0x53;
		buf.FrameBuf[1]=0x48;
		buf.FrameBuf[2]=buf.FramLen-4;
		buf.FrameBuf[3]=0x02;
		buf.FrameBuf[4]=0x00;                                        //
	  buf.FrameBuf[5]=0xf4;                                        //
	  buf.FrameBuf[6]=ItemID;                                      //
	  buf.FrameBuf[7]=Item.SampType;                               //样本类型
		buf.FrameBuf[8]=Item.AnalysisType;                           //分析类型  分析方法
		buf.FrameBuf[9]=Item.CalType;                                //校准类型 0=1点校准，1=5点校准，2=Spine
		buf.FrameBuf[10]=Item.WaveLen;                               //波长
		buf.FrameBuf[11]=Item.ResultPreci;                           //结果精度
		buf.FrameBuf[12]=Item.Readtime;                              //读数时间S
		buf.FrameBuf[13]=Item.ResulUnit;                             //结果单位
		buf.FrameBuf[15]=Item.CalibParam[0].CalibSolvalue.HighByte;  //定标液值
		buf.FrameBuf[14]=Item.CalibParam[0].CalibSolvalue.LowByte;   //定标液值
		buf.FrameBuf[16]=Item.CalibParam[0].CalibFactor.Byte1;       //定标F因子
		buf.FrameBuf[17]=Item.CalibParam[0].CalibFactor.Byte2;       //定标F因子
		buf.FrameBuf[18]=Item.CalibParam[0].CalibFactor.Byte3;       //定标F因子
		buf.FrameBuf[19]=Item.CalibParam[0].CalibFactor.Byte4;       //定标F因子
		buf.FrameBuf[20]=Item.CalibParam[0].bFactor;                 //b因子
		buf.FrameBuf[22]=Item.Range.MinValue.HighByte;               //正常值范围   
		buf.FrameBuf[21]=Item.Range.MinValue.LowByte;                //正常值范围   
		buf.FrameBuf[24]=Item.LinearityRange.MinValue.HighByte;      //线性范围  
		buf.FrameBuf[23]=Item.LinearityRange.MinValue.LowByte;       //线性范围  
		buf.FrameBuf[25]=0x00;                                       // 预
		buf.FrameBuf[26]=0x00;                                       // 预
		for(i=0;i<NAMESOFONE;i++)                                    
		{
			buf.FrameBuf[27+i]=Item.ItemName[i];                       //名称
		}
    buf.FrameBuf[35]=SenloSum(buf);
	  buf.UserID=0;
	  Out_DataLise(buf);//数据上传
}




void ItemNameIDer(u8 ItemID)////项目ID名称上传 
{
	TCommDataPacket buf;
	     ItemTParam Item;
               u8 i; 
	Item=TestItem[ItemID];

	  buf.FramLen=16;
		buf.FrameBuf[0]=0x53;
		buf.FrameBuf[1]=0x48;
		buf.FrameBuf[2]=buf.FramLen-4;
		buf.FrameBuf[3]=0x02;
		buf.FrameBuf[4]=0x00;                                        //
	  buf.FrameBuf[5]=0xf5;                                        //
	  buf.FrameBuf[6]=ItemID+1;                                      //
		for(i=0;i<NAMESOFONE;i++)                                    
		{
			buf.FrameBuf[7+i]=Item.ItemName[i];                       //名称
		}
    buf.FrameBuf[15]=SenloSum(buf);
	  buf.UserID=0;
	  Out_DataLise(buf);//数据上传

}

void Esc0201info(u8 type,u8 err)
{
  TCommDataPacket buf;
	buf.FramLen =9;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = 0x05;
	buf.FrameBuf[3] = type;
  buf.FrameBuf[4] = Syscommand.SysHByte;
  buf.FrameBuf[5] = Syscommand.SysLByte;
	buf.FrameBuf[6] = 0;
	buf.FrameBuf[7] = err;
	buf.FrameBuf[8]=SenloSum(buf);
	Out_DataLise(buf);//数据上传
}




void TimeSyc(TCommDataPacket buff) //时间同步
{
	 TCommDataPacket buf;
//	Esc0201info(1,0);
 
	buf.FramLen=14;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = 0x0a; 
  buf.FrameBuf[3] = 0x00;
  buf.FrameBuf[4] = 0x01;
  buf.FrameBuf[5] = 0x07;
  buf.FrameBuf[6] = buff.FrameBuf[6];//年
  buf.FrameBuf[7] = buff.FrameBuf[7];//年
  buf.FrameBuf[8] = buff.FrameBuf[8];//月
  buf.FrameBuf[9] = buff.FrameBuf[9];//日
  buf.FrameBuf[10] = buff.FrameBuf[10];//时
  buf.FrameBuf[11] = buff.FrameBuf[11];//分
  buf.FrameBuf[12] = buff.FrameBuf[12];//秒
	buf.FrameBuf[13]=SenloSum(buf);
	SCREEN_DataLise(buf);
	Esc0201info(2,0);
	//TimeVerSyc(0x0107);//数据上传
	
}

void Itemco232ter(TCommDataPacket buf)////单个项目上传 
{
// u8 i,mm=0; 
//	Esc0201info(1,0);
	SCREEN_DataLise(buf);
	Esc0201info(2,0);

}


void ItemName232ter(TCommDataPacket buf)////单个项目上传 
{
// u8 i,mm=0; 
//	Esc0201info(1,0);
	SCREEN_DataLise(buf);
	Esc0201info(2,0);
		

}
void ItemNameter(u8 page)////单个项目上传 
{
 u8 i,mm=0; 
	
	
	for(i=0;i<ITEMNUMBER;i++)
	 {
		 if(TestItem[i].activation)
		 {
		   mm++;
       if(page==1)
			 {
			    ItemNameIDer(i);////单个项目上传 
			    Delay_ms(10);

				 if(mm>=25)break ;
			 }else
			 {
				 if(mm>=25)
				 {
			    ItemNameIDer(i);////单个项目上传 
			    Delay_ms(10);
				 }
			 }
		 }
	 }	

}

void Bar3102_code(u8 SysHByte,u8 SysLByte,u8 loc)// 转盘
{
  TCommDataPacket buf;
	buf.FramLen =10;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = 0x06;
	buf.FrameBuf[3] = 0x00;
  buf.FrameBuf[4] = SysHByte;
  buf.FrameBuf[5] = SysLByte;
	buf.FrameBuf[6] = 0x00;
	buf.FrameBuf[7] = 0x00;
	buf.FrameBuf[8] = loc;
	buf.FrameBuf[9]=SenloSum(buf);
	Usart4out(buf);
}

void Esc02cd(u8 SysHByte,u8 SysLByte)
{
  TCommDataPacket buf;
	buf.FramLen =10;
  buf.FrameBuf[0] = 0x53;
  buf.FrameBuf[1] = 0x48;
  buf.FrameBuf[2] = buf.FramLen-4;
	buf.FrameBuf[3] = 0x02;
  buf.FrameBuf[4] = SysHByte;
  buf.FrameBuf[5] = SysLByte;
  buf.FrameBuf[6] = Syscommand.SysErr;
  Syscommand.SysErr=0;
	buf.FrameBuf[7] = 0;
  buf.FrameBuf[8] = 0;
	buf.FrameBuf[9]=SenloSum(buf);


	//PC232_Usart5(buf);//数据上传
	Out_DataLise(buf);//数据上传
}


void Bar_code(void)// 条码扫描跟踪
{
	u8 loc; 
  if(!Syscommand.Barcode)	
	 return ;
	
	if(Syscommand.Systray)
	{
			Syscommand.tray=buff2.FrameBuf[6]&0xf0;    //盘号
			Syscommand.loc=buff2.FrameBuf[7];     //位号
			
		
		   switch(Syscommand.tray)
				 {
					 case 0x10:{
											switch(Syscommand.Systray)
											{
												case 1:{
																	Syscommand.Systray=2;
																	loc=Syscommand.loc+CD_R1No;
													        if(loc>70)loc=loc-REAGNo;
													        Bar3102_code(0x31,0xCD,loc);
																	Syscommand.Bar02code=false;
																	Syscommand.TimeErr2=5000;
												       } break;  						
												case 2:{
																	if(Syscommand.Bar02code)
																	{
																		Syscommand.Bar02code=false;
																	  Syscommand.TimeErr2=0;
																		Syscommand.Systray=3;
																		USART3_DataLise(buff2);
																	   Syscommand.Bar00cdcode=false;
																	   Syscommand.Bar03cdcode=false;
																		 Syscommand.TimeErr=3000;
																	}
															 } break;  
												case 3:{
																	 if(Syscommand.Bar03cdcode)  // 扫码接收命令返回
																	 {
	                                 //   Syscommand.SysErr=128;
																		  Syscommand.TimeErr=2000;
																		  Syscommand.Systray=4;
																		  Syscommand.Bar03cdcode=false;
																	 }
															 } break; 
												case 4:{
																	 if(Syscommand.Bar00cdcode)  // 扫码接收命令返回
																	 {
	                                   Syscommand.SysErr=0;
																		 Syscommand.TimeErr=0;
																		 Syscommand.Bar00cdcode=false;
																	   Syscommand.Systray=88;
																	 }
															 } break;  
												case 88:{
																	
																	   Syscommand.Systray=0;
													           Esc02cd(0x00,0xcd);
															 } break;  						
											}
									} break;   						
					 case 0x20:{
											switch(Syscommand.Systray)
											{
												case 1:{
																	Syscommand.Systray++;
																	Bar3102_code(0x21,0xCD,Syscommand.loc);
																	Syscommand.Bar02code=false;
																	Syscommand.TimeErr2=5000;
												       } break;  						
												case 2:{
																	if(Syscommand.Bar02code)
																	{
																		Syscommand.Bar02code=false;
																		USART3_DataLise(buff2);   
                                    Syscommand.TimeErr=3000;																		
																	}else
																	{
																	 if(Syscommand.Bar00cdcode)
																	 {
																		Syscommand.Bar00cdcode=false;
																		Esc02cd(0x00,0xcd);
//																		Syscommand.Barcode=false;
//																	  Syscommand.Systray=0;
																	 }
																	}
															 } break;  						
											}
									} break;   						
					 case 0x50:{
											switch(Syscommand.Systray)
											{
												case 1:{
																	Syscommand.Systray++;
																	loc=Syscommand.loc+CD_SSNo;
													        if(Syscommand.loc>40)
																	{
																		loc=loc-1;
													          if(loc>80)loc=loc-40;
																	}else
																	{
													          if(loc>40)loc=loc-40;
																	}
													
													        Bar3102_code(0x51,0xCD,loc);
																	Syscommand.Bar02code=false;
																	Syscommand.TimeErr2=5000;
													// Syscommand.Bar02code=true;
												} break;  						
												case 2:{
																	if(Syscommand.Bar02code)
																	{
																		Syscommand.Bar02code=false;
																	  Syscommand.TimeErr2=0;
																		Syscommand.Systray++;
																		USART3_DataLise(buff2);
																	   Syscommand.Bar00cdcode=false;
																	   Syscommand.Bar03cdcode=false;
																		 Syscommand.TimeErr=3500;
																	}
															 } break;  						
												case 3:{
																	
																	 if(Syscommand.Bar03cdcode)  // 扫码接收命令返回
																	 {
	                                 //   Syscommand.SysErr=128;
																		  Syscommand.TimeErr=2000;
																		  Syscommand.Systray++;
																		  Syscommand.Bar03cdcode=false;
																	 }
															 } break;  						
												case 4:{
																	 if(Syscommand.Bar00cdcode)  // 扫码接收命令返回
																	 {
	                                   Syscommand.SysErr=0;
																		 Syscommand.TimeErr=0;
																		 Syscommand.Bar00cdcode=false;
																	   Syscommand.Systray=88;
																	 }
															 } break;  						
												case 88:{
																	Syscommand.Systray=0;
													        Esc02cd(0x00,0xcd);
															 } break;  						
											}
									} break;   						
				 
					 default :
					  {
											switch(Syscommand.Systray)
											{
												case 1:{
																	Syscommand.Systray++;
																	Bar3102_code(0x51,0xCD,Syscommand.loc);
																	Syscommand.Bar02code=false;
															 } break;  						
												case 2:{
																	if(Syscommand.Bar02code)
																	{
																		Syscommand.Bar02code=false;
																		USART3_DataLise(buff2);                                                  
																	}else
																	{
																	 if(Syscommand.Bar00cdcode)
																	 {
																		Syscommand.Bar00cdcode=false;
																		Esc02cd(0x00,0xcd);
//																		Syscommand.Barcode=false;
//																	  Syscommand.Systray=0;
																	 }
																	}
															 } break;  						
											}
					  }break;
				 
				 
				 }
		
	}
}

//////
void CycCommdPacketParse(TCommDataPacket inbuf)
{
	TSamBuf buf;
  u8 i;
	
	buf.len=0;
	if(inbuf.FrameBuf[2]>6)
	{
	 buf.len=inbuf.FrameBuf[2]-6;
	}
	buf.Type=inbuf.FrameBuf[6];
	buf.SampleID.HighByte=inbuf.FrameBuf[7];
	buf.SampleID.LowByte=inbuf.FrameBuf[8];
	for(i=0;i<buf.len;i++)
	{
	 buf.Itembuf[i]=inbuf.FrameBuf[i+9];
	}
	SampleBuf.SamBuf[SampleBuf.bufPoint]=buf;
	SampleBuf.buflen++;
	SampleBuf.bufPoint++;
	if(SampleBuf.bufPoint>=SAMPLELEN)
	{
		SampleBuf.bufPoint=0;
	}
}

void Lisebufcache(TCommDataPacket buf)//指令缓存
{
	cyc41_instruc buff;
	
	buff.R1Cup=buf.FrameBuf[5];
	buff.R1_HVol=buf.FrameBuf[6];
	buff.R1_LVol=buf.FrameBuf[7];
	buff.SS_LVol=buf.FrameBuf[8];
	buff.R1_Loc=buf.FrameBuf[9];
	buff.SS_Loc=buf.FrameBuf[10];
	buff.R2_Loc=buf.FrameBuf[11];
	buff.SS_HVol=buf.FrameBuf[12];
	buff.R2_Vol=buf.FrameBuf[13];
//	buff.Type12=buf.FrameBuf[14];
//	buff.Type12=buf.FrameBuf[15];
	buff.Type12=buf.FrameBuf[16];
	buff.Type13=buf.FrameBuf[17];
	buff.Dilute14=buf.FrameBuf[18];
	buff.Wave=buf.FrameBuf[19];
  cyc_41instru.cycbuf[cyc_41instru.point]=buff;
	
  cyc_41instru.point++;
}

void pausetime(void)//试剂样本盘暂停时间给显示器
{
  TWordRec word;
	TCommDataPacket buf;
	if(cyc_41instru.hByte==0x11)
	{
	  cyc_41instru.hByte=0x31;
		word.Value=	R1_00*CYCTIME;
	}else if(cyc_41instru.hByte==0x51)
	{
	  word.Value=	R1_SS*CYCTIME;
	}else if(cyc_41instru.hByte==0x21)
	{
	  word.Value=	R1_R2*CYCTIME;
	}
  buf.FrameBuf[0]=0x53;
  buf.FrameBuf[1]=0x48;
  buf.FrameBuf[2]=0x05;
  buf.FrameBuf[3]=0x02;
  buf.FrameBuf[4]=0x00;
  buf.FrameBuf[5]=0xab;
  buf.FrameBuf[6]=word.HighByte;
  buf.FrameBuf[7]=word.LowByte;
	buf.FramLen=buf.FrameBuf[2]+4;
	
	buf.FrameBuf[8]=SenloSum(buf);
  buf.UserID=0;
	SCREEN_DataLise(buf); //数据上传给串口屏
}
void ReagReplaace(TCommDataPacket buf)//试剂样本盘暂停
{
   if(buf.FrameBuf[3]==0x00)
	 {
		 cyc_41instru.pause=true ;
		 cyc_41instru.hByte=buf.FrameBuf[6];
		 cyc_41instru.R1R2SSloc=buf.FrameBuf[7];
	 }else if(buf.FrameBuf[3]==0x01&&cyc_41instru.R1R2SSloc)
	 {
			buf.FrameBuf[0]=0x53;
			buf.FrameBuf[1]=0x48;
			buf.FrameBuf[2]=0x06;
			buf.FrameBuf[3]=0x00;
			buf.FrameBuf[4]=cyc_41instru.hByte;
			buf.FrameBuf[5]=0xab;
			buf.FrameBuf[6]=0x00;
			buf.FrameBuf[7]=0x00;
			buf.FrameBuf[8]=cyc_41instru.R1R2SSloc;
			buf.FrameBuf[9]=SenloSum(buf);
			buf.UserID=0;
			buf.FramLen=buf.FrameBuf[2]+4;
		  Usart4out(buf);
	 }else if(buf.FrameBuf[3]==0x02)
	 {
		  cyc_41instru.pause=false  ;
	 }
}

void Commamdanalysis(TCommDataPacket buf)  //指令解析
{
	TWordRec iword;
//	 u8 i;
	Syscommand.Systray=0;
	Syscommand.Barcode=false;
	iword.HighByte=buf.FrameBuf[4];
	 iword.LowByte=buf.FrameBuf[5];
	 Syscommand.SysHByte=buf.FrameBuf[4];
	 Syscommand.SysLByte=buf.FrameBuf[5];
   if(Syscommand.SysHByte==0x41)
	 {
		  Syscommand.SysLByte=0x00;
			THeartbuf.Type=1;         ////机器工作状态
		 
		 
		if(buf.FrameBuf[10])
        THeartbuf.sampleloc=buf.FrameBuf[10];

		 if(cyc_41instru.pause&&buf.FrameBuf[14]==0x02)//暂停状态
		 {
		   pausetime(); //试剂样本盘暂停时间给显示器
		 }
		 Usart4out(buf);

			return ;
	 }
	 THeartbuf.Type=0;
		if(buf.FrameBuf[4]==0xb0&&buf.FrameBuf[5]==0x00)
		 {
			TimeSyc(buf);                                   //时间同步
		 }
	  switch(iword.Value)
		 {
//				case 0x0041:
//				{
//					USART3_DataLise(buf);  //数据传给从控SAM32F103RCT6 
//				}	break; //下版本号+//上版本号                                                                 break;  //启动测试
							
//				case 0x4001:
//				{
//				}	break; //下版本号+//上版本号                                                                 break;  //启动测试
					
				//				case 0x00F1:
//				{
//					 if(!SystemState.InitOK)
//					 {
//						 SystemState.InitOK = true;//自检ok
//						 VerInfoReturn();  
//					 }else
//						{
//							TestStart(); 
//						}
//				}	break; //下版本号+//上版本号                                                                 break;  //启动测试


				  case 0x00ab: ReagReplaace(buf);                                                     break;   //单个项目上传 
//				case 0x00bf: Reagentdata(PCSideRecBuff.DataBuf[0]);                                          break;  //试剂量
//				case 0x00be: ReagentdataAll();                                                               break;  //试剂量All
//				case 0x00bd: Test_List();                                                                    break;  //实验列表
//				case 0x00bc: Print_List();                                                                   break;  //打印列表
//				case 0x00bb: PCSprintFrameDis();                                                             break;  //打印结果   
//				case 0x00ba: CAL_List(PCSideRecBuff.DataBuf[0]);                                             break;  //校准列表 
					case 0x00bc:
						 QCmodeCMD=false;
					   USART6_Usart485(buf);
					   QCCMD_break1(iword);
					   QCsendCMD=iword;
						 break;
					 case 0x01bc:
						  QCmodeCMD=false;
					    USART6_Usart485(buf);
					    QCCMD_break1(iword);
					    QCsendCMD=iword;
						break;
				  case 0x00cd: {
						            Syscommand.Barcode=true;
	                      Syscommand.Systray=1;
						            buff2=buf;
						            USART3_DataLise(buf);  //第一次下发cd
					             }break;   // 						

          case 0x00f3: ItemID_List(buf);                                                        break;  //单个项目基本参数
				  case 0x00f4: EscItemIDparameter(buf.FrameBuf[6]);                                     break;   //单个项目上传 
				  case 0x00f5: ItemName232ter(buf);                                                     break;   //单个项目上传 
				  case 0x00f6: ItemNameter(buf.FrameBuf[6]);                                            break;   //单个项目上传 
				  case 0x00f8: Itemco232ter(buf);                                                       break;   //单个项目上传 


//				case 0x00FE: TestStopParse();                                                                break;  //紧急停止
				  case 0x00F2: CycCommdPacketParse( buf);                                                      break;  //常规项目测试包指令


//				case 0x0101: TestPauseParse(PCSideRecBuff.DataBuf[0]);                                       break;  //测试暂停/继续指令	
//				case 0x00FA: BlankInstParse();                                                               break;  
//				case 0x00ED: Blank_CupState();                                                               break;  //杯状态
//				case 0x00FB: DebugInstParse(PCSideRecBuff.DataBuf[0],PCSideRecBuff.DataBuf[1],     
//											 PCSideRecBuff.DataBuf[2], PCSideRecBuff.DataBuf[3]);                          break;   

				  case 0x0083:  
					{
	//								SpiFlash_Write((void *)&SysSave, W25QADDR0, sizeof(SysSave));
					 }break; //

//				case 0x0102: ReplaceAllCupsInit();                                                           break; //换杯确认     
//				case 0x0105: Delete();                                                                     break;//删除整个测试样本
//				case 0x0106: SaveName();                                                                     break;//病人姓名
					case 0x0107: TimeSyc(buf);                                    break; //时间同步
//					case 0x0108:
//							 {
//	               buf.UserID=8; 
//								 if(!Syscommand.V579finish) Out_DataLise(buf);//数据上传 数据上传给579网口
//								 if(!Syscommand.V103finish) USART3_UsartSent(buf);  //数据传给从控SAM32F103RCT6 
//							   Syscommand.V0108finish=true ;
//							 }break; //版本号查询					
						case 0x0206:
											{
												if(buf.FrameBuf[6])
												{
//													Com_priter=1;////打印关
												}else
												{
//													Com_priter=0;//打印开
												}
											}break; 


					case 0x75ad:             //单个项目ID查询AD
							{
//								 SItemID.Byte1=buf.FrameBuf[6];
//								 SItemID.Byte1=buf.FrameBuf[7];
//								 SItemID.Byte1=buf.FrameBuf[8];
//								 SItemID.Byte1=buf.FrameBuf[9];
								 ItemADdata(buf);
							}break;//
				  case 0x0113:  
					{
							 Sent_0113_Commd(buf);
					 }break; //
				 
				  case 0x9206:  
					{
            
			       USART7_Usart485(buf);
							
					}break; //

					case 0x7308:  
					{
            if(Syscommand.AD7308inst)
						{
						  Syscommand.AD7308inst=false ;
						}else
						{
						  Syscommand.AD7308inst=true;
						}
							
					}break; //
					default :
					  {
						 Usart4out(buf); 
					  }break;
				}
	 
}

void Data_TxUpExternThrd(void)
{
	
	if(Usa232Head.Time1ms==0)
		{
		  DataUpTXDout();             //数据上传发送
		}
	 
		if(ScreenHead.Time1ms==0)
			{
				SCREENLisesend();
			}
	if(USART3j45Head.Time1ms==0)
		{
			USART3Lisesend();
		}
			if(Usart1UsbHead.Time1ms==0)
			{
				USART1Lisesend();
			}
	}

/////////////////////////////
void Data_RxDnExternThrd(void)
{
	if(Syscommand.TimeHeart) 
	{
		
		TimeHeartbuf();
		Syscommand.TimeHeart=false  ;
	}
		if(!outbufusart4.outoff&& outbufusart4.timeouttx>=50)  USART4DataDnTXDout();       //数据下传发送
	 PC232_5Usart_Rx();       //Usart5数据查询 - 电脑RS232通讯
     USB_Usart1_Rx();       //Usart1数据查询
  Screen_Usart2_Rx();       //Usart2数据查询 - 串口屏通讯
         Usart8_Rx();       //Usart6数据查询
	       Usart6_Rx();       //Usart6数据查询
         Usart7_Rx();       //Usart6数据查询
      V5_Usart3_Rx();       //Usart3数据查询
				  Bar_code();  //条码扫描跟踪

}
//


