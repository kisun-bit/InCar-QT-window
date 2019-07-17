最近嵌入式课程设计将近， 经过两周的时间， 最终实现了大体要求， 其具体功能如下：

1. UART串口助手，能实现上位机与下位机之间的数据通信；
2. 实现了对媒体文件(音视频)的播放及相关辅助播放功能；
3. RTS实时时钟控制；
4. USB / SD/MMC 读取设备文件系统并可视化；
5. 拥有小游戏模块，扫雷；
6. 实现对led设备的控制（跑马灯、呼吸灯、普通模式开关灯等）；


以下为该次课程设计的具体实现步骤（图片缺失） ：


一、项目要求
1.配置QT5.7基于x86及arm 等两种CPU架构的调试及开发环境；
2.移植arm编译后的QT5.7及屏幕校准工具tslib1.4至CORTEX ARM9实验平台；
3.开发基于QT5.7的嵌入式音视频播放器；
4.开发基于QT5.7的嵌入式串口工具，实现数据双向通信；
5.能用Q5.7T在开发板上实现读取SD卡及U盘的文件（目录）信息，并以相关组件展示；
6.开发基于QT5.7的嵌入式LED io控制器，实现对LED设备的控制；
7.开发嵌入式QT小游戏模块;
8.QT开发计算器模块；
9.实现日期组件，能够修改系统(arm平台)时间并实时刷新；
二、项目开发环境
2.1 硬件环境
2.1.1开发端（PC） 
操作系统：64位ubuntu-19.04（虚拟机）
CPU    ：INTER_X86 core i5-6200 主频可加速至2.8GHZ
GPU	 ：NVIDIA GEFORCE-920M
虚拟硬盘：25GB
RAM	 ：4GB
2.1.2教学实验平台 
CPU：Samsung Exynos4412, 四核Cortex-A9,主频为1.4～1.6GHz
GPU: ARM Mali-400 MP Core (Quad Fragment processors)
操作系统：Linux 3.0

2.2 软件环境
2.2.1 编译QT5.7（ARM）
QT及相关工具下载
1.从http://download.qt.io/archive/qt/5.7/5.7.0/single/qt-everywhere-opensource-src-5.7.0.tar.gz下载linux下QT5.7源码版本（考虑到QT4.8.5中对音视频库的支持不够友好，我们直接选择版本更高的QT5并将平台上的QT4予以替换）
2.由于编译源码的需要，需要从https://www.veryarm.com/arm-none-linux-gnueabi-gcc下载交叉编译工具arm-none-linux-gnueabi-gcc 
3.实验平台屏幕校准工具tslib1.4：http://vdisk.weibo.com/s/AcUHxOERV2k0?sudaref=www.baidu.com
4.最后将下载成功的qt-everywhere-opensource-src-5.7.0.tar.gz、arm-linux-gcc-4.4.3-20100728.tar.gz及tslib1.4.tar.gz拷贝到虚拟机上。


交叉编译环境配置
解压arm-linux-gcc-4.4.3-20100728.tar.gz

执行tar  zxvf arm-linux-gcc-4.4.3-20100728.tar.gz  并将解压后的目录复制到/usr/local/arm/4.4.3下得到：

配置arm-gcc环境变量

重启之后执行arm-linux-gcc -v 
这里出现报错，查阅相关资料了解到该交叉编译工具仅支持32位系统，需安装依赖包
执行sudo apt-get install lib32ncurses5 lib32z1之后 验证arm-linux-gcc -v命令显示成功！

由此交叉编译环境搭建完毕。

交叉编译tslib1.4
进入tslib1.4压缩文件目录，并解压

安装完成进入tslib目录执行./configure命令

此处如果出现了报错，原因是autoreconf:notfound 则是缺失automake及autoconf库，
索性一次性安装所有make命令相关依赖包
执行sudo  apt-get  install  autoconf  automake  libtool  make libsysfs-dev 

这里我在解压后的tslib-1.4目录新建配置脚本autogen.sh，
写入 ./configure --prefix=/usr/local/tslib --host=arm-linuxac_cv_func_malloc_0_nonnull=yes并修改其文件模式为777
执行./autogem.sh

成功配置之后会在相应目录下生成Makefile文件，执行编译操作



如果在/user/local/tslib下生成了bin include等目录则证明编译成功！


