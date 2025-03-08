```markdown
# I2C 驱动开发笔记

## 硬件配置定义
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
```
