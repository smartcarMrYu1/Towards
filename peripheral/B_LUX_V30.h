/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-04-30     Sanjay_Wu  the first version
 */

#ifndef __BLUX_V30_H__
#define __BLUX_V30_H__

#include <rthw.h>
#include <rtthread.h>

#include <rthw.h>
#include <rtdevice.h>

/*bh1750 device address */
#define BLUXV30_WRITE_ADDR 0x94    //Write address from machine
#define BLUXV30_READ_ADDR  0x95    //Read  address from machine

/*bh1750 registers define */
#define BLUXV30_POWER_DOWN   	    0x00	// power down
#define BLUXV30_POWER_ON			0x01	// power on
#define BLUXV30_RESET		    	0x07	// reset

#define BLUXV30_CON_H_RES_MODE	    0x10	// Continuously H-Resolution Mode
#define BLUXV30_CON_H_RES_MODE2	    0x11	// Continuously H-Resolution Mode2 
#define BLUXV30_CON_L_RES_MODE	    0x13	// Continuously L-Resolution Mode
#define BLUXV30_ONE_H_RES_MODE   	0x20	// One Time H-Resolution Mode
#define BLUXV30_ONE_H_RES_MODE2  	0x21	// One Time H-Resolution Mode2
#define BLUXV30_ONE_L_RES_MODE  	0x23	// One Time L-Resolution Mode


struct bluxv30_device
{
    struct rt_i2c_bus_device *bus;
};

typedef struct bluxv30_device *bluxv30_device_t;

rt_err_t bluxv30_power_on(bluxv30_device_t hdev);
rt_err_t bluxv30_power_down(bluxv30_device_t hdev);
rt_err_t bluxv30_init(bluxv30_device_t hdev, const char *i2c_bus_name);
float bluxv30_read_light(bluxv30_device_t hdev);



#endif /* __BH1750_H__ */