交叉编译QT5.7
解压QT5.7在任意目录
命令行模式下进入该目录，修改qmake.conf配置参数
执行命令为vim ./qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf
并修改为以下内容：
-----------------------------------------------------------------------
MAKEFILE_GENERATOR      = UNIX
CONFIG                 += incremental
QMAKE_INCREMENTAL_STYLE = sublib
QT_QPA_DEFAULT_PLATFORM = linux #eglfs
QMAKE_CFLAGS_RELEASE += -O2 -march=armv9-a
QMAKE_CXXFLAGS_RELEASE += -O2 -march=armv9-a
include(../common/linux.conf)
include(../common/gcc-base-unix.conf)
include(../common/g++-unix.conf)
# modifications to g++.conf
QMAKE_CC                = arm-none-linux-gnueabi-gcc
QMAKE_CXX               = arm-none-linux-gnueabi-g++
QMAKE_LINK              = arm-none-linux-gnueabi-g++
QMAKE_LINK_SHLIB        = arm-none-linux-gnueabi-g++
# modifications to linux.conf
QMAKE_AR                = arm-none-linux-gnueabi-ar cqs
QMAKE_OBJCOPY           = arm-none-linux-gnueabi-objcopy
QMAKE_NM                = arm-none-linux-gnueabi-nm -P
QMAKE_STRIP             = arm-none-linux-gnueabi-strip
load(qt_config)
-------------------------------------------------------------------


创建一个脚本文件，用于生成Makefile，vim autocqt.sh,主要目的就是对qt进行裁剪和一些安装配置,脚本内容如下：
------------------------------------------------------------------
#!/bin/sh
./configure \
-v \
-prefix /user/local/Trolltech/QtEmbedded-4.6.3-arm \
-release \
-opensource \
-make libs \
-xplatform linux-arm-gnueabi-g++ \
-optimized-qmake \
-pch \
-qt-sql-sqlite \
-qt-libjpeg \
-qt-zlib \
-no-opengl \
-skip qt3d \
-skip qtcanvas3d \
-skip qtpurchasing \
-no-sse2 \
-no-openssl \
-no-nis \
-no-cups \
-no-glib \
-no-iconv \
-nomake examples \
-nomake tools \
-skip qtvirtualkeyboard \
-I/user/local/tslib/include \ #注意该路径为方才交叉编译tslib的目标目录下相应的include目录
-L/user/local/tslib/lib #注意该路径为方才交叉编译tslib的目标目录下相应的lib目录
Exit
---------------------------------------------------------------

执行./autocqt.sh

这里出现了错误，缺失libxtst-dev，执行apt-get还是不能安装

解决办法：进入软件包目录， 直接安装缓存的软件包

找到libx11-dev*和libxext-dev*

问题解决，提示如下信息则执行make命令


还是出现错误，提示如下信息，太不容易了，继续排错。

找到linux-gcc的位置，添加如下变量

不存在QMAKE_CXXFLAGS变量，则在../common/linux.conf、../common/gcc-base-unix.conf、../common/g++-unix.conf 这三个文件。如果没有找到还需要递归的打开着三个文件中包含的文件。 
在../common/gcc-base.conf （包含在文件../common/gcc-base-unix.conf 中）文件中找到，并修改为

../common/g++-base.conf

以及../common/gcc-base.conf，和以上文件一样的修改方式相同
修改之后运行成功查看 /user/local/Trolltech/QtEmbedded-4.6.3-arm 
qmake被成功构建

至此QT5.7交叉编译成功！
整理待移植目录到任意目录下

2.2.2 QT5.13测试环境（X86）配置
测试环境的配置相对简单
执行sudo apt-get install qtcreator qt5-default

安装成功！

2.2.3 QTcreator配置交叉编译环境
arm环境QT工具包配置
QTVersion：版本选择方才编译后的qmake路径（/user/local/Trolltech/QtEmbedded-4.6.3-arm/bin/qmake）;
QGCC：选择gcc为/opt/arm_gcc/bin/arm-linux-none-gnueabi-g++，为了区分重命名为arm_gcc；
QDebugger: /opt/arm_gcc/bin/arm-linux-none-gnueabi-gdb，为了区分重命名为arm_gdb；
KITS: 选择综上几步QT的环境依赖，QT(arm编译的版本)，GCC(arm_gcc),  QDebugger(arm_gdb)
X86-QT5.13测试环境配置
这里我们是采用命令行安装的x86-qt5.7的工具包，其会默认配置环境到/user/sbin/目录下，此时qtcreator能够自动检测出该qt包，所以直接选择Desktop(default)套件。
三、项目分析
3.1 硬件模块分析
1.串口（UART）通信模块有对串口数据的接收与发送、波特率的设置、端口的打开与关闭等功能，其上位机与下位机通信，需要解决两个问题，一个是通信数据的编码与解码方式，另一个是收发数据时的状态监听接口。
2.LED设备的控制模块有跑马灯、呼吸灯、普通开关灯等功能，由于QT本身采用C++实现，在ARM实验平台上安装LED设备驱动后，能够直接在QT程序中引入实现led控制的系统头文件，通过调用内核态的标准文件及IO处理库函数，达到控制LED设备的IO状态。
3.设备文件信息读取模块有展示文件信息，目录的打开与关闭等功能，对于SD卡以及U盘等设备的文件树读取，模块的核心基于linux系统的自动挂载特性，本质上就是递归目录并抽象成树结构，然后将该文件树可视化即可。
4.时钟控制模块有实时展示系统时间、修改设备时间等功能。QT5.7提供了修改系统时间的函数接口，但需要注意的是，执行程序的用户身份需要拥有root权限，否则无法修改对应系统时间的内核文件。其功能主要有实时展示系统时间

