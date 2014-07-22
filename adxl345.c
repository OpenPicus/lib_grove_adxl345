/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        adxl345.c
 *  Module:          FlyPort WI-FI - FlyPort ETH
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Saccucci Stefano     1.0     02/22/2014		   First release  
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/

#include "taskFlyport.h"
#include "grovelib.h"
#include "adxl345.h"

struct Interface *attachSensorToI2CBus(void *,int,int);
int set_register(BYTE,BYTE, BYTE);
BYTE read_register(BYTE,BYTE);

struct adxl345
{
	const void *class;
	BYTE devaddress;
	struct Interface *inter;	
};

static void *adxl345_ctor (void * _self, va_list *app)
{
	struct adxl345 *self = _self;
	self->devaddress =  va_arg(*app, const BYTE);
	self->inter = NULL;
	return self;
}	

static void adxl345_dtor (void * _sensor)
{
	
}	

static void *adxl345_attach (void * _board,void *_sensor,int ic2bus)
{
	struct adxl345 *sensor = _sensor;
	int add = sensor->devaddress;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,add);
	return sensor->inter;
}	

static int adxl345_config (void * _self, va_list *app)
{
	struct adxl345 *self = _self;
	BYTE reg = va_arg(*app, const BYTE);
	BYTE rd = 0x00;
	BYTE data = 0x00;
	BYTE state = 0x00;
	switch(reg)
	{
		case ADXL345_INIT:
			if(set_register(self->devaddress, ADXL345_THRESH_TAP, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_OFSX, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_OFSY, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_OFSZ, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_DUR, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_Latent, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_Window, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_THRESH_ACT, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_THRESH_INACT, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_TIME_INACT, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_ACT_INACT_CTL, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_THRESH_FF, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_TIME_FF, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_TAP_AXES, 0x00))
				return -1;
			DelayMs(1);
			while(read_register(self->devaddress, ADXL345_BW_RATE)!=0x0A)
			{
				if(set_register(self->devaddress, ADXL345_BW_RATE, 0x0A))
					return -1;
				DelayMs(1);
			}
			if(set_register(self->devaddress, ADXL345_INT_ENABLE, 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_DATA_FORMAT, ADXL345_FULL_RES + ADXL345_RANGE_16G))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_FIFO_CTL , 0x00))
				return -1;
			DelayMs(1);
			if(set_register(self->devaddress, ADXL345_POWER_CTL, ADXL345_MEASURE))
				return -1;
			else
				return 0;
		break;
		case ADXL345_SINGLE_TAP:
			state = va_arg(*app, const BYTE);
			rd = read_register(self->devaddress, ADXL345_INT_ENABLE);
			if(state==OFF)
				rd = (rd&(~ADXL345_SINGLE_TAP))|OFF;
			else
			{
				if((rd&(~ADXL345_DOUBLE_TAP))==0)
				{
					if(set_register(self->devaddress, ADXL345_Latent, 0x00))
						return -1;
					DelayMs(1);
					if(set_register(self->devaddress, ADXL345_Window, 0x00))
						return -1;
					DelayMs(1);
				}
				if(set_register(self->devaddress, ADXL345_THRESH_TAP, 0x30))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_DUR, 0x10))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_TAP_AXES, ADXL345_TAP_Z+ADXL345_TAP_Y+ADXL345_TAP_X))
					return -1;
				DelayMs(1);	
				rd = (rd&(~ADXL345_SINGLE_TAP))|ADXL345_SINGLE_TAP;
			}
			if(set_register(self->devaddress, ADXL345_INT_ENABLE, rd))
				return -1;
			else
				return 0;
		break;
		case ADXL345_DOUBLE_TAP:
			state = va_arg(*app, const BYTE);			
			rd = read_register(self->devaddress, ADXL345_INT_ENABLE);
			if(state==OFF)
			{
				if(set_register(self->devaddress, ADXL345_Latent, 0x00))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_Window, 0x00))
					return -1;
				rd = (rd&(~ADXL345_DOUBLE_TAP))|OFF;
			}
			else
			{
				if(set_register(self->devaddress, ADXL345_Latent, 0x20))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_Window, 0xFF))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_THRESH_TAP, 0x30))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_DUR, 0x10))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_TAP_AXES, ADXL345_TAP_Z+ADXL345_TAP_Y+ADXL345_TAP_X))
					return -1;
				DelayMs(1);	
				rd = (rd&(~ADXL345_DOUBLE_TAP))|ADXL345_DOUBLE_TAP;
			}
			if(set_register(self->devaddress, ADXL345_INT_ENABLE, rd))
				return -1;
			else
				return 0;
		break;
		case ADXL345_FREE_FALL:
			state = va_arg(*app, const BYTE);
			rd = read_register(self->devaddress, ADXL345_INT_ENABLE);
			if(state==OFF)
				rd = (rd&(~ADXL345_FREE_FALL))|OFF;
			else
			{
				if(set_register(self->devaddress, ADXL345_THRESH_FF, 0x0B))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_TIME_FF, 0x20))
					return -1;
				DelayMs(1);
				rd = (rd&(~ADXL345_FREE_FALL))|ADXL345_FREE_FALL;
			}
			if(set_register(self->devaddress, ADXL345_INT_ENABLE, rd))
				return -1;
			else
				return 0;
		break;
		case ADXL345_ACTIVITY:
			state = va_arg(*app, const BYTE);
			rd = read_register(self->devaddress, ADXL345_INT_ENABLE);
			if(state==OFF)
				rd = (rd&(~ADXL345_ACTIVITY))|OFF;
			else
			{
				if(set_register(self->devaddress, ADXL345_THRESH_ACT, 0x0A))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_ACT_INACT_CTL, 0b11111111))
					return -1;
				DelayMs(1);
				rd = (rd&(~ADXL345_ACTIVITY))|ADXL345_ACTIVITY;
			}
			if(set_register(self->devaddress, ADXL345_INT_ENABLE, rd))
				return -1;
			else
				return 0;
		break;
		case ADXL345_INACTIVITY:
			state = va_arg(*app, const BYTE);
			rd = read_register(self->devaddress, ADXL345_INT_ENABLE);
			if(state==OFF)
				rd = (rd&(~ADXL345_INACTIVITY))|OFF;
			else
			{
				if(set_register(self->devaddress, ADXL345_THRESH_INACT, 0x02))
					return -1;
				DelayMs(1);
				data = va_arg(*app, const BYTE);
				if(set_register(self->devaddress, ADXL345_TIME_INACT, data))
					return -1;
				DelayMs(1);
				if(set_register(self->devaddress, ADXL345_ACT_INACT_CTL, 0b11111111))
					return -1;
				DelayMs(1);
				rd = (rd&(~ADXL345_INACTIVITY))|ADXL345_INACTIVITY;
			}
			if(set_register(self->devaddress, ADXL345_INT_ENABLE, rd))
				return -1;
			else
				return 0;
		break;
		default:
			data = va_arg(*app, const BYTE);
			if(set_register(self->devaddress, reg, data))
				return -1;
			else
				return 0;
		break;
	}
	return -1;	
}

