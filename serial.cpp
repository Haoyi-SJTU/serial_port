#include <iostream>
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    // 1. 打开串口 COM9
    // 注意：在Windows API中，为了兼容COM10及以上的端口号，标准的写法是加上 "\\\\.\\" 前缀
    HANDLE hSerial = CreateFileA("\\\\.\\COM10",
                                 GENERIC_READ | GENERIC_WRITE, // 允许读和写
                                 0,                            // 独占访问
                                 NULL,                         // 默认安全属性
                                 OPEN_EXISTING,                // 必须打开已存在的设备
                                 FILE_ATTRIBUTE_NORMAL,        // 默认属性
                                 NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "错误：无法打开串口 COM9。" << std::endl;
        std::cerr << "请检查CH340是否已连接，或者串口是否被其他程序（如串口助手）占用。" << std::endl;
        return 1;
    }

    // 2. 配置串口参数 (波特率 9600, 数据位 8, 停止位 1, 无校验)
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    // 获取当前的串口状态
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "错误：无法获取串口状态。" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // 设置我们需要的参数
    dcbSerialParams.BaudRate = CBR_9600;   // 波特率 9600
    dcbSerialParams.ByteSize = 8;          // 数据位 8
    dcbSerialParams.StopBits = ONESTOPBIT; // 停止位 1
    dcbSerialParams.Parity = NOPARITY;     // 无校验位

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "错误：无法设置串口参数。" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // 3. 准备要写入的十六进制数据
    BYTE data1[] = {0xA0, 0x01, 0x01, 0xA2};
    BYTE data2[] = {0xA0, 0x01, 0x00, 0xA1};
    DWORD bytesWritten; // 用于接收实际写入的字节数

    // 4. 写入第一组数据：0xA0 0x01 0x01 0xA2
    std::cout << "开" << std::endl;
    if (!WriteFile(hSerial, data1, sizeof(data1), &bytesWritten, NULL))
    {
        std::cerr << "错误：写入第一组数据失败。" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // 5. 等待 1 秒 (1000 毫秒)
    std::cout << "等待 1 秒..." << std::endl;
    Sleep(120);
    // 9.46 17.9 19
    // 9.33 20N 16.1
    // 8.62 17 18.9
    // 8.44 16.2 18 13 18.4 19
    // 7.6V 15N
    // 6.19 7 11 10.9
    // 6.1 10.4 6.7
    // 6V 5N
    // 5.7V 4N
    // >5.5V none

    // 6. 写入第二组数据：0xA0 0x01 0x01 0xA1
    std::cout << "正在发送第二组指令: A0 01 01 A1 ..." << std::endl;
    if (!WriteFile(hSerial, data2, sizeof(data2), &bytesWritten, NULL))
    {
        std::cerr << "错误：写入第二组数据失败。" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    Sleep(1500);

    std::cout << "指令发送完毕！" << std::endl;

    // 7. 释放资源，关闭串口
    CloseHandle(hSerial);
    return 0;
}