3.2 软件模块分析
1.音视频播放器模块有播放，暂停，快进等功能。其实现有两种方式，第一种是采用mplayer这一强大的linux媒体播放插件，通过在QT程序中另开进程执行相应的bash命令达到音视频的播放；另一种是采用qt原生媒体处理库，但是值得注意的是，在使用原生qt音视频播放库的时候，可能会出现媒体文件解码失败，导致白屏或者黑屏的情况，所以这里我们组采用第一种方式。
2.小游戏模块（扫雷）有方块点击识别，重新开始，方阵规格设置等功能，该游戏的核心在于随机生成雷点方阵，可以抽象化为一个矩阵，并以0代表类，随后扫描所有矩阵内元素，并在该位置填充提示雷点个数的信息。
3.计算器模块能够进行加减乘除等基本运算，能够取消当次运算操作。该类实现较简单，用变量保存每次的运算状态即可。
4.整体布局及界面。需要遵循操作友好，布局合理等。在web前端设计中，栅格布局的思想同样适用于QT界面设计，QT提供了丰富的布局类来实现布局。






四、设计与实现
4.1 设计思路
首先先将整个项目分为七大模块，然后分别针对这七个模块单独完成其核心功能。做到让整个项目模块化使项目整体具有清晰的层次结构

以下是各模块的设计思路以及介绍：



4.1.1 游戏模块：扫雷小游戏
模块介绍： 在设计之前，我们先来了解这款游戏的规则。 在不掀开任何藏有地雷的方块情况下，以最快的速度找出所有的地雷。如果在掀开方块的过程中，不小心翻开（踩到）藏有地雷的方块，则宣告失败（游戏结束），惟有将所有不含地雷的方块掀开，游戏才算成功。 游戏的操作方面主要以鼠标为主，当鼠标指针对准未翻开的方块按下左键时即表示翻开方块，当鼠标指针对准未翻开的方块按下右键时即表示标记或取消标记地雷，反复按下右键则方块会以标记，取消标记两者之间不断循环。游戏者可以通过地雷区内的数字提示了解以数字为中心的其周边八个方格内所含的地雷数，假若翻开的方块显示数字“3”，则表示以其为中心的周边方块内藏有3个地雷。当按下的方块不是地雷，且周边八个方块也都没有地雷时，方块会以被翻开方块的八个方向将空白方块翻开。 

模块功能：能够显示主菜单和界面，能够接收鼠标输入功能；能够根据规则翻转相应的格子；能够判断游戏胜负。

模块分析：流程规划大致上可以分为三个部分，分别为：游戏开始阶段、游戏进行阶段和游戏结束阶段。 游戏开始时，会以游戏者最后一次设定的地雷区大小为范围画出地雷区，同时会产生地雷并且开始计时。游戏进行阶段主要就是如何判断按下的方块是非地雷时的处理，这也是整个游戏的技术核心，我们可以通过扩散的观念来检查周边的方块是否含有地雷及是否继续往外翻开，以及鼠标右键的标记功能的实现。游戏结束阶段，判定游戏是否结束有两种情况，第一种是没有踩到雷，成功完成游戏，会弹出提示框提示赢得游戏，如果创造了新的纪录，则会弹出提示框提示输入姓名。第二种是踩到雷，也就是游戏失败，同样会弹出提示框来提示输掉游戏。  

设计思路：
1.使用int型双指针对象Map, Map成员变量用来保存雷区信息．以为雷区行数列数根据难易级别要动态改变，所以这里定义成指针，方便动态分配空间．当Map[x][y]取值为0-8时表示(x,y)这一格周围８格中的雷数，并且是已经被点开来了的，-1表示雷．若未被点开，则范围为99-108，99表示雷，100-108表示周围雷个数．若这个地方被插上小红旗，则范围为49-58．
2.布雷的实现主要依据于随机数，以数组矩阵的一维变量为一个循环层，分层生成其预定义矩阵宽度之内的随机雷点个数，记录其随机数作为雷点的位置索引，进行填雷，这里已-1代表雷。
3.提示信息的生成主要依据与上步中随机数组矩阵的雷点位置，循环每一个点，记录该点外层雷点个数，填充即可。


