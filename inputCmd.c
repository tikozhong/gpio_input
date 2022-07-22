/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : inputCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "inputCmd.h"
#include "string.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "board.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char INPUT_HELP[] = {
	"input commands:"
	"\n x.input.help()"
	"\n x.input.readPin()/(indx)/(indx0,indx1)"
	"\n x.input.enableFalling(indx)/()"
	"\n x.input.disableFalling(indx)/()"
	"\n x.input.enableRaising(indx)/()"
	"\n x.input.disableRaising(indx)/()"
	"\n"
};

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : inputCmd
* Description    : gpio function commands
* Input          : 
									: 
* Output         : None
* Return         : None 
*******************************************************************************/
u8 inputCmd(INPUT_DEV_T *dev, const char* CMD, u8 brdAddr, char* buff, u16 len){
	u32 i,j,ii,jj;
	const char* line;
	char strTmp[100];
	INPUT_RSRC_T *pRsrc = &dev->rsrc;
	
	if(strncmp(CMD, pRsrc->name, strlen(pRsrc->name)) != 0)	return 0;
	line = &CMD[strlen(pRsrc->name)];
	//input.help()
	if(strncmp(line, ".help", strlen(".help")) == 0){
		strFormat(buff, len, "%s+ok@%d.%s.help()\r\n", INPUT_HELP, brdAddr, pRsrc->name);
	}		
	//input.readPin()/(indx)/(indx0,indx1)
	else if(sscanf(line, ".readpin %d %d ", &i, &j)==2){
		if(i>=dev->rsrc.gpioLen || j>=dev->rsrc.gpioLen){
			strFormat(buff, len, "+err@%d.%s.readpin(\"overflow\")\r\n", brdAddr, pRsrc->name);	
			return 1;
		}
		ii = 0;	jj = 0;
		if(dev->ReadPin(&dev->rsrc, i))	ii = 1;
		if(dev->ReadPin(&dev->rsrc, j))	jj = 1;
		strFormat(buff, len, "+ok@%d.%s.readpin(%d,%d,%d,%d)\r\n", brdAddr, pRsrc->name, i, ii, j, jj);
	}
	else if(sscanf(line, ".readpin %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(buff, len, "+err@%d.%s.readpin(\"overflow\")\r\n", brdAddr, pRsrc->name);	
			return 1;	
		}
		ii = 0;	
		if(dev->ReadPin(&dev->rsrc, i))	ii = 1;
		strFormat(buff, len, "+ok@%d.%s.readpin(%d,%d)\r\n", brdAddr, pRsrc->name, i, ii);
	}
	else if(strncmp(line, ".readpin  ", strlen("readpin  ")) == 0){
		strFormat(buff, len, "+ok@%d.%s.readpin(0x%x)\r\n", brdAddr, pRsrc->name, dev->rsrc.status[0]);
	}
	//"\n input.enableFalling(indx)/()"
	else if(sscanf(line, ".enablefalling %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(buff, len, "+err@%d.%s.enablefalling(\"overflow\")\r\n", brdAddr, pRsrc->name);	
			return 1;	
		}
		pRsrc->enableFalling |= BIT(i);
		strFormat(buff, len, "+ok@%d.%s.enablefalling(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableFalling);
	}
	else if(strncmp(line, ".enablefalling  ", strlen(".enablefalling  ")) == 0){
		pRsrc->enableFalling = 0xffffffff;
		strFormat(buff, len, "+ok@%d.%s.enablefalling(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableFalling);
	}
	//"\n input.disableFalling(indx)/()"	
	else if(sscanf(line, ".disablefalling %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(buff, len, "+err@%d.%s.disablefalling(\"overflow\")\r\n", brdAddr, pRsrc->name);	
			return 1;	
		}
		pRsrc->enableFalling &= 0xffff ^ BIT(i);
		strFormat(buff, len, "+ok@%d.%s.disablefalling(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableFalling);
	}
	else if(strncmp(line, ".disablefalling  ", strlen(".disablefalling  ")) == 0){
		pRsrc->enableFalling = 0;
		strFormat(buff, len, "+ok@%d.%s.disablefalling(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableFalling);
	}	
	//"\n input.enableRaising(indx)/()"
	else if(sscanf(line, ".enableraising %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(buff, len, "+err@%d.%s.enableraising(\"overflow\")\r\n", brdAddr, pRsrc->name);	
			return 1;	
		}
		pRsrc->enableRaising |= BIT(i);
		strFormat(buff, len, "+ok@%d.%s.enableraising(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableRaising);
	}
	else if(strncmp(line, ".enableraising  ", strlen(".enableraising  ")) == 0){
		pRsrc->enableRaising = 0xffffffff;
		strFormat(buff, len, "+ok@%d.%s.enableraising(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableRaising);
	}	
	//"\n input.disableRaising(indx)/()"
	else if(sscanf(line, ".disablefalling %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){
			strFormat(buff, len, "+err@%d.%s.disableraising(\"overflow\")\r\n", brdAddr, pRsrc->name);	
			return 1;	
		}
		pRsrc->enableRaising &= 0xffff ^ BIT(i);
		strFormat(buff, len, "+ok@%d.%s.disableraising(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableRaising);
	}
	else if(strncmp(line, ".disableraising  ", strlen(".disableraising  ")) == 0){
		pRsrc->enableRaising = 0;
		strFormat(buff, len, "+ok@%d.%s.disableraising(0x%x)\r\n", brdAddr, pRsrc->name, pRsrc->enableRaising);
	}	
	
	else{
		memset(strTmp,0,100);
		strcpy(strTmp,CMD);
		strFormat(buff, len, "+unknown@%d.%s", brdAddr, strTmp);
	}
	return 1;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
