/******************************************************************************
;  *       @型号                   : MC32F7361
;  *       @创建日期               : 2021.12.21
;  *       @公司/作者              : SINOMCU-FAE
;  *       @晟矽微技术支持         : 2048615934
;  *       @晟矽微官网             : http://www.sinomcu.com/
;  *       @版权                   : 2021 SINOMCU公司版权所有.
;  *---------------------- 建议 ---------------------------------
;  *                   变量定义时使用全局变量
******************************************************************************/
#ifndef USER
#define USER
#include "mc32-common.h"
#include "MC32F7361.h"

/*****************************************************************
;       Function : Define variables
;*****************************************************************/

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

#define DEF_SET_BIT0 0x01
#define DEF_SET_BIT1 0x02
#define DEF_SET_BIT2 0x04
#define DEF_SET_BIT3 0x08
#define DEF_SET_BIT4 0x10
#define DEF_SET_BIT5 0x20
#define DEF_SET_BIT6 0x40
#define DEF_SET_BIT7 0x80

#define DEF_CLR_BIT0 0xFE
#define DEF_CLR_BIT1 0xFD
#define DEF_CLR_BIT2 0xFB
#define DEF_CLR_BIT3 0xF7
#define DEF_CLR_BIT4 0xEF
#define DEF_CLR_BIT5 0xDF
#define DEF_CLR_BIT6 0xBF
#define DEF_CLR_BIT7 0x7F

#define USE_MY_DEBUG 0 // 是否打开调试（至少占用87字节）

// 引脚定义
#define RGB_PIN_RED P15D
#define RGB_PIN_GRE P17D
#define RGB_PIN_BLE P16D

#define KEY_1_1_PIN P01D // 开关K1_1 -- 检测是否开启锁色 -- 对应 左侧开关--上档
#define KEY_1_2_PIN P02D // 开关K1_2 -- 检测是否开启渐变 -- 对应 左侧开关--中档
#define KEY_1_3_PIN P00D // 开关K1_3 == 检测是否开启呼吸 -- 对应 左侧开关--下档

#define KEY_2_1_PIN P14D // 开关K2_1 -- 检测是否关闭感光 -- 对应 右侧开关--下档
#define KEY_2_2_PIN P13D // 开关K2_2 -- 检测是否打开感光 -- 对应 右侧开关--中档
#define KEY_2_3_PIN P12D // 开关K2_3 -- 检测是否关闭了电源 -- 对应 右侧开关--上档

#define LIGHT_SENSOR_PIN P11D // 光敏器件连接的引脚，0--检测到亮，1--检测到暗

// 按红->紫->蓝->绿->黄->橙的顺序，对各个颜色的RGB值进行定义

// #define GRADIENT_TIME 1800 // 渐变时，从当前颜色切换到下一个颜色的时间（单位：ms）
#define GRADIENT_TIME 1800
// #define GRADIENT_TIME 36000 // 渐变时会变得很慢，而且无法对开关变化及时做出响应
// #define WEIGHT_VAL 10000   // 权值，必须要大于上面的这个时间值，并且最高位为1，其余位是0
#define WEIGHT_VAL 100 //

#define BREATHING_LIGHT_ON_TIME 850  // 呼吸模式下，灯光渐渐变亮的时间（ms）
#define BREATHING_LIGHT_OFF_TIME 450 // 呼吸模式下，灯光渐渐熄灭的时间（ms）

