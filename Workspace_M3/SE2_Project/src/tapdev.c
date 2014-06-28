///*
// * Copyright (c) 2001, Swedish Institute of Computer Science.
// * All rights reserved.
// *
// * Redistribution and use in source and binary forms, with or without
// * modification, are permitted provided that the following conditions
// * are met:
// *
// * 1. Redistributions of source code must retain the above copyright
// *    notice, this list of conditions and the following disclaimer.
// *
// * 2. Redistributions in binary form must reproduce the above copyright
// *    notice, this list of conditions and the following disclaimer in the
// *    documentation and/or other materials provided with the distribution.
// *
// * 3. Neither the name of the Institute nor the names of its contributors
// *    may be used to endorse or promote products derived from this software
// *    without specific prior written permission.
// *
// * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
// * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
// * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// * SUCH DAMAGE.
// *
// * Author: Adam Dunkels <adam@sics.se>
// *
// * $Id: tapdev.c,v 1.8 2006/06/07 08:39:58 adam Exp $
// */
//
//#define UIP_DRIPADDR0   192
//#define UIP_DRIPADDR1   168
//#define UIP_DRIPADDR2   0
//#define UIP_DRIPADDR3   1
//
//#include <stdlib.h>
//#include <stdio.h>
//
//#include <string.h>
//
//
//#ifdef linux
//#include <sys/ioctl.h>
//#include <linux/if.h>
//#include <linux/if_tun.h>
//#define DEVTAP "/dev/net/tun"
//#else  /* linux */
//#define DEVTAP "/dev/tap0"
//#endif /* linux */
//
//#include "uip.h"
//
//static int drop = 0;
//static int fd;
//
//

#include "chip.h"
#include "lpc_types.h"
#include "enet_17xx_40xx.h"
#include "uip.h"
#include "enet.h"

#define useRMMI TRUE

void
tapdev_init(void){
	Chip_ENET_Setup(LPC_ETHERNET, useRMMI);
}

unsigned int
tapdev_read(void){
	void* buff = NULL;
	int32_t bytes = 0;
	buff = ENET_RXGet(&bytes);
	if(bytes){
		memcpy(uip_buf, buff, bytes);
		uip_len = bytes;
	}
	Chip_ENET_IncRXConsumeIndex(LPC_ETHERNET);
	return bytes;
}

void
tapdev_send(void){
	void* bufAddr = ENET_TXBuffGet();
	memcpy(bufAddr, uip_buf, uip_len);
	ENET_TXQueue(uip_len);
}