游戏模块流程图如下：




4.1.2 音视频播放器模块
模块介绍：用户通过该应用可以完整无误的播放音视频文件，并为用户提供一个简洁友好的用户界面。音视频播放器模块有播放，暂停，快进等功能。。

模块分析：音视频播放器模块有播放，暂停，快进等功能。其实现有两种方式，第一种是采用mplayer这一强大的linux媒体播放插件，通过在QT程序中另开进程执行相应的bash命令达到音视频的播放；另一种是采用qt原生媒体处理库，但是值得注意的是，在使用原生qt音视频播放库的时候，可能会出现媒体文件解码失败，导致白屏或者黑屏的情况，所以这里我们组采用第一种方式。下面是对音视频文件解码的分析：
1.PCM（脉冲编码调制）技术及声波的数字化
我们知道声音是一种连续变化的波形，是模拟信号，而我们的计算机或者说MCU存储及使用的都是二进制数字信号，所以我们需要对声音信号进行一些处理，这里便使用到了PCM技术，即脉冲编码调制，该技术属于通信原理或数字信号处理相关理论范畴，相关知识可以搜索脉冲编码调制关键字查看相关课件进行了解，不过需要一些相关理论知识，否则看不懂。

如图是一个使用PCM技术的将模拟信号转换成数字信号传输的系统方框图，现在我们主要看A/D模数转换区间内的内容，因为我们的音频数据就是根据这个过程得到的。这里我们需要解决三个部分的问题即采样频率、量化位数以及编码方式。 
采样频率 
常用有1025Hz(11kHz) ，22050Hz(22kHz)， 44100Hz(44kHz) 三种，我们知道人耳能够听到的声波频率为0~20kHz，根据抽样定理，我们需要大于等于两倍于声波频率的采样频率采样才能还原出原有的波形，所以对于声音采样为44kHz已经可以覆盖人耳可听到的声波范围。 
量化位数 
常用有8位、16位、24位，注意这里是二进制权，即8位实际表示来量化精度或者说量化间隔为1/256。 
编码方式 
数字通信系统中编码是为了更好的传输，而我们这里是为了便于使用和存储，因为编码方式不同就形成了不同格式的音频文件。 
声道 
我们知道声道是为了模拟人对声音的立体感，他是基于双耳效应的，它是音频所特有的，由于他是对双耳效应的模拟，我们一般是单声道输入，双声道输出，实际对于音频数据来讲就是将单声道的数据倍份为两份。


设计思路：
1.音视频播放主要采用mplayer插件，在qt程序中实例化QTprocess开启进程并使用mplayer（bash命令）来与系统交互；
2.Mplayer提供了多种模式，只需指定相关参数，其中重要的参数有，‘-wid’（linux系统会为每一个窗体对象派发一个windowId来唯一确定，使用该参数能够指定mplayer的媒体流的输入窗体对象，‘-quiet’静默模式播放，‘-slave’从模式播放等），另外还有很多交互模式下的命令来动态获取播放信息，‘pause\n’（暂停或者开启播放，我们需要在类中定义好媒体文件的播放状态），‘seek %s 1\n’（该命令主要控制媒体文件的播放进度）




音乐模块的功能模块图如下：





4.1.3  Led灯控模块
模块介绍：用户通过试验箱上得触屏可以控制LED的开关和其亮度的变化并能在一定时间间隔进行连续操作。硬件电路如下图所示，可以看出三个LED灯与GPF4,5,6相连接。且当io口输出为高电平时,led灯熄灭,当io输出低电平时,led灯点亮。

模块分析：硬件电路图分析

nLED_1, nLED_2, nLED_4, 给低电平则对应LED灯亮，高电平则对应LED灯灭，

S3C2440芯片GPF4-GPF6对应nLED_1, nLED_2, nLED_4，


设计思路：LED设备的控制模块有跑马灯、呼吸灯、普通开关灯等功能，由于QT本身采用C++实现，在ARM实验平台上安装LED设备驱动后，能够直接在QT程序中引入实现led控制的系统头文件，通过调用内核态的标准文件及IO处理库函数（sys/ioctl库、fcntl库），达到控制LED设备的IO状态。具备上述功能之后，跑马灯、呼吸灯的实现变得更为简单，实例化QTimer来控制实现不同LED的 IO状态。




