#include "sht_sensor.h"
uint8_t sht_cnt = 0, sht_id = 0, idx = 1, waitting;
uint32_t sht_tick;
SHT_DEVICE_t sht_list[10];

#include "modbus_data.h"
extern uint16_t modbus_holding_buf[NoB_HOLDING_REGISTER],
		modbus_input_buf[NoB_INPUT_REGISTER];

#include "control_system.h"
extern DEVICE_t device;

#include "config.h"

#include "kalman_filter.h"
Kalman_filter_TypeDef kalman;

void SHT_Measurement_Sum(void);
void SHT_Calc_Calib_Linear_Regression(float *val_a_b, float *val_x, float *val_y, uint8_t point);
uint16_t SHT_Calculator(uint16_t *pt, uint16_t *ph, uint16_t t_, uint16_t h_);

void SHT_Init_Load(void){

    // float a_b[2];
    // float pointx[4] = {21.4, 34.5, 50.4, 65.2, 81.4};
    // float pointy[4] = {20,3, 35.2, 49.8, 65.1, 80.2};

    for(int i = 0; i < 10; i++){
        sht_list[i].temp.a_user = sys_cfg.calib_Ta[i];
        sht_list[i].temp.b_user = sys_cfg.calib_Tb[i];
        sht_list[i].humi.a_user = sys_cfg.calib_Ha[i];
        sht_list[i].humi.b_user = sys_cfg.calib_Hb[i];

        printf("ss_%d | T_a = %2.2f | T_b = %2.2f\n", i, sht_list[i].temp.a_user , sht_list[i].temp.b_user);
        printf("ss_%d | H_a = %2.2f | H_b = %2.2f\n", i, sht_list[i].humi.a_user , sht_list[i].humi.b_user);
        HAL_Delay(50);
    }

    device.temp_set = sys_cfg.settemp;
    device.humi_set = sys_cfg.sethumi;
    modbus_holding_buf[20] = device.temp_set * 10;
    modbus_holding_buf[21] = device.humi_set * 10;

    // test
    // SHT_Calc_Calib_Linear_Regression(&a_b, pointx, pointy, 4);
    // printf("a = %2.2f | b = %2.2f\n", a_b[0], a_b[1]);
}

// Doc tat ca cac cam bien SHT
void SHT_Read_Process(void)
{
    switch (idx)
    {
        case 0:             // Waiting
             if (HAL_GetTick() - sht_tick > 1000)
            {
                if (waitting == 0)  sht_list[sht_id].state = 1;     // Error connect line sensor
                idx = 1;
                sht_id++;
                if (sht_id == 10)   sht_id = 0; 
            }
            break;
        case 1:             // READ SHT SENSOR
            Modbus_Uart2_Read_Regs(sht_id+1, 0, 2);
            idx = 0;
            waitting = 0;       // Cho timeout
            sht_tick = HAL_GetTick();
            break;
        
        default:
            break;
    }
}

void SHT_Get_Calib_Value(uint8_t address, uint16_t value)
{   
    // hiệu chuẩn 2 điểm

    uint8_t _id = (address / 5) - 6; // _id = 0 -> sensor id = 1
    uint8_t _reg = address % 5;

    printf("Sensor: %d - Reg: %d - Value: %d\n", _id, _reg, value);
    switch (_reg)
    {
        case TEMP_Y1_REG:
            sht_list[_id].temp.point.Y1 = value / 10.0;
            sht_list[_id].temp.point.X1 = sht_list[_id].temp.raw / 10.0;
            printf("ss_%d | Y1 = %2.2f, X1 = %2.2f\n", _id, sht_list[_id].temp.point.Y1, sht_list[_id].temp.point.X1);
            break;
        case TEMP_Y2_REG:
            sht_list[_id].temp.point.Y2 = value / 10.0;
            sht_list[_id].temp.point.X2 = sht_list[_id].temp.raw / 10.0;
            printf("ss_%d | Y2 = %2.2f, X2 = %2.2f\n", _id, sht_list[_id].temp.point.Y2, sht_list[_id].temp.point.X2);

            break;

        case HUMI_Y1_REG:
            sht_list[_id].humi.point.Y1 = value / 10.0;
            sht_list[_id].humi.point.X1 = sht_list[_id].humi.raw / 10.0;
            printf("ss_%d | Y2 = %2.2f, X2 = %2.2f\n", _id, sht_list[_id].humi.point.Y1, sht_list[_id].humi.point.X1);

            break;
        case HUMI_Y2_REG:
            sht_list[_id].humi.point.Y2 = value / 10.0;
            sht_list[_id].humi.point.X2 = sht_list[_id].humi.raw / 10.0;
            printf("ss_%d | Y2 = %2.2f, X2 = %2.2f\n", _id, sht_list[_id].humi.point.Y2, sht_list[_id].humi.point.X2);

            break;

        case ACTION_CALIB_REG:
            // Thu thi hieu chuan, Tim a , b
            if (value == 1)
            {
                sht_list[_id].temp.a_user = (sht_list[_id].temp.point.Y2 - sht_list[_id].temp.point.Y1) /
                                            (sht_list[_id].temp.point.X2 - sht_list[_id].temp.point.X1) ;
                sht_list[_id].temp.b_user = sht_list[_id].temp.point.Y1 - (sht_list[_id].temp.a_user  * sht_list[_id].temp.point.X1);

                sys_cfg.calib_Ta[_id] = sht_list[_id].temp.a_user;
                sys_cfg.calib_Tb[_id] = sht_list[_id].temp.b_user;
                CFG_Save();

                printf("Sensor: %d Ta: %2.2f | Tb: %2.2f\n", _id, sht_list[_id].temp.a_user, sht_list[_id].temp.b_user);
            }
            else if (value == 2)
            {
                sht_list[_id].humi.a_user = (float)(sht_list[_id].humi.point.Y2 - sht_list[_id].humi.point.Y1) /
                                            (sht_list[_id].humi.point.X2 - sht_list[_id].humi.point.X1);
                sht_list[_id].humi.b_user = (float)sht_list[_id].humi.point.Y1 - (sht_list[_id].humi.a_user*sht_list[_id].humi.point.X1);

                sys_cfg.calib_Ha[_id] = sht_list[_id].humi.a_user;
                sys_cfg.calib_Hb[_id] = sht_list[_id].humi.b_user;
                CFG_Save();

                printf("Sensor: %d Ha: %2.2f | Hb: %2.2f\n", _id, sys_cfg.calib_Ha[_id], sys_cfg.calib_Hb[_id]);

            }
            else if (value == 3)
            {
                sys_cfg.calib_Ta[_id] = sht_list[_id].temp.a_user = 1.0;
                sys_cfg.calib_Tb[_id] = sht_list[_id].temp.b_user = 0.0;
                CFG_Save();
                printf("Sensor: %d Ta: %2.2f | Tb: %2.2f\n", _id, sys_cfg.calib_Ta[_id], sys_cfg.calib_Tb[_id]);
                
            }
            else if (value == 4)
            {
                sys_cfg.calib_Ha[_id] = sht_list[_id].humi.a_user = 1.0;
                sys_cfg.calib_Hb[_id] = sht_list[_id].humi.b_user = 0.0;
                CFG_Save();
                printf("Sensor: %d Ha: %2.2f | Hb: %2.2f\n", _id, sys_cfg.calib_Ha[_id], sys_cfg.calib_Hb[_id]);

            }
            break;
        default:
            break;
    }
}

