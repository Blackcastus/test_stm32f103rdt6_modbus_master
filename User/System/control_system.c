// - Dieu khien may tach am, tao am
// - Dieu khien heater, cooler

#include "control_system.h"
#include "modbus_data.h"
#include "sht_sensor.h"

DEVICE_t device;

extern SHT_DEVICE_t sht_list[10];

uint32_t humidify_tick, moise_tick, thermis_tick;

void Ctrl_System_Turn_On_Daikin(void);
void Ctrl_System_Turn_Off_Daikin(void);
void Ctrl_System_Daikin_Set_Temp(uint8_t temp);
void Ctrl_System_Cooler(void);
void Ctrl_System_Valve_Air(uint8_t valve);
void Ctrl_System_Heater(void);
void Ctrl_System_Humidify(void);
void Ctrl_System_Moise(void);
void Ctrl_System_Waning_Temp_High(void);
void Ctrl_System_Light_Warning(void);
void Ctrl_System_Thermistor(uint8_t num);
void Ctrl_System_Turn_Off_All_Device(void);
// void Ctrl_System_Write_Daikin_TO_Modbus(uint8_t dk_add_reg, uint8_t dk_data); // TH calib nhieu diem


void Ctrl_System_Init_Load(void){
    device.light = 1;                       // den luan luan bat
    SHT_Init_Load();
    Ctrl_System_Turn_Off_All_Device();
}

void Ctrl_System_Device_Manage(void)
{
    Ctrl_System_Light_Warning();
    Modbus_Refesh_Regs();
    if(device.active == ACTIVE_ON)
    {
        Ctrl_System_Cooler();
        Ctrl_System_Heater();
        Ctrl_System_Moise();
        Ctrl_System_Humidify();
    }
    else
    {
        Ctrl_System_Turn_Off_All_Device();
    }
}

void Ctrl_System_Cooler(void) 
{
    // dieu khien phan lam lanh
    if(device.temp_set <= 28.0) 
    {
        Ctrl_System_Thermistor(THERMISTOR_ALL_OFF);
        Ctrl_System_Turn_On_Daikin();
        Ctrl_System_Daikin_Set_Temp(device.temp_set);
        Ctrl_System_Valve_Air(VALVE_COOL);
    }
}

void Ctrl_System_Heater(void) 
{
    // dieu khien phan gia nhiet
    // canh bao nhiet do cao, khoa cua ra vao

    Ctrl_System_Waning_Temp_High();

    if(device.temp_set > 28.0) 
    {
        if(device.temp_now < device.temp_set - DENTA_ERR) 
        {
            Ctrl_System_Turn_Off_Daikin();
            Ctrl_System_Valve_Air(VALVE_HEAT);
            if(HAL_GetTick() - thermis_tick > TIME_WAIT_DEVICE)
            {
                Ctrl_System_Thermistor(THERMISTOR3_ON);
                if(device.heater.status == 0)
                {
                    device.heater.status = 1;
                    device.heater.val_sample = device.temp_now;
                    device.heater.tick = HAL_GetTick();
                }
            }
            if(device.heater.status == 1)
            {
                // neu sau 60 phut gia nhiet ma nhiet do khong tang duoc thi canh bao
                if(HAL_GetTick() - device.heater.tick > SAMPLE_TIME_60M && device.temp_now < device.temp_set)
                {
                    if(fabs(device.temp_now - device.temp_set) > 3)
                    {
                        if(device.temp_now - device.heater.val_sample > -5.0)
                        {
                            device.heater.status = 2;
                        }
                    }
                }
            }
        }
        else if(device.temp_now > device.temp_set)
        {
            Ctrl_System_Thermistor(THERMISTOR_ALL_OFF);
            Ctrl_System_Valve_Air(VALVE_HEAT);
            if(device.heater.status == 1)
                device.heater.status = 0;

            thermis_tick = HAL_GetTick();
        }
    }
}

void Ctrl_System_Moise(void)
{
    // dieu khien may tao am
    if (device.humi_now < device.humi_set - DENTA_ERR)
    {
        if(HAL_GetTick() - moise_tick > TIME_WAIT_DEVICE)
        {
            if(device.moise.status == 0)
            {
                device.moise.stt_rl = device.moise.status = 1;
                device.moise.val_sample = device.humi_now;
                device.moise.tick = HAL_GetTick();
            }
        }
        if(device.moise.status == 1)
        {
            if(HAL_GetTick() - device.moise.tick > SAMPLE_TIME_60M && device.humi_now < device.humi_set)
            {
                if(fabs(device.humi_now - device.humi_set) > 3)
                {
                    if(device.humi_now - device.humidify.val_sample > -5.0)
                        device.humidify.status = 2;
                }
            }
        }
    }
    else if (device.humi_now > device.humi_set - DENTA_ERR)
    {
        if (device.moise.status == 1)
            device.moise.stt_rl = device.moise.status = 0;

        moise_tick = HAL_GetTick();
    }
}

void Ctrl_System_Humidify(void)
{
    // dieu khien may tach am
    if(device.humi_now > device.humi_set + DENTA_ERR)
    {
        if(HAL_GetTick() - humidify_tick > TIME_WAIT_DEVICE)
        {
            if(device.humidify.status == 0)
            {
                device.humidify.stt_rl = device.humidify.status = 1;
                device.humidify.val_sample = device.humi_now;
                device.humidify.tick = HAL_GetTick;
            }
        }
        if(device.humidify.status)
        {
            if(HAL_GetTick() - device.humidify.tick > SAMPLE_TIME_60M && device.humi_now > device.humi_set)
            {
                if(fabs(device.humi_now - device.humi_set) > 3)
                {
                    if (device.humi_now - device.humidify.val_sample > -5.0)
                        device.humidify.status = 2;                    
                }
            }
        }
    }
    else if(device.humi_now <= device.humi_set)
    {
        if(device.humidify.status == 1)
        {
            device.humidify.stt_rl = device.humidify.status = 0;
        }
        
        humidify_tick = HAL_GetTick();
    }
}