4.1.4 RTC时钟控制模块
模块介绍：
实现在主菜单界面下的时钟实时刷新功能，并能够通过QT修改内核下的关于系统时间的相关文件，达到时钟控制的效果。

模块分析：
硬件逻辑分析：
RTC工作过程: 

RTC架构: 
图中浅灰色的部分都是属于备份域的,在VDD掉电时可在VBAT的驱动下继续运行.这部分仅包括RTC的分频器,计数器,和闹钟控制器.若VDD电源有效,RTC可以触发RTC_Second(秒中断)、RTC_Overflow(溢出事件)和RTC_Alarm(闹钟中断).从结构图可以看到到,其中的定时器溢出事件无法被配置为中断.如果STM32原本处于待机状态,可由闹钟事件或WKUP事件(外部唤醒事件,属于EXTI模块,不属于RTC)使它退出待机模式.闹钟事件是在计数器RTC_CNT的值等于闹钟寄存器RTC_ALR的值时触发的. 
因为RTC的寄存器是属于备份域,所以它的所有寄存器都是16位的.它的计数RTC_CNT的32位由RTC_CNTL和RTC_CNTH两个寄存器组成,分别保存计数值的低16位和高16位.在配置RTC模块的时钟时,把输入的32768Hz的RTCCLK进行32768分频得到实际驱动计数器的时钟TR_CLK = RTCCLK/37768 = 1Hz,计时周期为1秒,计时器在TR_CLK的驱动下计数,即每秒计数器RTC_CNT的值加1(常用)
由于备份域的存在,使得RTC核具有了完全独立于APB1接口的特性,也因此对RTC寄存器的访问要遵守一定的规则. 
系统复位后,禁止访问后备寄存器和RCT,防止对后卫区域(BKP)的意外写操作.(执行以下操作使能对后备寄存器好RTC的访问): 
1,设置RCC_APB1ENR寄存器的PWREN和BKPEN位来使能电源和后备接口时钟. 
2,设置PWR_CR寄存器的DBP位使能对后备寄存器和RTC的访问. 
设置为可访问后,在第一次通过APB1接口访问RTC时,必须等待APB1与RTC外设同步,确保被读取出来的RTC寄存器值是正确的,如果在同步之后,一直没有关闭APB1的RTC外设接口,就不需要再次同步了. 
如果内核要对RTC寄存器进行任何的写操作,在内核发出写指令后,RTC模块在3个RTCCLK时钟之后,才开始正式的写RTC寄存器操作.我们知道RTCCLK的频率比内核主频低得多,所以必须要检查RTC关闭操作标志位RTOFF当这个标志被置1时,写操作才正式完成. 

软件逻辑分析：
该模块基于QDateTime::currentTime()获取当前系统时间，其对象的setTime方法来修改系统时间，对于实时刷新功能，则采用定时器（QTimer）来控制实现。


4.1.5 计算器模块
模块分析：将计算器的计算流程分成三个部分，分别为数据采集与存储、计算结果以及显示结果。数据的存储采用数组的形式存储,在数据采集的同时并调用显示，所以将LED的显示写成一个函数，这样方便被调用，简化程序。同时，存储数据时将数值与运算符分开存储，即采用两个数组存储，这样方便计算部分程序的设计。在计算部分，为实现先乘除后加减的运算，先遍历一遍存储运算符的数组，先计算乘除，选出乘除运算符对应下标的数据计算结果放入乘号或除号后面那个存储单元，设为digit[i+1],将digit[i]数据单元赋值为0，再通过判断digit[i]数据单元前面的运算符号这里设为symbol[i-1]来选择数据digit[i+1]与digit[i]之间的运算符symbol[i]为加还是减，如果symbol[i-1]为减，则symbol[i]为减，否则为加。这样将数据数组和运算符数组经过计算乘除后重新赋值，在进行一次简单的加减运算便得到结果了。最后返回计算结果并显示。 

计算器模块功能图：




4.1.6 UART模块
模块介绍：该模块需要UART通信相关的寄存器的功能和设置方法，设置引脚复用，选择UART接收和发送对应的引脚用于UART通信，数据流格式设置，设置UART通信的数据帧格式，波特率等，设置UART部件的工作模式等。

模块分析：
硬件分析

