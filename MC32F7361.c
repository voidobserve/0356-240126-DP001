/******************************************************************************
;  *       @型号                 : MC32F7361
;  *       @创建日期             : 2021.12.21
;  *       @公司/作者            : SINOMCU-FAE
;  *       @晟矽微技术支持       : 2048615934
;  *       @晟矽微官网           : http://www.sinomcu.com/
;  *       @版权                 : 2021 SINOMCU公司版权所有.
;  *----------------------摘要描述---------------------------------


******************************************************************************/

#include "user.h"
#if 1 // 验证通过的代码
#if USE_MY_DEBUG
// 发送32bit的数据（高位先行）
void send_data_32bit_msb(u32 send_data)
{
    u8 i = 0; // 循环计数值

    P21D = 0;
    delay_ms(1);
    P21D = 1; // 先将引脚电平拉低
    delay_ms(1);
    P21D = 0;

    for (i = 0; i < 32; i++)
    {
        // 每次循环发送一位二进制的数据
        if ((send_data >> (32 - 1 - i)) & 0x01)
        {
            // 如果要发送的是"1"
            P21D = 1;
        }
        else
        {
            // 如果要发送的是"0"
            P21D = 0;
        }

        delay_ms(10); // 引脚电平的保持时间
    }

    // 发送完成后，将引脚电平拉低
    P21D = 0;
    delay_ms(1);
    P21D = 1;
    delay_ms(1);
    P21D = 0;
}
#endif // USE_MY_DEBUG // 发送32bit的数据（高位先行）

// 毫秒级延时
// 前提条件：FCPU = FHOSC / 4
void delay_ms(u32 xms)
{
    while (xms)
    {
        u16 i = 550;
        while (i--)
        {
            Nop();
        }
        xms--;
    }
}