const u8 color_buf[][3] = {

#if 0 // 客户删掉的，显示效果不好的颜色

// {0xFF, 0x99, 0xCC}, // 浅粉色 // 是蓝色调的
// {0xFF, 0x66, 0xFF}, // 浅紫色 // 是蓝色调的
// {0xE5, 0xCC, 0xFF}, // 浅薰衣草紫 // 是蓝色调的
// {0x22, 0x8B, 0x22}, // 黄绿色 -- 它更像 蓝绿色，但是亮度较低
// {0x66, 0xB2, 0xFF}, // 浅蓝色 -- 偏向蓝色色调
// {0x7F, 0xFF, 0xD4}, // 浅绿色 -- 偏向蓝色色调
// {0xFF, 0xD7, 0x00}, // 金色 -- 实际上是翠绿色 / 黄绿色

// {0xFF, 0xCE, 0xA6}, // 4000K色温 -- 实际上偏向蓝色
// {0xFF, 0xCF, 0xA7}, // 4000K的数据减少了一点分量，但还是偏向蓝色
// {0xFF, 0xB1, 0x6E}, // 3000K色温 -- 实际上是天蓝色，并且能看到明显的红色分量
// {0xFF, 0xE4, 0xCE}, // 5000K色温 -- 实际测得也是天蓝色

// {0xFF, 0xFF, 0x00}, // 黄色 -- 实际测试是翠绿色
// {0xFF, 0xA5, 0x00}, // 橙色 -- 实际测试是黄绿色

// {255, 100, 0}, // 黄色
#endif

    // {0xFF, 0xCE, 0x53}, // 在4000K色温的基础上削弱了蓝色分量，看起来偏白，将R脚接VDD后，更像蓝绿色

    // {0xFF, 103, 83}, //  -- 偏向白、蓝
    // {0xFF, 103, 42}, //  -- 趋近白色，没有其他分量
    // {0xFF, 105, 50}, // 白色 ，严格一点的话，可能会有些红色、蓝色分量

    // {255, 28, 18},
    // {255, 29, 19},
    // {255, 30, 20},
    // {255, 32, 22},
    {255, 33, 23}, // 可能是最接近400K的 --------------------------------
    // {255, 38, 28}, // 有些偏亮，不像4000K
    // {255, 43, 33}, // 有些偏亮，不像4000K

    {102, 39, 191}, // 中蓝色 --------------------------------

    // {0x33, 0x99, 0xFF}, // 中蓝色 -- 实际测试有点像亮蓝色
    {0x00, 0x00, 0xFF}, // 纯蓝色 -- 实际测试通过 --------------------------------
    {0x00, 0xFF, 0xFF}, // 蓝绿色 -- 会比中蓝色还亮 --------------------------------
    {0x00, 0xFF, 0x00}, // 绿色 -- 实际测试通过 --------------------------------

    // -- {255, 127, 0}, // 翠绿色，说明绿色颜色分量较大
    // -- {255, 100, 0}, // 黄绿色
    // -- {255, 90, 0}, // 黄绿色
    // -- {255, 80, 0}, // 黄色，有点绿色分量
    // -- {255, 75, 0}, // 黄色，还是有绿色分量
    {255, 65, 0}, // 黄色 -- 可能还不够亮 --------------------------------
    // {255, 65, 1}, // 黄色会变淡
    // {255, 65, 5}, // 黄色非常淡，整体偏白
    // -- {255, 60, 0}, // 黄色，带点橙色分量
    // -- {255, 55, 0}, // 黄色，带点橙色分量
    // -- {255, 50, 0}, // 偏向橙色
    // -- {255, 45, 0}, // 偏向橙色

    // {255, 35, 0}, // 橙色，可能有点偏黄
    // {255, 30, 0}, //
    {255, 25, 0}, // 橙色， --------------------------------
    // {255, 20, 0}, // 可能有点偏红

    {0xFF, 0x00, 0x00}, // 红色 -- 实际测试也是红色 --------------------------------

    // {0xFF, 0x00, 0x7F}, // 粉红色 -- 实际测试，蓝紫色
    {255, 0, 32}, // 粉色 --------------------------------

    // {0x99, 0x33, 0xFF}, // 纯紫色 -- 实际测试，蓝色
    // {127, 0, 255}, // 蓝色，只有少许紫色分量
    // {127, 13, 157}, // 蓝色，带少许紫色
    // {255, 0, 157}, // 紫色，颜色不够深，带点粉红
    // {255, 13, 127}, // 紫色测试 ,颜色不够深，偏白
    // {255, 13, 157}, // 紫色颜色不够深，偏淡

    // {255, 39, 157}, // 紫色偏白
    // {255, 0, 191}, // 紫色，带点粉红，还是颜色不够深
    {255, 0, 255}, // 紫色，颜色不够深，偏白，而且有蓝色分量 --------------------------------
    // {255, 10, 255}, // 紫色，颜色不够深，偏白，
    // {255, 20, 255}, // 紫色会越来越淡  |
    // {255, 30, 255}, // 紫色会越来越淡  |
    // {255, 40, 255}, // 紫色会越来越淡 \|/

    // {255, 13, 191}, // 紫色偏蓝
    // {255, 26, 191}, // 紫色偏蓝
    // {255, 13, 255}, // 更加偏蓝
};
#define COLOR_NUMS 10

u16 i; // 循环计数值

//===============Field Protection Variables===============
u8 abuf;
u8 statusbuf;

