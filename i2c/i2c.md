
# I2C 驱动开发笔记
首先是要了解通信机制的基本原理，再来开发基本功能。i2c通信机制需要用到四条线（电源、接地、时钟、数据），在代码中只能对时钟、数据进行操作。根据i2c通信机制可以将i2c基本功能分为4类：
A.初始化（发送模式和接受模式）
B.起始/结束信号
C.发送/接收数据
D.发送或等待ACK
## 硬件配置定义
首先是要写头文件，对相关变量进行定义。
```c
#define GPIOX_DATA GPIOA
#define GPIOX_CLK GPIOA
#define PIN_DATA GPIO_PIN_2
#define PIN_CLK GPIO_PIN_2
#define RCC_DATA RCC_APB2Periph_GPIOA
#define RCC_CLK RCC_APB2Periph_GPIOA
```

## 功能实现

### 初始化配置
现在开始写基本功能：初始化函数，将端口设置为输出模式或者输入模式，以便后续的发送和接收数据。
#### 输出模式
```c
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_DATA, ENABLE);
GPIO_InitStructure.GPIO_Pin = PIN_DATA;     
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOX_DATA, &GPIO_InitStructure);
GPIO_SetBits(GPIOX_DATA,PIN_DATA);
RCC_APB2PeriphClockCmd(RCC_CLK, ENABLE);
GPIO_InitStructure.GPIO_Pin = PIN_CLK;     
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          
GPIO_Init(GPIOX_CLK, &GPIO_InitStructure);
GPIO_SetBits(GPIOX_CLK,PIN_CLK);
```
#### 输入模式
```c
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = PIN_DATA;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLUP;
HAL_GPIO_Init(GPIOX_DATA, &GPIO_InitStruct);
```

### 起始/停止信号
接下来开始写I2C协议中的起始/结束信号功能，下面代码的本质是想在时钟线高电平是数据线产生一个上升沿（结束信号）或下降沿（开始信号）的信号。
#### 开始信号
```c
GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
```

#### 结束信号
```c
GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
```

### 数据收发
接下来实现发送和接受数据功能，发送和接收8个比特的数据。整体逻辑是时钟线高电平时数据线的电平表示数据比特。
#### 发送数据
```c
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
for(uint8_t i=0;i<8;i++) {
    if(data & 0x80) {
        GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
    } else {
        GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
    }
    GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
    GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
    data <<=1;
}
```

#### 接收数据
```c
uint8_t data = 0;
for(uint8_t i=0;i<8;i++) {
    GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
    if(GPIO_ReadInputDataBit(GPIOX_DATA,PIN_DATA)) {
        data += 1;
    }
    GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
    data <<=1;
}
return data;
```

### ACK处理
I2C协议中每发送一个字节都要等待对方的一个ACK信号，每接收一个字节都要向对方发送一个ACK。ACK信号是当时钟线为高电平是数据线为低电平。
#### 发送ACK
```c
GPIO_WriteBit(GPIOX_DATA,PIN_DATA,1);
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
GPIO_WriteBit(GPIOX_DATA,PIN_DATA,0);
```

#### 等待ACK
```c
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,1);
if(GPIO_ReadInputDataBit(GPIOX_DATA,PIN_DATA)) {
    GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
    return 1;
}
GPIO_WriteBit(GPIOX_CLK,PIN_CLK,0);
return 0;
```