void Ctrl_System_Turn_On_Daikin(void) 
{
    if(device.cooler.status == 0)
    {
        HAL_Delay(50);
        Modbus_Uart2_Write_Single_Reg(ID_DAIKIN, 5, 1);
        // sample time
        device.cooler.val_sample = device.temp_now;
        device.cooler.tick = HAL_GetTick();
        // device.cooler.status = 1;
    }

    else if(device.cooler.status == 1)
    {
        // neu sau 60 phut lam lanh ma nhiet do khong thay doi nhieu so voi ban dau thi bao loi
        if(HAL_GetTick() - device.cooler.tick > SAMPLE_TIME_60M && device.temp_now > device.temp_set)
        {
            if(fabs(device.temp_now - device.temp_set) > 3)
            {
                if(device.temp_now - device.cooler.val_sample > -5)
                {
                    device.cooler.status = 2; // nhiet do khong thay doi nhieu sau 60 phut hoat dong -> warning
                }
            }
        }
    }

}

void Ctrl_System_Turn_Off_Daikin(void) 
{
     if(device.cooler.status == 1)
    {
        HAL_Delay(50);
        Modbus_Uart2_Write_Single_Reg(ID_DAIKIN, 5, 0);
        device.cooler.status = 0;
    }
}

void Ctrl_System_Daikin_Set_Temp(uint8_t temp) 
{
    if(device.cooler.status == 1 && temp != device.temp_daikin)
    {
        HAL_Delay(50);
        Modbus_Uart2_Write_Single_Reg(ID_DAIKIN, 1, temp);
        device.temp_daikin = temp;
    }
}

void Ctrl_System_Valve_Air(uint8_t valve) 
{
    switch (valve)
    {
        case 0:
            // close all valves
            device.valve_fan = 0;
            device.valve_cooler = 0;
            device.valve_heater = 0;
            break;
        case 1:
            // open the cool valve
            device.valve_fan = 1;
            device.valve_cooler = 1;
            device.valve_heater = 0;

            break;
        case 2:
            // open the heater valve
            device.valve_fan = 1;
            device.valve_cooler = 0;
            device.valve_heater = 1;
           
            break;
        default:

            break;
    }
}

void Ctrl_System_Thermistor(uint8_t num)
{
    switch (num)
    {
        case 0:
            device.thermistor1 = 0;
            device.thermistor2 = 0;
            device.thermistor3 = 0;
            break;
        case 1:
            device.thermistor1 = 1;
            device.thermistor2 = 0;
            device.thermistor3 = 0;
            break;
        case 2:
            device.thermistor1 = 1;
            device.thermistor2 = 1;
            device.thermistor3 = 0;
            break;
        case 3:
            device.thermistor1 = 1;
            device.thermistor2 = 1;
            device.thermistor3 = 1;
            break;
        default:
            break;
    }    
    
}

void Ctrl_System_Waning_Temp_High(void)
{
    // khoa cua khong cho mo khi nhiet do trong phong dang tang cao
    if(device.temp_now > 40.0 && device.sensor_door == 0)
    {
        device.door = 1;
    }
    else
    {
        device.door = 0;
    }
}

void Ctrl_System_Light_Warning(void)
{
    // Khi chay chuong trinh den xanh sang
    // Khi dung chuong trinh den do sang
    // khi co loi den vang sang

    if(device.active == ACTIVE_ON)
    {
        device.warning_green = 1;
        device.warning_red = 0;
    }
    else
    {
        device.warning_green = 0;
        device.warning_red = 1;
    }
    for(int i = 0; i < 10; i++)
    {
        if(sht_list[i].state != 0)
        {
            device.warning_yellow = 1;
        }
        else{
            device.warning_yellow = 0;
        }
    }
}

void Ctrl_System_Turn_Off_All_Device(void)
{
    Ctrl_System_Turn_Off_Daikin();
    Ctrl_System_Thermistor(THERMISTOR_ALL_OFF);
    Ctrl_System_Valve_Air(VALVE_OFF);
    device.humidify.stt_rl = device.humidify.status = 0;
    device.moise.stt_rl = device.moise.status = 0;
    device.door = 0;
    device.cooler.status = 0;
}

// void Ctrl_System_Write_Daikin_TO_Modbus(uint8_t dk_add_reg, uint8_t dk_data)
// {
//     switch (device.cooler.index)
//     {
//         case 0:
//             if(HAL_GetTick() - device.cooler.daikin_tick > TIMEOUT){   // wait timeout
//                 if(device.cooler.wait == 0)    device.cooler.state = 1;       
//                 device.cooler.index = 1;
//             }
//             break;
//         case 1:
//             Modbus_Uart2_Write_Single_Reg(ID_DAIKIN, dk_add_reg, dk_data); // goi lenh bat may lanh
//             device.cooler.wait = 0;                                // reset wait time out
//             device.cooler.index = 0;                               // chuyen sang inden wait time out
//             device.cooler.daikin_tick = HAL_GetTick();             // lay mau time time out
//             break;
//         default:
//             break;
//     }
// }