我们可以从原理图上看到，教学平台的串口信号是由一块 MAX3380ECUP 芯片转换输出到 
COM 口上的。它的电气标准遵循 RS232 协议。教学平台上的 4 个串口端口，其中 UART2 和 
UART3 室插针接口。可以在教学平台上看到其标号。 串口的使用场景最常见的是，教学平台的串口信息调试。教学平台上电启动之后，无论是 Uboot 还是 kernel 内核还是 Android 系统，都会在其运行过程中打印输出一些信息到系统的 console 控制台。系统中指定 UART1 为调试串口。 424我们使用教学平台的 UART1 来输出打印信息到 PC 上的串口中，需要在 PC 端的串口终端上 配置一些参数。以 Ubuntu 系统的 minicom 串口终端为例： 串口通讯需要做一些配置，配置参数有上图显示中的波特率、硬件/软件控制位、通信位。上 图中的“115200 8N1” 代表的就是波特率设置为 115200，一帧传输 8 位数据，1 位停止位。波特率指的是信号被调制以后在单位时间内的变化，即单位时间内载波参数变化的次数。它是对符号传输速率的一种度量，1 波特即指每秒传输 1 个符号。

软件分析：
QtSerialPort库及QSerialPortInfo库提供了丰富的接口，例如串口扫描（QSerialPortInfo::availablePorts()）、设置数据校验位、设置波特率以及接收与发送数据等接口，对于数据的接收，QtSerialPort对象同样提供了readyRead()来监听数据接收区的缓冲流。
1.对发送操作来说，通过读取QCombox中的text()，来判定用户是否完成通信波特率以及串口的设置，若完成，则在对该text采用‘latin1’进行编码后发送。
2.对接收操作来说，QtSerialPort对象的readyRead()信号能够及时的收取接收缓冲区的数据，最后对QByteArray类型的缓冲数组进行清空操作。

4.1.7 USB HOST、SD/MMC模块
模块介绍：当用户通过该模块可实时查看USB的插入情况等。

模块分析：该模块应具有可以弹出该USB设备/SD卡读取界面的功能，并通过对单片机编程与PC机通过通用串行通讯接口进行通讯，并令PC机识别出一个USB存储设备，并以P2口作为PIO口控制NAND FLASH芯片的读写，且将NAND FLASH的容量大小通过USB发给PC机。工作模式是针对主机控制器来说的。也就是说，SD/MMC 控制器可以在符合 MMC 
的标准下工作，或者可以在符合 SD 的标准下工作，或者可以在符合 SDIO 的标准下工作。故 
就分别简称为：MMC 模式、SD 模式和 SDIO 模式。传输模式：传输模式也是针对主机控制器来说的，指控制器与卡之间数据的传输模式，或者说是总线类型。SD/MMC 控制器可支持 DMA 数据传输模式、1 位/4 位/8 位的三种传输模式(总线类型)。该

模块流程逻辑图：
                   


4.2 设计和实现的主要内容
4.2.1 主界面具体布局实现：

在web开发前端设计中，各大前端框架如bootstrap，easyUI等都有着栅格布局的思想，而QT对于其窗体程序的布局设计较之更为灵活，提供了大量布局类（诸如QHBoxLayout、QVBoxLayout、QGridLayout等）来支持。对页面设计十分友好。相较于在.cpp文件中用代码实现界面设计的方式，我们更倾向于在qt-designer中事先对界面进行排布，更加直观清晰。
4.2.2 游戏模块具体的实现
1.核心代码及其作用：

初始化：首先定义了一个类来实现扫雷相关操作．
class CMineMap
{
public:
    CMineMap();
    ~CMineMap();
    void Create();
    void Create(int,int,int);
    void Restart();
private:
    int ctnflag(int m, int n);

public:
    int **Map;
    int nMime;//雷数
    int mx;   //行数
    int my;   //列数
    bool Onrbtdown(int m, int n);
    bool Onlrbtdown(int m, int n);
    bool Onlbtup(int m, int n);
    bool iswin();
    int winf;
    int rMine;
    bool first;
    int timer;
};
初始化结构类解释：Map成员变量用来保存雷区信息．以为雷区行数列数根据难易级别要动态改变，所以这里定义成指针，方便动态分配空间．当Map[x][y]取值为0-8时表示(x,y)这一格周围８格中的雷数，并且是已经被点开来了的，-1表示雷．若未被点开，则范围为99-108，99表示雷，100-108表示周围雷个数．若这个地方被插上小红旗，则范围为49-58。

布雷：上述类中nMine表示雷的数量，在雷区范围内生成nMine个随机坐标，把对应坐标的赋值成99，在没有雷的地方统计周围的雷数，并赋值为100-108． 