// Lay gia tri cac cam bien
void SHT_Get_Value(uint8_t id, uint16_t t, uint16_t h)
{
    // kiem tra gia tri over range -> loi cam bien

    if(SHT_Calculator(&sht_list[id].temp.raw, &sht_list[id].humi.raw, t, h) == 0)
    {
        // gan trang thai = 0 
        modbus_input_buf[id*5+2] = sht_list[id].state = 0;

        // gia tri user sau khi hieu chinh tu user
        modbus_input_buf[id*5] = sht_list[id].temp.user = (sht_list[id].temp.a_user * sht_list[id].temp.raw / 10.0 + sht_list[id].temp.b_user) * 10;
        modbus_input_buf[id*5+1] = sht_list[id].humi.user = (sht_list[id].humi.a_user * sht_list[id].humi.raw / 10.0 + sht_list[id].humi.b_user) * 10;
    }
    else
    {
        modbus_input_buf[id*5+2] = sht_list[id].state = 2;
    }

    if(sht_id < 8) {
        sht_cnt++;
    } else if (sht_id == 8) {
        // Da request het cam bien => tinh sum
        SHT_Measurement_Sum();
        sht_cnt = 0;
    }

    waitting = 1; // Next con tiep theo
    sht_tick = HAL_GetTick();
}

void SHT_Measurement_Sum(void)
{
    uint16_t temp_sum = 0, humi_sum = 0;
    for (int i = 0; i < 8; i++)
    {
        if (sht_list[i].state == 0)
        {
            temp_sum += sht_list[i].temp.user;
            humi_sum += sht_list[i].humi.user;
        }
    }
    // Tinh trung binh

    device.temp_now = (temp_sum / sht_cnt) / 10.0;
    device.humi_now = (humi_sum / sht_cnt) / 10.0;

    modbus_input_buf[50] = device.temp_now * 10;
    modbus_input_buf[51] = device.humi_now * 10;
}

void SHT_Calc_Calib_Linear_Regression(float *val_a_b, float *val_x, float *val_y, uint8_t point)
{
    /*
        + val_a_b giá trị a, b sau khi tính toán sẽ trả về mảng val_a_b[0], val_a_b[1]
    */
    float sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;

    for(int i = 1; i <= point; i++)
    {
        sumX += val_x[i];
        sumX2 += val_x[i] * val_x[i];
        sumY += val_y[i];
        sumXY += val_x[i] * val_y[i];
    }

    val_a_b[1] = (point * sumXY - sumX * sumY) / (point * sumX2 - sumX * sumX);
    val_a_b[0] = (sumY - val_a_b[1] * sumX ) / point;
}

uint16_t SHT_Calculator(uint16_t *pt, uint16_t *ph, uint16_t t_, uint16_t h_)
{
    uint16_t range_t_min = 0xFF38;  // -20.00 celsius
    uint16_t range_t_max = 0x0320;  // 80.00 celsius
    uint16_t range_h_min = 0x000;   // 0 %RH
    uint16_t range_h_max = 0x03E8;  // 100 %RH

    if(range_t_min <= t_ && t_ <= 0xFFFF)    *pt = (t_ - 65536);
    else if( 0x0000 <= t_ && t_ <= range_t_max)  *pt = t_;
    else    return 2;

    if( range_h_min <= h_ && h_ <= range_h_max)   *ph = h_;
    else    return 2;

    return 0;
}