static float adxl345_get (void * _self, va_list *app)
{
	struct adxl345 *self = _self;
	BYTE regist = va_arg(*app, const BYTE);
	BYTE registh;
	float data;
	BYTE reg_bits=0x00;
	switch(regist)
	{
		case ADXL345_X:
				regist = read_register(self->devaddress, ADXL345_DATAX0);
				DelayMs(1);
				registh = read_register(self->devaddress, ADXL345_DATAX1);
				data = ((((int)registh)<<8)|regist)*0.004;
			break;
		case ADXL345_Y:
				regist = read_register(self->devaddress, ADXL345_DATAY0);
				DelayMs(1);
				registh = read_register(self->devaddress, ADXL345_DATAY1);
				data = ((((int)registh)<<8)|regist)*0.004;
			break;
		case ADXL345_Z:
				regist = read_register(self->devaddress, ADXL345_DATAZ0);
				DelayMs(1);
				registh = read_register(self->devaddress, ADXL345_DATAZ1);
				data = ((((int)registh)<<8)|regist)*0.004;
			break;
		case ADXL345_GESTURE:
				data = read_register(self->devaddress, ADXL345_INT_SOURCE);
				DelayMs(1);
		break;
		
		default:
				data = read_register(self->devaddress, regist);
				DelayMs(1);
				reg_bits = va_arg(*app, const BYTE);
				if(reg_bits!=NO_BITS)
					data = ((BYTE)data)&reg_bits;
		break;
	}
	return data;
}

static const struct SensorClass _adxl345 =
{	
	sizeof(struct adxl345),
	adxl345_ctor,
	adxl345_dtor,
	adxl345_attach,
	adxl345_config,
	0,
	adxl345_get,
};

const void *adxl345 = &_adxl345;