点击事件 ：当一个格子（坐标为(x,y)）被点下，如果个格子周围有雷，则Map[x][y]值为101-108，将这个值减去100就行了．如果周围都没有雷，就会爆出一大片。
bool CMineMap::Onlbtup(int m, int n)
{
    if (Map[m][n] >= 101 && Map[m][n] <= 108)
    {
        Map[m][n] -= 100;
        first = 0;
        iswin();
        return true;
    }
    if (Map[m][n] == 100)
    {   //点击的地方周围都没雷，则递归点击周围８个
        Map[m][n] -= 100;
        Onlbtup(m - 1, n);
        Onlbtup(m + 1, n);
        Onlbtup(m, n - 1);
        Onlbtup(m, n + 1);
        Onlbtup(m - 1, n - 1);
        Onlbtup(m + 1, n - 1);
        Onlbtup(m - 1, n + 1);
        Onlbtup(m + 1, n + 1);
    }
    if (Map[m][n] == 99)
    {   // 中雷了
        ... // 相应处理函数
    }
    return true;
}

鼠标消息处理 ：Qt 中添加鼠标消息很容易，直接重载父类的mousePressEvent函数就好了． 
通过event->button()和event->buttons()可以获取到底是哪个键被按下． 
通过event->x()和event->y()获取鼠标按下的坐标．根据这个坐标计算出点到哪一格，然后调用扫雷类的相关操作．另外，当游戏失败或过关时，可以通过点击表情从新开始游戏。
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int px=event->x()-offsetx;
    int py=event->y() -offsety;
    int m = (px) / 20;
    int n = (py) / 20 - 2;
    if(event->buttons()==(Qt::LeftButton|Qt::RightButton))
    {
        if(mineMap.Onlrbtdown(m, n))
            update();
    }
    else if(event->button()==Qt::LeftButton)
    {
        if(mineMap.Onlbtup(m,n))
        {
            if (!(rumtime->isActive() ))
                rumtime->start(1000);//开始计时
            update();
        }
        if (mineMap.winf==0 || mineMap.winf == 2)
        {
            if (rumtime->isActive() )
                rumtime->stop();//停止计时
            if (px >  mineMap.mx * 10 - 15 && px< mineMap.mx * 10 + 15 && py>4 && py < 34)
            {
                mineMap.Restart();//点击表情图标重新开始
                update();
            }
        }
    }
    else if(event->button()==Qt::RightButton)
    {
        if(mineMap.Onrbtdown(m, n))
            update();
    }

2.模块展示实列图：


4.2.3 音视频播放模块的具体实现
1.核心代码及其作用（由于音视频均采用mplayer插件，这里我们就以视频播放器为例）：
文件选择：
QStringList  arr = QFileDialog::getOpenFileNames(this,
                                  "需要播放的文件",
                                  ".",
                                  "video (*.mp4 *.avi *.rmvb)");

播放功能：该功能主要依赖mplayer插件以及QProcess类，他们分别的作用是：1.mplayer控制音视频的播放，包括其进度，播放列表，媒体流输出窗体、暂停等等交互命令，2. QProcess另开了一个进程，用以执行mplayer命令(bash)， 此外与mplayer进程交互相关播放信息。
void VideoWidget::playMovie(const QString filename){
    playProcess->close(); // 该处process为QProcess的实例
    QString program = "mplayer";
    QStringList arguments;
    arguments << filename;
     //(mplayer -gui-wid<window_id>)  can play video in a gun widget
    arguments << "-wid"; // 表示视频流的输出窗体
    arguments << QString::number(ui->label_show->winId());//parse to string
    arguments << "-slave"; // 从模式下，支持wid投射视频流至qt窗体中
    arguments << "-zoom"; // 自适应
    arguments << "-quiet"; // 静默模式
    playProcess->start(program, arguments); //执行bash命令，设置mlayer相关控制参数
    isPlaying = 1;
    ui->btn_start->setIcon(QIcon(":/new/prefix1/res/stop.png"));
    QString cmd = QString("volumn %1 1\n").arg(ui->dial_volice->value()); // 初始音量调节
//            qDebug() << cmd << endl;
    playProcess->write(cmd.toLatin1());
    ui->horizontalSlider->setValue(0);
}

快进后退：QT中的QSlider控件支持value_change事件，当拖动操作发生时，我们考虑触发事件，随后转到该槽，执行mplayer的”seek %s 1\n”这一交互式命令来控制进度。
void VideoWidget::on_horizontalSlider_valueChanged(int value)
{
    if(isPlaying == 1 || isPlaying == 2){
        QString cmd = QString("seek %1 1\n").arg(value);
//        qDebug() << cmd << endl;
        playProcess->write(cmd.toLatin1()); 
    }
} 


