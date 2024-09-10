USER:项目代码协议操作
  freertosconfig freertos设置
  lwipopts lwip设置
  user samp 切换通道及数据采样，sysdata
  user voltage ctrl da向器件发送电压
  user protocol 协议执行相关，其他项目只需按照函数格式编写，增添到order_and_function即可
  user aging order 对应协议各个老化命令执行的操作函数
  user task 采样流程处
  user alarm 采样报警


PERIPHERAL:外设驱动，改引脚和个数即可
  AD5044  上电烧录程序时会锁死无法操作只能下电上电重置
  mb88346 上电默认发满偏值
  ad8688  设置范围
  user PERIPHERAL gpio 读取版号，读取插板状态


BSP_DRIVER:主控板驱动，协议不改基本不需要改
  user all gpio init 大部分引脚初始化及置位0
  user cpld cpld设置 应该设置exmc_init_s.read_write_timing，但会导致flash无法操作，不设置也能用，速率慢一点
  user delay 200MHZ下不精确定时
  user updata 在线升级相关,由于无法单页擦写，所有无校验
  user time rtc及定时器相关，与协议关联
  user data processing 数据接收 协议校验 数据写入
  user tcp server tcpip处理线程
  user adc 芯片级adc相关



校准未写
