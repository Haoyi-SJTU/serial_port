# USB Relay Controller (Windows10/11)

This project is applicable to the opening and closing control of the LCUS-1 USB relay, or any other relay module that is controlled to open and close via a serial port.

A **lightweight** program that uses the native Windows C++ API (`<windows.h>`) to send hexadecimal commands via a CH340 serial module (thereby enabling functions like relay pull-in/disconnection) in a Windows environment. Below is a **from-scratch** operation guide.

<img align="left" width="88" src="https://github.com/Haoyi-SJTU/serial_port/blob/main/fig/Snipaste_2026-03-12_17-25-10.png" />

**This project is packaged into an EXE program. You can use it directly without compilation.**

(The EXE program is available at this repository. **You can download and run the software directly in [release](https://github.com/Haoyi-SJTU/serial_port/releases/tag/release-v1.0)**. The software supports serial port selection, on/off switching, and timed shutdown. This software may be falsely flagged or accidentally deleted by your computer's firewall. If this happens, please temporarily disable your firewall and try again.)

<img src="https://github.com/Haoyi-SJTU/serial_port/blob/main/fig/Snipaste_2026-03-12_17-24-09.png" width="360px"><img src="https://github.com/Haoyi-SJTU/serial_port/blob/main/fig/Snipaste_2026-03-12_17-24-40.png" width="360px">

## 🛠️ 1. Development Environment

### 1.1 C++ Compiler (MinGW-w64)
1. Go to the [MSYS2 Official Website](https://www.msys2.org/) to download the installer, and install it to the default path (usually `C:\msys64`).
2. After installation, open the **MSYS2 UCRT64** terminal.
3. Copy and run the following command to install the GCC toolchain:
   ```bash
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   ```

When prompted, enter Y and press Enter to confirm the installation.

### 1.2 Configure System Environment Variables
Press the Win key, search for "Environment Variables", and select "Edit the system environment variables". Click on "Environment Variables" in the bottom right corner, find Path in the "System variables" list, and double-click to edit. Click "New", and add the path to the compiler's bin directory:

   ```Bash
   C:\msys64\ucrt64\bin
   ```
Save and exit. Restart VS Code to apply the environment variables.

### 1.3 Install Recommended VS Code Extensions
Search for and install the following in the VS Code Extensions Marketplace:

 - C/C++ (Microsoft): Provides code highlighting, auto-completion, and debugging.
 - Code Runner (Jun Han): Provides a convenient one-click run button in the top right corner.

## ⚙️ 2. Hardware and Code Configuration
### 2.1 Confirm Serial Port (COM Port)
Before running the code, plug the CH340 module, and use a serial port assistant software (eg, [ALITHON](https://www.alithon.com/downloads)) to confirm the COM port number. 

### 2.2 Modify, Compile, and Run
Open the serial.cpp file and modify the following parameters according to your actual situation:

 - Serial Port: CreateFileA("\\\\.\\COM10", ...) (Be sure to keep the \\\\.\\ prefix to support COM10 and above ports).

 - Baud Rate: The default configuration is CBR_9600.

 - Hexadecimal Commands: Modify the data1 (turn on) and data2 (turn off) arrays according to your relay protocol.

Run the program:
   ```Bash
   .\serial.exe
   ```
## 4. Common Issues
 - Error: "Cannot open serial port"

   Check if the device is securely plugged in.

   Check if the COM port in the "Device Manager" perfectly matches the one entered in the code (CreateFileA). Note: Even if the code specifies opening COM10, if the printed error message says COM9, ensure your code logic and print statements are consistent.

   Check if other programs (such as a "Serial Port Debugging Assistant") are currently occupying the port.

 - Delay is too short: The current code has Sleep(120); (120 milliseconds) between the two commands. If the hardware responds slowly, try extending it to Sleep(1000)


## 5. Blog

[csdn blog](https://blog.csdn.net/tuck_frump/article/details/158974435?fromshare=blogdetail&sharetype=blogdetail&sharerId=158974435&sharerefer=PC&sharesource=tuck_frump&sharefrom=from_link)

---

# USB继电器控制器（Windows10/11）

本项目适用于LCUS-1型USB继电器的开合控制，或其它任何通过串口控制开合的继电器模块。

 Windows 环境下使用 C++ 原生 API (`<windows.h>`) 通过 CH340 串口模块发送十六进制指令（进而实现继电器吸合/断开等功能）的**轻量级**程序。以下是**从零开始**的操作指南。

<img align="left" width="88" src="https://github.com/Haoyi-SJTU/serial_port/blob/main/fig/Snipaste_2026-03-12_17-25-10.png" />

**This project is packaged into an EXE program. You can use it directly without compilation.**

(本项目已封装为EXE程序，**您可以直接在[release](https://github.com/Haoyi-SJTU/serial_port/releases/tag/release-v1.0)下载软件、运行，为您省去所有环境配置、源码编译的过程**。软件支持串口选择、开关开启、关闭及定时关闭。软件在windows 10、windows 11上测试正常运行。该软件可能被计算机防火墙误报或误删，如出现该情况请暂时关闭防火墙后再次尝试。)

<img src="https://github.com/Haoyi-SJTU/serial_port/blob/main/fig/Snipaste_2026-03-12_17-24-09.png" width="360px"><img src="https://github.com/Haoyi-SJTU/serial_port/blob/main/fig/Snipaste_2026-03-12_17-24-40.png" width="360px">

## 🛠️ 1. 开发环境

### 1.1 C++ 编译器 (MinGW-w64)
1. 前往 [MSYS2 官网](https://www.msys2.org/) 下载安装程序，并按默认路径（通常为 `C:\msys64`）安装。
2. 安装完成后，打开 **MSYS2 UCRT64** 终端。
3. 复制并运行以下命令以安装 GCC 工具链：
   ```bash
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   ```
   遇到提示时，输入 Y 并回车确认安装

### 1.2 配置系统环境变量
按下 Win 键，搜索“环境变量”，选择“编辑系统环境变量”。点击右下角“环境变量”，在“系统变量”列表中找到 Path，双击编辑。点击“新建”，添加编译器的 bin 目录路径：
   ```bash
   C:\msys64\ucrt64\bin
   ```
保存并退出。重启 VS Code 以使环境变量生效。

### 1.3 安装 VS Code 推荐插件
在 VS Code 扩展商店中搜索并安装：

 - C/C++ (Microsoft)：提供代码高亮、补全和调试。

 - Code Runner (Jun Han)：提供右上角一键运行代码的便捷按钮。

## ⚙️ 2. 硬件与代码配置
### 2.1 确认串口号 (COM Port)
在运行代码前，请将 CH340 模块插入，使用串口助手软件查看串口号（如[友善串口助手](https://www.alithon.com/downloads)，嘎嘎好用。）。

### 2.2 修改,编译与运行
打开 serial.cpp 文件，根据实际情况修改以下参数：

 - 串口号：CreateFileA("\\\\.\\COM10", ...) (务必保留 \\\\.\\ 前缀以支持 COM10 及以上端口)。

 - 波特率：默认配置为 CBR_9600。

 - 十六进制指令：根据继电器协议修改 data1 (开启) 和 data2 (关闭) 数组。

运行程序
   ```bash
   .\serial.exe
   ```


## 4. 常见问题
 - 报错：“错误：无法打开串口”

   检查设备是否插好。

   检查“设备管理器”中的 COM 口是否与代码中 (CreateFileA) 填写的完全一致。注意：代码中虽然写着打开 COM10，但打印出来的错误提示写的是 COM9，请确保代码逻辑与打印提示统一。

   检查是否有其他程序（如“串口调试助手”）正在占用该端口。

 - 延时过短：当前代码在两条指令间有 Sleep(120); (120毫秒)。如果硬件反应慢，可尝试延长至 Sleep(1000);。

## 5. 项目博文

[csdn博文](https://blog.csdn.net/tuck_frump/article/details/158974435?fromshare=blogdetail&sharetype=blogdetail&sharerId=158974435&sharerefer=PC&sharesource=tuck_frump&sharefrom=from_link)