音量调节：与快进后退的原理一致，这里直接来代码
void VideoWidget::on_dial_volice_valueChanged(int value)
{
//        qDebug() << value;
        if(isPlaying == 1 || isPlaying == 2){
            QString cmd = QString("volumn %1 1\n").arg(value);
//            qDebug() << cmd << endl;
            playProcess->write(cmd.toLatin1());
        }
} 












2.模块展示实列图：
视频播放器示例图

音频播放器实例图


4.2.4	Led灯控模块的具体实现
1.核心代码及其作用：
LED设备的控制：在实验教学平台上，在安装LED设备驱动之后，通过调用系统文件以及IO库函数(sys/fcntl.h 、sys/ioctl.h)，为验证LED设备驱动时候安装成功，可以进入/dev用find命令查找，在该arm平台上LED设备驱动已经安装成功，位置位于/dev/leds，所以直接只用ioctl来控制设备IO
void LedControl::open_led(){
    fd = ::open("/dev/leds", 0);
    if(fd < 0){
        QMessageBox::warning(nullptr, tr("LED"), tr("led 设备打开失败"));

    }else { 
        ::ioctl(fd, 1, 1);
        ::ioctl(fd, 1, 2);
        ::ioctl(fd, 1, 3);
        ::ioctl(fd, 1, 4);
    }
}
呼吸灯实现：呼吸灯依赖于定时器（QTimer），这里我们采用的方式是用一个全局变量count来记录QTimer对象中的槽函数执行次数，如果count为奇数则关闭设备，否则打开设备，从而实现呼吸灯的效果
void LedControl::void_second_model(){
    count += 1;
    if(count%2==1){
        led_init();
    }
    else{
        ::ioctl(fd, 1, 1);
        ::ioctl(fd, 1, 2);
        ::ioctl(fd,1, 3);
        ::ioctl(fd, 1,4);
    }
}
跑马灯：跑马灯同样依赖于定时器（QTimer），与呼吸灯不同的是，其需要将变量count，维护一个取值范围（该范围是0-3，代表不同的led编号）
void LedControl::void_third_model(){
    if(led_index==3){
        led_index = 0;
        return;
    }
    else{
        ::ioctl(fd, 0, 0);
        ::ioctl(fd, 0, 1);
        ::ioctl(fd, 0, 2);
        ::ioctl(fd, 0, 3);
        ::ioctl(fd,1, led_index);
    }
    led_index += 1;
}









2.模块展示实列图：
跑马灯示例图
     



呼吸灯示例图
  


4.2.5	系统时间模块的具体实现
1.核心代码及其作用：
系统时间获取与修改
void SysTimeControl::on_pushButton_clicked()
{
    QDateTime dt = QDateTime::currentDateTime(); //获取时间
    dt.setTime(ui->dateTimeEdit->time()); // 修改时间
    time_t tt = (time_t)dt.toTime_t();
    int r = stime(&tt);
    if (r)
    {
        if (errno == EPERM)QMessageBox::warning(this, "Error", "没有权限");
    }
} 
系统时间显示
用定时器每隔一秒刷新时间数据到LcdNUmber控件中，
void MainMeun::init(){
    sys_timer = new QTimer(this);
    connect(sys_timer, SIGNAL(timeout()), this, SLOT(flush_time()));
    sys_timer->start(500);
}

void MainMeun::flush_time(){
    ui->lcdNumber->display(QTime::currentTime().toString("hh:mm:ss"));
}



2.模块展示实列图：



六、参考文献

[1] 徐千洋.Linux C函数库参考手册.[M]中国青年出版社.2002 
[2] 李现勇.Visual C++串口通信技术与工程实践[M].人民邮电出版社.2004
[3] 何小平.选择适合ARM的嵌入式操作系统[J].BMRfech Inc.2003
[4] 马忠梅，马广云，徐英慧，田译.ARM嵌入式处理结构与应用基础[M].北京航空航天大学出版社.2002
[5] 邹思铁.嵌入式Linux设计与应用[M].北京清华大学出版社.2002
[6] 杜春雷.ARM体系结构与编程[M].清华大学出版社.2003
[7] Kirk Zurell[M].C Programing For Embedded Systems
[8] 杨海清，周安栋，罗勇，陈牧.嵌入式系统实时网络通信中的LCD显示设计方法[J].计算机与数字工程.2010年02期
[9] 田泽.嵌入式系统开发与应用[M].北京航空航天大学出版社.2005
[10] 陈鑫.嵌入式软件技术的现状与发展动向[M].软件世界.2001
[11] 田泽.嵌入式系统开发与应用实验教程[M].北京航空航天大学出版社.2004
[12] Alessandro Rubini，Jonathan Corbet.Linux设备驱动程序[M].中国电力出版社.2002