/************************************************
;  *    @函数名          : CLR_RAM
;  *    @说明            : 清RAM
;  *    @输入参数        :
;  *    @返回参数        :
;  ***********************************************/
void CLR_RAM(void)
{
    for (FSR0 = 0; FSR0 < 0xff; FSR0++)
    {
        INDF0 = 0x00;
    }
    FSR0 = 0xFF;
    INDF0 = 0x00;
}
/************************************************
;  *    @函数名            : IO_Init
;  *    @说明              : IO初始化
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void IO_Init(void)
{
    IOP0 = 0x00;   // io口数据位
    OEP0 = 0x3F;   // io口方向 1:out  0:in
    PUP0 = 0x00;   // io口上拉电阻   1:enable  0:disable
    PDP0 = 0x00;   // io口下拉电阻   1:enable  0:disable
    P0ADCR = 0x00; // io类型选择  1:模拟输入  0:通用io

    IOP1 = 0x00;   // io口数据位
    OEP1 = 0xFF;   // io口方向 1:out  0:in
    PUP1 = 0x00;   // io口上拉电阻   1:enable  0:disable
    PDP1 = 0x00;   // io口下拉电阻   1:enable  0:disable
    P1ADCR = 0x00; // io类型选择  1:模拟输入  0:通用io

    IOP2 = 0x00; // io口数据位
    OEP2 = 0x0F; // io口方向 1:out  0:in
    PUP2 = 0x00; // io口上拉电阻   1:enable  0:disable
    PDP2 = 0x00; // io口下拉电阻   1:enable  0:disablea

    PMOD = 0x00; // P00、P01、P13 io端口值从寄存器读，推挽输出

    // DRVCR = 0x70; // 全部IO增强型驱动（这个会导致蓝色最亮，红灯和绿灯直接变成微微点亮）
    // DRVCR = 0x80; // 普通驱动
    // DRVCR = 0; // 普通端口增强驱动（红光会变得很暗--微微点亮，其他颜色只有蓝色和绿色的组合）
    // DRVCR = 0x90; // P16、P17输出驱动电流为50mA，其余都是普通驱动（绿光会变得很暗--微微点亮，其他颜色只有红色和蓝色的组合）
    // 那么端口输出配置这里只能将全部IO配置为普通驱动
    DRVCR = 0x80; // 普通驱动

    // 开关K1、K2和光敏器件的相关配置
    OEP0 &= ~0x07; // P00、P01、P02配置为输入模式
    PUP0 |= 0x07;  // P00、P01、P02使能上拉电阻
    OEP1 &= ~0x1E; // P11、P12、P13、P14配置为输入模式
    PUP1 |= 0x1C;  // P12、P13、P14使能上拉电阻

    PMOD |= 0x2A; // P00、P01、P13输入通道连通
}

// 开关K1配置（初始化）
// void key1_config(void)
// {
// P00
// OEP0 &= ~(0x01 << 0); // 输入模式
// PUP0 |= 0x01 << 0;    // 上拉
// PMOD |= 0x01 << 1;    // 端口作为输出口，读端口操作将读取端口的实际电平状态（输入通道连通）

// P01
// OEP0 &= ~(0x01 << 1); // 输入模式
// PUP0 |= 0x01 << 1;    // 上拉
// PMOD |= 0x01 << 3;    // 端口作为输出口，读端口操作将读取端口的实际电平状态（输入通道连通）

// P02
// OEP0 &= ~(0x01 << 2); // 输入模式
// PUP0 |= 0x01 << 2;    // 上拉
// }

// 开关K2配置（初始化）
// void key2_config(void)
// {
// P12
// OEP1 &= ~(0x01 << 2); // 输入模式
// PUP1 |= 0x01 << 2;    // 上拉

// P13
// OEP1 &= ~(0x01 << 3); // 输入模式
// PUP1 |= 0x01 << 3;    // 上拉
// PMOD |= 0x01 << 5;    // 端口作为输出口，读端口操作将读取端口的实际电平状态（输入通道连通）

// P14
// OEP1 &= ~(0x01 << 4); // 输入模式
// PUP1 |= 0x01 << 4;    // 上拉
// }

// 开关状态扫描
void key_scan(void)
{
    // 扫描左侧开关
    if (KEY_1_1_PIN == 0)
    {
        // 如果是左侧开关--上档
        delay_ms(10);
        if (KEY_1_1_PIN == 0)
        {
            left_key_val = LEFT_EY_UP;
        }
    }
    else if (KEY_1_2_PIN == 0)
    {
        // 如果是左侧开关--中档
        delay_ms(10);
        if (KEY_1_2_PIN == 0)
        {
            left_key_val = LEFT_KEY_MID;
        }
    }
    else if (KEY_1_3_PIN == 0)
    {
        // 如果是左侧开关--下档
        delay_ms(10);
        if (KEY_1_3_PIN == 0)
        {
            left_key_val = LEFT_KEY_BOTTOM;
        }
    }

    // 扫描右侧开关
    if (KEY_2_1_PIN == 0)
    {
        // 如果是右侧开关--下档
        delay_ms(10);
        if (KEY_2_1_PIN == 0)
        {
            right_key_val = RIGHT_KEY_BOTTOM;
        }
    }
    else if (KEY_2_2_PIN == 0)
    {
        // 如果是右侧开关--中档
        delay_ms(10);
        if (KEY_2_2_PIN == 0)
        {
            right_key_val = RIGHT_KEY_MID;
        }
    }
    else if (KEY_2_3_PIN == 0)
    {
        // 如果是右侧开关--上档
        delay_ms(10);
        if (KEY_2_3_PIN == 0)
        {
            right_key_val = RIGHT_KEY_UP;
        }
    }
}

// pwm0--负责控制B，蓝色（默认占空比为100）
void timer0_pwm_config(void)
{
    // T0CR |= 0x01 << 0; // 时钟预分频--8分频
    // T0CR |= 0x01 << 1;

    // T0CR &= ~(0x01 << 3); // 时钟源选择FCPU（可以不写，在寄存器中默认就是0）
    // T0CR &= ~(0x01 << 4);
    T0CR |= 0x01 << 6; // 使能 PWM0 功能，并允许端口输出脉宽调制波形
    T0CNT = 255 - 1;
    T0LOAD = 255 - 1; //

    // T0CNT = 255;
    // T0LOAD = 255; //

    T0DATA = 255;           // 占空比，占空比 == DATA / LOAD * 100%
    PWMCR0 &= ~(0x01 << 4); // PWM0输出正向波形
    PWMCR1 &= ~(0x01 << 0); // PWM0工作于普通模式
    PWMCR3 &= ~(0x01 << 2); // PWM0 输出通道为 PWM0 O0（P16）
    T0EN = 1;
}

// pwm1--负责控制G，绿色（默认占空比为100）
void timer1_pwm_config(void)
{
    // T1CR |= 0x01 << 0; // 时钟预分频--8分频
    // T1CR |= 0x01 << 1;

    // T1CR &= ~(0x01 << 3); // 时钟源选择FCPU（可以不写，在寄存器中默认就是0）
    // T1CR &= ~(0x01 << 4);
    T1CR |= 0x01 << 6; // 使能 PWM 功能，并允许端口输出脉宽调制波形
    T1CNT = 255 - 1;
    T1LOAD = 255 - 1; //

    // T1CNT = 255;
    // T1LOAD = 255;

    T1DATA = 255;           // 占空比，占空比 == DATA / LOAD * 100%
    PWMCR1 &= ~(0x01 << 1); // PWM1工作于普通模式
    T1EN = 1;
}

// pwm2--负责控制R，红色（默认占空比为100）
void timer2_pwm_config(void)
{
    // T2CR |= 0x01 << 0; // T2时钟预分频--8分频
    // T2CR |= 0x01 << 1;

    // T2CR &= ~(0x01 << 3); // 时钟源选择FCPU（可以不写，在寄存器中默认就是0）
    // T2CR &= ~(0x01 << 4);
    T2CR |= 0x01 << 6; // 使能 PWM2 功能，并允许端口输出脉宽调制波形
    T2CNT = 255 - 1;   // T2计数器
    T2LOAD = 255 - 1;  // T2重载寄存器

    // T2CNT = 255;   // T2计数器
    // T2LOAD = 255;  // T2重载寄存器

    T2DATA = 255; // T2 比较寄存器，用于设置 PWM2 的占空比
    // PWMCR0 &= ~(0x01 << 6); // PWM2端口输出正向波形（可以不写，在寄存器中默认就是0）
    // PWMCR1 &= ~(0x01 << 2); // PWM2 工作于普通模式（可以不写，在寄存器中默认就是0）
    T2EN = 1; // 开启定时器T2
}

#endif // 验证通过的代码

// 光敏器件的配置
// void light_sensor_config(void)
// {
// 浮空输入
// OEP1 &= ~(0x01 << 1); // 输入
// PUP1 &= ~(0x01 << 1); // 内部上拉无效（可以不写，默认就是0）
// PDP1 &= ~(0x01 << 1); // 内部下拉无效（可以不写，默认就是0）
// }

// 设置PWM占空比
// 参数1：哪一个pwm，例如 0 -- pwm0
// 参数2：占空比，0~255
void pwm_set_duty(u8 pwm_x, const u8 duty)
{
    // 要节省程序空间，这里的错误处理就没加
    // if (pwm_x > 2 || duty > 100)
    // {
    //     return;
    // }

    // if (duty > 0 && duty < 255)
    // {
    //     duty--;
    // }

    switch (pwm_x)
    {
    case 0:
        T0DATA = duty; // 占空比
        break;
    case 1:
        T1DATA = duty;
        break;
    case 2:
        T2DATA = duty;
        break;
    }
}

// 显示单种颜色
void color_show(u8 *rgb_val)
{
    cur_r = rgb_val[0];
    cur_g = rgb_val[1];
    cur_b = rgb_val[2];

    color_set_component_r(cur_r); // 红色分量
    color_set_component_g(cur_g); // 绿色分量
    color_set_component_b(cur_b); // 蓝色分量
}

// 设置颜色的红色分量
void color_set_component_r(const u8 component_r_val)
{
    pwm_set_duty(2, component_r_val); // 红色分量
}

// 设置颜色的绿色分量
void color_set_component_g(const u8 component_g_val)
{
    pwm_set_duty(1, component_g_val); // 绿色分量
}

// 设置颜色的蓝色分量
void color_set_component_b(const u8 component_b_val)
{
    pwm_set_duty(0, component_b_val); // 蓝色分量
}

#if 1
// 获取渐变模式下调节颜色分量的时间间隔
u16 __get_gradient_adjust_time(u8 color_component)
{
    return (u32)GRADIENT_TIME * 100 / color_component;
}

// 颜色渐变--从当前的颜色变化为指定颜色
void __color_gradient(u8 *next_color)
{
    next_r = next_color[0];
    next_g = next_color[1];
    next_b = next_color[2];

    u8 diff_r = 0; // 两种颜色的红色分量的差值
    u8 diff_g = 0; // 两种颜色的绿色分量的差值
    u8 diff_b = 0; // 两种颜色的蓝色分量的差值

    // 求出两种颜色的RGB的差值（这里用函数来封装，空间占用更大）
    if (cur_r > next_r)
    {
        diff_r = cur_r - next_r;
        DIR_R = 0; // 接下来红色分量要变小
    }
    else
    {
        diff_r = next_r - cur_r;
        DIR_R = 1; // 接下来红色分量要变大
    }

    if (cur_g > next_g)
    {
        diff_g = cur_g - next_g;
        DIR_G = 0; // 接下来绿色分量要变小
    }
    else
    {
        diff_g = next_g - cur_g;
        DIR_G = 1; // 接下来绿色分量要变大
    }

    if (cur_b > next_b)
    {
        diff_b = cur_b - next_b;
        DIR_B = 0; // 接下来蓝色分量要变小
    }
    else
    {
        diff_b = next_b - cur_b;
        DIR_B = 1; // 接下来蓝色分量要变大/
    }

    // 获取调整颜色分量的时间
    time_interval_r = __get_gradient_adjust_time(diff_r);
    time_interval_g = __get_gradient_adjust_time(diff_g);
    time_interval_b = __get_gradient_adjust_time(diff_b);

    // 测试差值是否正确（测试通过）
    // send_data_32bit_msb(diff_r);
    // send_data_32bit_msb(diff_g);
    // send_data_32bit_msb(diff_b);

    // 变化时，还要考虑差值为零的情况、开关状态突然变化的情况
    // 变化到最后时，如果超过了预定的颜色值，还需要做回调整
    tmp_r = 0; // 临时变量
    tmp_g = 0;
    tmp_b = 0;

    for (u16 i = 0; i < GRADIENT_TIME; i++)
    {
        // 如果开关状态有变化，让函数返回
        // 如果开启/关闭光感，也要做相应的处理
        if (KEY_2_3_PIN == 0)
        {
            delay_ms(10);
            if (KEY_2_3_PIN == 0)
            {
                right_key_val = RIGHT_KEY_UP;
                // 如果关闭了电源
                return;
            }
        }
        else if (0 == KEY_1_1_PIN)
        {
            delay_ms(10);
            if (0 == KEY_1_1_PIN)
            {
                left_key_val = LEFT_EY_UP;
                // 如果开启了锁色
                return;
            }
        }
        else if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN) &&
                 (0 == SWITCH_CHANGE_FLAG))
        {
            // 打开感光，并且检测到是白天，才会进入，防止影响调整时间
            delay_ms(10);
            if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN))
            {
                // 如果打开了感光，且检测到是白天
                RGB_ENABLE = 0; // 白天灯不亮
                SWITCH_CHANGE_FLAG = 1;
                return;
            }
        }

        // tmp_r++;
        // tmp_g++;
        // tmp_b++;

        tmp_r += 100;
        tmp_g += 100;
        tmp_b += 100;

        if (DIR_R) // 如果颜色分量要增加
        {
            // 如果到了调节时间，且当前颜色分量小于目标颜色分量
            if (tmp_r >= time_interval_r && cur_r < next_r)
            {
                // tmp_r -= time_interval_r;
                tmp_r = 0;
                cur_r++;
            }
        }
        else
        {

            // 如果到了调节时间，且当前颜色分量大于目标颜色分量
            if (tmp_r >= time_interval_r && cur_r > next_r)
            {
                // tmp_r -= time_interval_r;
                tmp_r = 0;
                cur_r--;
            }
        }

        if (DIR_G)
        {
            // 如果到了调节时间，且当前颜色分量小于目标颜色分量
            if (tmp_g >= time_interval_g && cur_g < next_g)
            {
                // tmp_g -= time_interval_g;
                tmp_g = 0;
                cur_g++;
            }
        }
        else
        {
            // 如果到了调节时间，且当前颜色分量大于目标颜色分量
            if (tmp_g >= time_interval_g && cur_g > next_g)
            {
                // tmp_g -= time_interval_g;
                tmp_g = 0;
                cur_g--;
            }
        }

        if (DIR_B)
        {
            // 如果到了调节时间，且当前颜色分量小于目标颜色分量
            if (tmp_b >= time_interval_b && cur_b < next_b)
            {
                // tmp_b -= time_interval_b;
                tmp_b = 0;
                cur_b++;
            }
        }
        else
        {
            // 如果到了调节时间，且当前颜色分量大于目标颜色分量
            if (tmp_b >= time_interval_b && cur_b > next_b)
            {
                // tmp_b -= time_interval_b;
                tmp_b = 0;
                cur_b--;
            }
        }

        color_set_component_r(cur_r); // 红色分量
        color_set_component_g(cur_g); // 绿色分量
        color_set_component_b(cur_b); // 蓝色分量
        delay_ms(1);                  //
    }

    // 最后，直接显示目标的颜色（不推荐，颜色会有跳动）
    // cur_r = next_r; // 更新全局保存的颜色分量
    // cur_g = next_g;
    // cur_b = next_b;
    // color_set_component_r(next_r); // 红色分量
    // color_set_component_g(next_g); // 绿色分量
    // color_set_component_b(next_b); // 蓝色分量

    for (i = 0; i < 2000; i++) // 保持 xx ms
    {
        if (KEY_2_3_PIN == 0)
        {
            delay_ms(10);
            if (KEY_2_3_PIN == 0)
            {
                right_key_val = RIGHT_KEY_UP;
                // 如果关闭了电源
                return;
            }
        }
        else if (0 == KEY_1_1_PIN)
        {
            delay_ms(10);
            if (0 == KEY_1_1_PIN)
            {
                left_key_val = LEFT_EY_UP;
                // 如果开启了锁色
                return;
            }
        }
        else if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN) &&
                 (0 == SWITCH_CHANGE_FLAG))
        {
            delay_ms(10);
            if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN))
            {
                // 如果打开了感光，且检测到是白天
                RGB_ENABLE = 0; // 白天灯不亮
                SWITCH_CHANGE_FLAG = 1;
                return;
            }
        }

        delay_ms(1);
    }
}

// 渐变模式
void mode_gradient(void)
{
    __color_gradient(color_buf[cur_color_index]);

    if (0 == SWITCH_CHANGE_FLAG)
    {
        // 如果开关状态没有变化，继续执行渐变，切换到下一种颜色
        cur_color_index++;
        if (COLOR_NUMS == cur_color_index) // 如果已经超出了存放颜色的数组的下标
        {
            cur_color_index = 0;
        }
    }
    else
    {
        // 检测到 __color_gradient() 中已经将标志位置一，现在将它清零
        SWITCH_CHANGE_FLAG = 0;
    }
}
#endif

// 根据目标颜色分量，得到呼吸模式下要调整颜色分量的时间间隔
// 参数1：目前呼吸模式处于渐亮还是渐灭的状态，1--渐亮，0--渐灭
u16 __get_time_interval(u8 light_on_off, u8 color_component)
{
    // 要节省代码空间
    if (0 == color_component)
    {
        return 0; // 颜色分量为0，返回0
    }

    if (light_on_off)
    {
        // 如果在呼吸模式处于渐渐点亮的状态
        return (u32)BREATHING_LIGHT_ON_TIME * 100 / color_component;
        // return BREATHING_LIGHT_ON_TIME / color_component;
    }
    else
    {
        // 如果在呼吸模式处于渐渐熄灭的状态
        // return BREATHING_LIGHT_OFF_TIME * 100 / color_component;
        return (u32)BREATHING_LIGHT_OFF_TIME * 100 / color_component;
    }

    // return color_component * 100 / 850;

    // 测试不通过的程序
    // return color_component * 100 / 1000; (呼吸变暗时，第3种、第4种、第11种颜色会变色)
}

// // 根据目标颜色分量和调整时间间隔，得到呼吸模式下每次要调整的颜色分量的值
// u16 __get_adjust_val(u8 light_on_off, u8 color_component, u16 time_interval)
// {
//     return color_component * 10 / (850 / time_interval);

//     // 测试不通过的程序
//     // return color_component / (85 / time_interval);
//     // return color_component / (100 / time_interval);
//     // return color_component * 10 / (1000 / time_interval); // (呼吸变暗时，第3种、第4种、第11种颜色会变色)
// }

// 呼吸模式下，切换到指定的颜色
void __color_breathing(u8 *next_color)
{
    next_r = next_color[0]; // 得到目标颜色分量
    next_g = next_color[1];
    next_b = next_color[2];

    tmp_r = 0; // 临时变量，负责控制每隔固定的时间调节一次当前RGB的亮度
    tmp_g = 0;
    tmp_b = 0;

    cur_r = 0; // 表示当前显示的颜色分量，每次调用该函数，初始值都是0，由暗到亮
    cur_g = 0;
    cur_b = 0;

    time_interval_r = __get_time_interval(1, next_r); // 调整PWM的时间间隔
    time_interval_g = __get_time_interval(1, next_g);
    time_interval_b = __get_time_interval(1, next_b);

    // 灯光渐渐变亮
    for (i = 0; i < BREATHING_LIGHT_ON_TIME; i++)
    {
        // tmp_r++;
        // tmp_g++;
        // tmp_b++;

        tmp_r += 100;
        tmp_g += 100;
        tmp_b += 100;

        if (KEY_2_3_PIN == 0)
        {
            // 如果关闭了电源
            delay_ms(10);
            if (KEY_2_3_PIN == 0)
            {
                right_key_val = RIGHT_KEY_UP;
                return;
            }
        }
        else if (0 == KEY_1_1_PIN)
        {
            delay_ms(10);
            if (0 == KEY_1_1_PIN)
            {
                left_key_val = LEFT_EY_UP;
                // 如果开启了锁色
                return;
            }
        }
        else if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN) &&
                 (0 == SWITCH_CHANGE_FLAG))
        {
            // 打开感光，并且检测到是白天，才会进入，防止影响调整时间
            delay_ms(10);
            if (0 == KEY_2_2_PIN && 0 == LIGHT_SENSOR_PIN)
            {
                // 如果打开了感光，且检测到是白天
                RGB_ENABLE = 0; // 白天灯不亮
                SWITCH_CHANGE_FLAG = 1;
                return;
            }
        }

        // 如果到了调整的时间，并且当前颜色分量还没有超过目标颜色分量
        if (cur_r < next_r && tmp_r >= time_interval_r)
        {
            // tmp_r -= time_interval_r;
            tmp_r = 0;
            cur_r++;
            color_set_component_r(cur_r);
        }

        // 如果到了调整的时间，并且当前颜色分量还没有超过目标颜色分量
        if (cur_g < next_g && tmp_g >= time_interval_g)
        {
            // tmp_g -= time_interval_g;
            tmp_g = 0;
            cur_g++;
            color_set_component_g(cur_g);
        }

        // 如果到了调整的时间，并且当前颜色分量还没有超过目标颜色分量
        if (cur_b < next_b && tmp_b >= time_interval_b)
        {
            // tmp_b -= time_interval_b;
            tmp_b = 0;
            cur_b++;
            color_set_component_b(cur_b);
#if USE_MY_DEBUG
            P22D = ~P22D;
#endif
        }

        delay_ms(1);
    }

    // 直接显示当前颜色的最高亮度
    // color_show(next_color); // 直接显示最终的颜色，颜色可能会有跳变

    // P22D = 1;
    // 保持500ms
    for (i = 0; i < 500; i++) // 在保持500ms的时间内，顺便检测电源是否关闭
    {
        if (KEY_2_3_PIN == 0)
        {
            delay_ms(10);

            // 这里不用检测锁色，因为此时已经接近最大亮度

            if (KEY_2_3_PIN == 0)
            {
                right_key_val = RIGHT_KEY_UP;
                // 如果关闭了电源
                return;
            }
            else if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN) &&
                     (0 == SWITCH_CHANGE_FLAG))
            {
                // 打开感光，并且检测到是白天，才会进入，防止影响调整时间
                delay_ms(10);
                if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN))
                {
                    // 如果打开了感光，且检测到是白天
                    RGB_ENABLE = 0; // 白天灯不亮
                    SWITCH_CHANGE_FLAG = 1;
                    return;
                }
            }
        }

        delay_ms(1);
    }
    // P22D = 0;

    // 450ms渐渐熄灭
    tmp_r = 0;
    tmp_g = 0;
    tmp_b = 0;

    time_interval_r = __get_time_interval(0, next_r); // 调整PWM的时间间隔
    time_interval_g = __get_time_interval(0, next_g);
    time_interval_b = __get_time_interval(0, next_b);

    if (0 == time_interval_r)
    {
        BREATHING_BREAK_R = 0;
    }
    else
    {
        BREATHING_BREAK_R = 1;
    }

    if (0 == time_interval_g)
    {
        BREATHING_BREAK_G = 0;
    }
    else
    {
        BREATHING_BREAK_G = 1;
    }

    if (0 == time_interval_b)
    {
        BREATHING_BREAK_B = 0;
    }
    else
    {
        BREATHING_BREAK_B = 1;
    }

    for (i = 0; i < BREATHING_LIGHT_OFF_TIME; i++)
    {
        if (KEY_2_3_PIN == 0) // 在450ms渐灭的时间内，顺便检测是否关闭了电源
        {
            delay_ms(10);
            if (KEY_2_3_PIN == 0)
            {
                right_key_val = RIGHT_KEY_UP;
                // 如果关闭了电源
                return;
            }
        }
        else if (0 == KEY_1_1_PIN)
        {
            delay_ms(10);
            if (0 == KEY_1_1_PIN)
            {
                left_key_val = LEFT_EY_UP;
                // 如果开启了锁色
                return;
            }
        }
        else if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN) &&
                 (0 == SWITCH_CHANGE_FLAG))
        {
            // 打开感光，并且检测到是白天，才会进入，防止影响调整时间
            delay_ms(10);
            if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN))
            {
                // 如果打开了感光，且检测到是白天
                RGB_ENABLE = 0; // 白天灯不亮
                SWITCH_CHANGE_FLAG = 1;
                return;
            }
        }

        // tmp_r++;
        // tmp_g++;
        // tmp_b++;
        tmp_r += 100;
        tmp_g += 100;
        tmp_b += 100;

        // 如果到了调整的时间，并且当前颜色分量还没有超过目标颜色分量
        if (cur_r > 0 && tmp_r >= time_interval_r)
        {
            // tmp_r -= time_interval_r;
            tmp_r = 0;
            cur_r--;
            color_set_component_r(cur_r);
        }

        if (cur_g > 0 && tmp_g >= time_interval_g)
        {
            // tmp_g -= time_interval_g;
            tmp_g = 0;
            cur_g--;
            color_set_component_g(cur_g);
        }

        if (cur_b > 0 && tmp_b >= time_interval_b)
        {
            // tmp_b -= time_interval_b;
            tmp_b = 0;
            cur_b--;
            color_set_component_b(cur_b);
#if USE_MY_DEBUG
            P22D = ~P22D;
#endif
        }

        if ((BREATHING_BREAK_R && 0 == cur_r) ||
            (BREATHING_BREAK_G && 0 == cur_g) ||
            (BREATHING_BREAK_B && 0 == cur_b))
        {
            // 如果有一个分量提前减到0，直接退出渐灭的状态，准备熄灭所有灯，
            // 防止灯暗下去时变色
            break;
        }

        delay_ms(1);
    }

    // 最后，熄灭RGB灯
    color[0] = 0x00;
    color[1] = 0x00;
    color[2] = 0x00;
    color_show(color); // 在这里会更新当前的颜色分量

    // 熄灭时间（熄灭时间要短，第一版交给客户的是200ms）
    for (i = 0; i < 50; i++)
    {
        if (KEY_2_3_PIN == 0)
        {
            delay_ms(10);
            if (KEY_2_3_PIN == 0)
            {
                right_key_val = RIGHT_KEY_UP;
                // 如果关闭了电源
                return;
            }
        }
        else if (0 == KEY_1_1_PIN)
        {
            delay_ms(10);
            if (0 == KEY_1_1_PIN)
            {
                left_key_val = LEFT_EY_UP;
                // 如果开启了锁色
                return;
            }
        }
        else if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN) &&
                 (0 == SWITCH_CHANGE_FLAG))
        {
            // 打开感光，并且检测到是白天，才会进入，防止影响调整时间
            delay_ms(10);
            if ((0 == KEY_2_2_PIN) && (0 == LIGHT_SENSOR_PIN))
            {
                // 如果打开了感光，且检测到是白天
                RGB_ENABLE = 0; // 白天灯不亮
                SWITCH_CHANGE_FLAG = 1;
                return;
            }
        }
        delay_ms(1);
    }
}

// 呼吸模式
void mode_breathing(void)
{
    __color_breathing(color_buf[cur_color_index]);

    if (0 == SWITCH_CHANGE_FLAG)
    {
        cur_color_index++;
        if (COLOR_NUMS == cur_color_index) // 如果已经超出了存放颜色的数组的下标
        {
            cur_color_index = 0;
        }
    }
    else
    {
        // 检测到 __color_breathing() 中已经将标志位置一，现在将它清零
        SWITCH_CHANGE_FLAG = 0;
    }
}

void Sys_Init(void)
{
    GIE = 0;
    CLR_RAM();
    IO_Init(); // 在其中已经配置了开关、光敏器件

    timer0_pwm_config(); // 控制蓝色的PWM配置
    timer1_pwm_config(); // 控制绿色的PWM配置
    timer2_pwm_config(); // 控制红色的PWM配置

    GIE = 1;
}

/************************************************
;  *    @函数名            : main
;  *    @说明              : 主程序
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void main(void)
{
    Sys_Init();

    color[0] = 0x00;
    color[1] = 0x00;
    color[2] = 0x00;
    color_show(color);   // 黑色，灯不亮
    cur_color_index = 0; // 当前显示的颜色对应的下标，初始化为0

#if 0  // 测试用的引脚
    // OEP1 |= 0xE0; // 重新将IO配置为输出模式
    // P15D = 0;
    // P16D = 0;
    // P17D = 0;

    // P22D = 0;


    // P04D = 0;
    // P05D = 0;
    // P20D = 0;

    // P23D = 0;
    // P22D = 0;
    // P21D = 0;

    // delay_ms(5000);
#endif // 测试用的引脚

    while (1)
    {

#if 0  // 测试单个颜色时使用
       // color[0] = color_buf[0][0];
       // color[1] = color_buf[0][1];
       // color[2] = color_buf[0][2];
       // color_show(color); // 调用该函数会直接更新当前显示的颜色的分量
       // delay_ms(1000);
#endif // 测试单个颜色时使用

#if 1
        key_scan(); // 扫描开关状态

        // 先检测电源是否打开
        if (RIGHT_KEY_UP == right_key_val)
        {
            // 如果关闭了电源
            color[0] = 0x00;
            color[1] = 0x00;
            color[2] = 0x00;
            color_show(color); // 黑色
            cur_color_index = 0;

            while (RIGHT_KEY_UP == right_key_val) // 等待电源打开，再做其他动作
            {
                key_scan(); // 扫描开关状态
            }
        }

        // 再检测是否打开感光
        if (RIGHT_KEY_MID == right_key_val)
        {
            // 如果打开了感光
            if (LIGHT_SENSOR_PIN)
            {
                // 检测到是晚上--高电平
                RGB_ENABLE = 1; // 晚上灯亮
            }
            else
            {
                // 检测到是白天--低电平
                RGB_ENABLE = 0; // 白天灯不亮
            }
        }
        else if (RIGHT_KEY_BOTTOM == right_key_val)
        {
            // 如果关闭了感光
            RGB_ENABLE = 1; // 白天、晚上，灯都亮
        }

        // 如果灯可以亮，再检测是否锁色
        if (RGB_ENABLE)
        {
            if (LEFT_EY_UP == left_key_val)
            {
                // 如果是要锁色
                LOCK_FLAG = 1;
            }
            else
            {
                LOCK_FLAG = 0;
            }

            if (LEFT_KEY_MID == left_key_val && 0 == LOCK_FLAG)
            {
                // 渐变模式
                mode_gradient();
            }
            else if (LEFT_KEY_BOTTOM == left_key_val && 0 == LOCK_FLAG)
            {
                // 呼吸模式
                mode_breathing();
            }
            else if (1 == LOCK_FLAG)
            {
                // 如果开启了锁色，显示当前颜色，亮度最大
                if (cur_color_index > 0)
                {
                    cur_color_index--; // 渐变/呼吸模式中会把这个下标加一，这里要减一
                }
                // __color_gradient(color_buf[cur_color_index]);
                // color_show(color_buf[cur_color_index]); // 直接显示对应的颜色

                color[0] = color_buf[cur_color_index][0];
                color[1] = color_buf[cur_color_index][1];
                color[2] = color_buf[cur_color_index][2];
                color_show(color); // 调用该函数会直接更新当前显示的颜色的分量

                while (LEFT_EY_UP == left_key_val && RIGHT_KEY_UP != right_key_val)
                {
                    key_scan();
                }
            }
        }
        else
        {
            // 如果灯不可以亮
            color[0] = 0x00;
            color[1] = 0x00;
            color[2] = 0x00;
            color_show(color); // 黑色
        }
#endif

#if 0

        // color_show(color_buf[2]);
        // mode_breathing();
        // mode_gradient();

        // 开关功能测试：
        // P21D = KEY_2_1_PIN; // 这里是可以检测到引脚对应的电平的
        // P22D = KEY_2_2_PIN;
        // P23D = KEY_2_3_PIN;

        // 开关状态扫描函数测试：
        // 原来是检测不到右侧开关的状态的，现在调试再修改后就可以了
        // if (RIGHT_KEY_UP == right_key_val)
        // {
        //     P21D = ~P21D;
        // }
        // else if (RIGHT_KEY_MID == right_key_val)
        // {
        //     P22D = ~P22D;
        // }
        // else if (RIGHT_KEY_BOTTOM == right_key_val)
        // {
        //     P23D = ~P23D;
        // }

        // 测试通过，可以检测到左边开关的状态
        // if (LEFT_EY_UP == left_key_val)
        // {
        //     P21D = ~P21D;
        // }
        // else if (LEFT_KEY_MID == left_key_val)
        // {
        //     P22D = ~P22D;
        // }
        // else if (LEFT_KEY_BOTTOM == left_key_val)
        // {
        //     P23D = ~P23D;
        // }

        // 呼吸模式测试：
        // color[0] = 0xFF;
        // color[1] = 0xFF;
        // color[2] = 0xFF;
        // __color_breathing(color);
        // P21D = ~P21D;

        // color[0] = 0x77;
        // color[1] = 0x77;
        // color[2] = 0x77;
        // __color_breathing(color);
        // P21D = ~P21D;

        // color[0] = 0xBB;
        // color[1] = 0xBB;
        // color[2] = 0xBB;
        // __color_breathing(color);
        // P21D = ~P21D;

        // color[0] = 0x22;
        // color[1] = 0x22;
        // color[2] = 0x22;
        // __color_breathing(color);
        // P21D = ~P21D;

        // delay_ms(500);

#endif

    } // while (1)
}

/************************************************
;  *    @函数名            : interrupt
;  *    @说明              : 中断函数
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void int_isr(void) __interrupt
{
    __asm;
    movra _abuf;
    swapar _PFLAG;
    movra _statusbuf;
    __endasm;

    // if (T1IF & T1IE)
    // {
    //     T1IF = 0;
    //     // timer1_cnt++; // 定时器1计数值++
    // }

    __asm;
    swapar _statusbuf;
    movra _PFLAG;
    swapr _abuf;
    swapar _abuf;
    __endasm;
}

/**************************** end of file *********************************************/
