/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-4-30     Sanjay_Wu  the first version
 */

#include "sensor_blux.h"
#include "B_LUX_V30.h"

#define DBG_ENABLE
#define DBG_LEVEL DBG_LOG
#define DBG_SECTION_NAME  "sensor.rohm.bluxv30"
#define DBG_COLOR
#include <rtdbg.h>


static bluxv30_device_t bluxv30_create(struct rt_sensor_intf *intf)
{
    bluxv30_device_t hdev = rt_malloc(sizeof(bluxv30_device_t));
	
    if (hdev == RT_NULL)
    {
        return RT_NULL;
    }
	
    bluxv30_init(hdev, intf->dev_name);

    return hdev;
}

static rt_size_t bluxv30_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    bluxv30_device_t hdev = sensor->parent.user_data;
    struct rt_sensor_data *data = (struct rt_sensor_data *)buf;
    
    if (sensor->info.type == RT_SENSOR_CLASS_LIGHT)
    {
        float light_value;

		light_value = bluxv30_read_light(hdev);

        data->type = RT_SENSOR_CLASS_LIGHT;
        data->data.light = (rt_int32_t)(light_value * 10);
        data->timestamp = rt_sensor_get_ts();
    }
    
    return 1;
}

rt_err_t bluxv30_set_power(bluxv30_device_t hdev, rt_uint8_t power)
{
    if (power == RT_SENSOR_POWER_NORMAL)
    {
        bluxv30_power_on(hdev);
    }
    else if (power == RT_SENSOR_POWER_DOWN)
    {
        bluxv30_power_down(hdev);
    }
    else
    {
        return -RT_ERROR;
    }
	
    return RT_EOK;
}


static rt_err_t bluxv30_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;
    bluxv30_device_t hdev = sensor->parent.user_data;

    switch (cmd)
    {
		case RT_SENSOR_CTRL_SET_POWER:
			result = bluxv30_set_power(hdev, (rt_uint32_t)args & 0xff);
		break;
		
		case RT_SENSOR_CTRL_SELF_TEST:
			result =  -RT_EINVAL;
        break;
		
		default:
			result = -RT_ERROR;
		break;
    }
    return result;
}

static struct rt_sensor_ops sensor_ops =
{
    bluxv30_fetch_data,
    bluxv30_control
};


int rt_hw_bluxv30_init(const char *name, struct rt_sensor_config *cfg)
{
	rt_int8_t result;
	rt_sensor_t sensor = RT_NULL;
	bluxv30_device_t hdev = bluxv30_create(&cfg->intf);

    sensor = rt_calloc(1, sizeof(struct rt_sensor_device));
    if (sensor == RT_NULL)
        return -1;

    sensor->info.type       = RT_SENSOR_CLASS_LIGHT;
    sensor->info.vendor     = RT_SENSOR_VENDOR_UNKNOWN;
    sensor->info.model      = "bluxv30_light";
    sensor->info.unit       = RT_SENSOR_UNIT_LUX;
    sensor->info.intf_type  = RT_SENSOR_INTF_I2C;
    sensor->info.range_max  = 65535;
    sensor->info.range_min  = 1;
    sensor->info.period_min = 120;

    rt_memcpy(&sensor->config, cfg, sizeof(struct rt_sensor_config));
    sensor->ops = &sensor_ops;
    
    result = rt_hw_sensor_register(sensor, name, RT_DEVICE_FLAG_RDWR, hdev);
    if (result != RT_EOK)
    {
        LOG_E("device register err code: %d", result);
        rt_free(sensor);
        return -RT_ERROR;
    }

    LOG_I("light sensor init success");
    return RT_EOK;
}


int bluxv30_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name = "i2c2";
    //cfg.intf.user_data = (void *)BH1750_ADDR;
    cfg.irq_pin.pin = RT_PIN_NONE;

    rt_hw_bluxv30_init("blux_v30", &cfg);
	
    return 0;
}
INIT_APP_EXPORT(bluxv30_port);