//===============Global Variable===============
// 定义开关的状态
enum
{
    LEFT_EY_UP = 1,  // 左侧开关，上档--锁色
    LEFT_KEY_MID,    // 左侧开关，中档--渐变模式
    LEFT_KEY_BOTTOM, // 左侧开关，下档--呼吸模式
    RIGHT_KEY_UP,    // 右侧开关，上档--关闭电源
    RIGHT_KEY_MID,   // 右侧开关，中档--打开感光
    RIGHT_KEY_BOTTOM // 右侧开关，下档--关闭感光
};
volatile u8 left_key_val;  // 左侧开关状态
volatile u8 right_key_val; // 右侧开关状态

volatile u8 cur_r; // 当前显示的红色分量的值
volatile u8 cur_g; // 当前显示的绿色分量的值
volatile u8 cur_b; // 当前显示的蓝色分量的值

volatile u8 cur_color_index; // 当前显示的颜色的下标/索引，对应颜色数组中的颜色下标

u8 color[3]; // 存放一种颜色的数组变量

u8 next_r;
u8 next_g;
u8 next_b;

volatile u16 tmp_r; // 临时变量 ，在呼吸模式中，负责控制每隔固定的时间调节一次当前RGB的亮度
volatile u16 tmp_g;
volatile u16 tmp_b;

u16 time_interval_r;
u16 time_interval_g;
u16 time_interval_b; // 调整PWM的时间间隔

//===============Global Function===============
void Sys_Init(void);
void CLR_RAM(void);
void IO_Init(void);

void delay_ms(u32 xms); // 毫秒级延时

// void key1_config(void); // 开关K1配置（初始化）
// void key2_config(void); // 开关K2配置（初始化）

void key_scan(void); // 开关状态扫描

void timer0_pwm_config(void); // pwm0--负责控制B，蓝色（默认占空比为100）
void timer1_pwm_config(void); // pwm1--负责控制G，绿色（默认占空比为100）
void timer2_pwm_config(void); // pwm2--负责控制R，红色（默认占空比为100）
void pwm_set_duty(u8 pwm_x, const u8 duty);

// void light_sensor_config(void); // 光敏器件的配置

void color_set_component_r(const u8 component_r_val); // 设置颜色的红色分量
void color_set_component_g(const u8 component_g_val); // 设置颜色的绿色分量
void color_set_component_b(const u8 component_b_val); // 设置颜色的蓝色分量
void color_show(u8 *rgb_val);                         // 显示单种颜色
void __color_gradient(u8 *next_color);                // 颜色渐变--从一种颜色变化为另一种颜色

void mode_gradient(void); // 渐变模式

void __color_breathing(u8 *next_color); // 呼吸模式下，切换到指定的颜色
void mode_breathing(void);              // 呼吸模式
//============Define  Flag=================
typedef union
{
    unsigned char byte;
    struct
    {
        u8 bit0 : 1;
        u8 bit1 : 1;
        u8 bit2 : 1;
        u8 bit3 : 1;
        u8 bit4 : 1;
        u8 bit5 : 1;
        u8 bit6 : 1;
        u8 bit7 : 1;
    } bits;
} bit_flag;
volatile bit_flag flag1;
volatile bit_flag flag2;

// 定义渐变时控制颜色变化方向的标志位
#define DIR_R flag1.bits.bit0 // 红色分量的变化方向，0--变小，1--变大
#define DIR_G flag1.bits.bit1 // 绿色分量的变化方向，0--变小，1--变大
#define DIR_B flag1.bits.bit2 // 蓝色分量的变化方向，0--变小，1--变大

#define RGB_ENABLE flag1.bits.bit3   // RGB灯是否使能的标志
#define POWER_ENABLE flag1.bits.bit4 // 电源是否使能的标志
#define LOCK_FLAG flag1.bits.bit5    // 锁色标志位

// 开关状态是否变化的标志位（主要用来处理开启锁色、开启感光、关闭电源的处理）
// 0--未变化，1--变化（需要进行相应的处理）
#define SWITCH_CHANGE_FLAG flag1.bits.bit6

// 下面的三个标志位一同控制，是否要停止呼吸渐灭的的状态
#define BREATHING_BREAK_R flag2.bits.bit0 // 1--要对该颜色分量进行判断，如果为0了，就要跳出呼吸渐灭的模式，防止颜色变化
#define BREATHING_BREAK_G flag2.bits.bit1 //
#define BREATHING_BREAK_B flag2.bits.bit2 //

#endif

/**************************** end of file *********************************************/
