/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-4-30     Sanjay_Wu  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include <string.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME "blux_v30"
#define BLUX_PIN_EN  GET_PIN(A,7)
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#include "B_LUX_V30.h"

//
static rt_err_t bluxv30_read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs;

    msgs.addr = BLUXV30_WRITE_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.buf = buf;
    msgs.len = len;

    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static rt_err_t bluxv30_write_cmd(struct rt_i2c_bus_device *bus, rt_uint8_t cmd)
{
    struct rt_i2c_msg msgs;

    msgs.addr = BLUXV30_WRITE_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf = &cmd;
    msgs.len = 1;

    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
        return RT_EOK;
    else
        return -RT_ERROR;
}

static rt_err_t bluxv30_set_measure_mode(bluxv30_device_t hdev, rt_uint8_t regs, rt_uint8_t m_time)
{
    RT_ASSERT(hdev);
	
	bluxv30_write_cmd(hdev->bus, BLUXV30_WRITE_ADDR);
	bluxv30_write_cmd(hdev->bus, regs);
	rt_thread_mdelay(m_time);
	
    return RT_EOK;
}

rt_err_t bluxv30_power_on(bluxv30_device_t hdev)
{
    RT_ASSERT(hdev);

    //bluxv30_write_cmd(hdev->bus, BH1750_POWER_ON);
    rt_pin_mode(BLUX_PIN_EN, PIN_MODE_OUTPUT);
    rt_pin_write(BLUX_PIN_EN,BLUXV30_POWER_ON);
	
    return RT_EOK;
}

rt_err_t bluxv30_power_down(bluxv30_device_t hdev)
{
    RT_ASSERT(hdev);

    //bluxv30_write_cmd(hdev->bus, BH1750_POWER_DOWN);
    rt_pin_mode(BLUX_PIN_EN, PIN_MODE_OUTPUT);
    rt_pin_write(BLUX_PIN_EN,BLUXV30_POWER_DOWN);
	
    return RT_EOK;
}

rt_err_t bluxv30_init(bluxv30_device_t hdev, const char *i2c_bus_name)
{
    hdev->bus = rt_i2c_bus_device_find(i2c_bus_name);
    if (hdev->bus == RT_NULL)
    {
        LOG_E("Can't find BLUX device on '%s' ", i2c_bus_name);
        rt_free(hdev->bus);
        return RT_NULL;
    }
	
    return RT_EOK;
}

float bluxv30_read_light(bluxv30_device_t hdev)
{
    rt_uint8_t temp[2];
    float current_light = 0;

    RT_ASSERT(hdev);

    bluxv30_set_measure_mode(hdev,0x00, 120);
	bluxv30_read_regs(hdev->bus, 2, temp);
	current_light = ((float)((temp[0] << 8) + temp[1]) / 1.2);

    return current_light;
}
