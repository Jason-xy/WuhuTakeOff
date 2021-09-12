# 摘 要

四轴飞行器具有高灵活性，可自由实现悬停和空间中的自由移动；机械稳定性好，不存在很大的安全隐患，适合研究；同时成本低廉、性价比高；应用领域广泛，在玩具、航模、以及航拍等诸多领域都有投入使用；结构简单，适合本科在校生研究。除此以外，四轴飞行器还具有广阔的应用场景。在军事上，四轴飞行器的灵活性，以及低成本，低事故代价等适合进行侦查和 监视工作，获取情报，近空区域执行特殊任务等。是现在电子战等战斗形式的中坚力量；在工业方面可用于高危作业，有害工厂施工等有害人体健康，或者人工施工难度大的场合进行工作；而在民用上，则可以开发出移动监控探头，目标跟踪等多种功能。

本次综合课程设计要求设计并搭建一台简单的四轴飞行器，共分三个学期完成。第一学期的设计主要包含机架的搭建以及硬件系统驱动的编写。采用STM32F4单片机作为控制计算核心，通过小组设计制作的PCB转接板集成各个外设功能模块，如使用ESP8266模块通过WIFI进行飞行器和电脑端的信息交流，使用GY-86姿态传感器模块传送收集到的飞行器本身以及周围的环境信息，使用电机和电调共同调整飞行器桨叶旋转方向和速度，再由遥控器和接收机配合工作，控制电机转动。最终实现飞行器基本部件集成和功能初步实现，也为制作一体化飞控做相关准备。

**关键词：四轴飞行器、STM32、PCB转接板、系统驱动**

# 目 录

**第一章 复杂工程问题归纳与实施方案可行性研究**

>1.1	需求分析与建模	
>>1.1.1项目背景	
>>1.1.2目标内容	
>>1.1.3需求分析	
>>1.1.4预计完成内容	
>1.2复杂工程问题归纳	
>1.3实施方案与可行性研究	
>>1.3.1实施方案概要设计
>>1.3.2硬件部分实施方案
>>1.3.3软件部分实施方案
>>1.3.4实施方案可行性研究

**第二章 工程实现**

>2.1  工程实现任务
>2.2  硬件部分实现
>2.3  软件部分实现

**第三章 存在问题与解决方案**

>2.1  存在的主要问题
>2.2  解决方案

**第四章 执行情况与完成度**

**第五章 分工协作与交流情况**

**参考文献**

**致谢**

>   **说明:**

1.  **报告要求4000字以上。**

2.  **本模板仅为基本参考，请各位同学根据个人情况进行目录结构扩展。**

3.  **报告正文必须双面打印。**

# 复杂工程问题归纳与实施方案可行性研究

本章将对四轴飞行器整体设计进行研究，结合使用背景进行相应的需求分析并置定目标任务。针对硬件搭建，硬件调试，软件驱动系统集成进行概要设计与详细设计，归纳复杂工程问题的实施方案，最后进行方案的可行性研究分析。

##  1.1需求分析与建模

**1.1.1 项目背景**

四轴飞行器具有高灵活性，可自由实现悬停和空间中的自由移动；其机械稳定性好，不存在很大的安全隐患，适合研究；同时成本低廉、性价比较高；应用领域广泛，在玩具、航模、以及航拍等诸多领域都有投入使用；加之其结构简单，适合本科在校生进行实际项目研究。

此外，四轴飞行器还具有广阔的应用场景：在军事上，四轴飞行器的灵活性，以及低成本，低事故代价等适合进行侦查和监视工作，获取情报，近空区域执行特殊任务，是现在电子战等战斗形式的中坚力量；在工业上，可用于高危作业，有害工厂施工等有害人体健康，或者人工施工难度大的场合进行工作；而在民用上，则可以开发出移动监控探头，目标跟踪等多种功能，可谓是百花齐放。

**1.1.2 目标内容**

第一步完成硬件的自主选型与搭建机架。

第二步通过自主绘制打样焊接PCB转接板，实现STM32核心系统与系列外设的硬件系统集成：其中包括选用STM32F4嵌入式开发平台，在其上扩展GY-86姿态传感模块，用于确定四轴飞行器的实时飞行姿态，GY-86姿态传感器采集数据后，在STM32上完成姿态解算；遥控器与接收机通过电磁波远程传输信号，STM32上接收到控制信号后结合自身姿态数据通过PID控制算法实时调整PWM脉宽调制完成电机转速调节，同时使用ESP8266模块通过WIFI信号实现四轴飞行器和电脑上位机的通信。

第三步进行基础外设驱动的编写，使用前后台系统完成基本软件功能的集成调试。

最终目标实现能够用遥控器控制四个电机转速，并能利用上位机实时查看飞行器各项参数，能够连接匿名上位机软件实时查看飞机姿态，能够初步探索硬件一体化飞行控制实现。

**1.1.3需求分析**

该项目共分时三个学期完成。第一个学期主要内容为四轴飞行器机械部件搭建、四轴飞行器硬件系统设计实现以及相关软件驱动模块编写与系统集成三大模块。

根据四轴飞行器的结构，绘制出以下结构图：

![](media/0102492e54703d2124c0076453051f82.png)

图1-1 四轴飞行器结构框图



参考项目背景与目标内容，以下为四轴飞行器的DFD数据流图：

![DFD](media\DFD.svg)

图1-1 0层数据流图



0层数据流图展示了最核心的四轴飞行器操作逻辑，用户遥控四轴飞行器，STM32核心系统产生电路信号与串口信号，分别控制飞行器相关外设和与上位机进行通讯。

对四轴飞行器STM32核心系统进行细化，得到下图1层数据流图：

![layerI](media\layerI.svg)

图1-2 1层数据流图



图中描述了四轴飞行器的基本操作逻辑，用户通过遥控器进行遥控操作，遥控器发送无线信号，接收机接收信号并转化为PWM波供STM32芯片采集解析，同时姿态数据也由GY-86进行提供，STM32处理这些数据后，将处理结果通过引脚电信号的方式发送给飞行器相关外设，同时也将数据信息通过WIFI发送给上位机以供用户查看。

**1.1.4预计完成内容**

-   四轴飞行器机械部件采购与搭建

-   PCB转接板绘制打样与焊接

-   GY-86姿态数据处理并进行硬件姿态解算

-   捕获遥控器接收机产生的PWM信号

-   STM32输出PWM波通过电调控制电机转速

-   飞行器各部件工作状态由OLED屏幕显示

-   WIFI模块完成上位机通信

-   一体化集成式飞控PCB版绘制

## 1.2复杂工程问题归纳

四轴飞行器设计制作是不折不扣的复杂工程问题，经过小组讨论研究，归纳出了以下富有挑战性并值得解决的问题：

**1.四轴飞行器的硬件相关部分内容学习问题：**

学习过程中我们遇到了很多不熟悉的硬件相关名词，较短时间内掌握STM32的基础开发以及四轴飞行器相关外设驱动编写有很大挑战性。并且需要我们自主查找资料并提取其中关键信息进行快速学习。

**2.机械部件搭建稳定性问题：**

机械部件的搭建对于转接板布局以及整个四轴飞行器的硬件部分正常工作尤为重要。搭建的机械部件要求稳定且可靠，同时尽量避免部件之间产生相互干扰，否则在后期可能会对调试的正确和稳定性产生严重影响。这需要搭建部件时提前查阅相关资料，学习已有经验，多次设计与尝试，逐步找出机械部件搭建方案的最优解。

**3.转接板布局合理性问题：**

PCB转接板需在部件均能够稳定工作的前提下，设计最优布局和线路，同时得考虑后期扩展性和插装焊接等可维护性。

**4.STM32GPIO引脚分配问题：**

四轴飞行器实现的时候需要将多个外设与STM32GPIO引脚相连，如何按照部件功能合理分配相应引脚非常重要。在引脚调用的同时需要进行使能初始化，其中涉及到引脚对应的多个寄存器的设置以及相关总线时钟的使能问题。对电路的有限理解程度和交叉环境开发等因素可能会导致潜在问题。

**5.系统多功能协调问题：**

当STM32同时控制如GY-86、接收机、WIFI、串口、电调等多个模块时，也要完成相关数据的解析运算。如何保证各部分协调工作，应对随机模块故障，增强集成系统健壮性和稳定性等，需要在软件层面上进行详细设计。

## 1.3实施方案与可行性研究

本部分主要是为前面归纳出的问题提出解决方案，解决方案包含概要设计和详细设计，同时对所提出的设计方案进行可行性研究。

**1.3.1	实施方案概要设计**

实施方案总体分为3个部分。

首先是相关硬件资源的采购、调试与组装，完成飞行器雏形的搭建。其中包含了元器件的选型，机架的选择，四轴飞行器的组装，PCB转接板的设计制作，STM32主控与各个外设的硬件连接。

其次是针对每一个外设单独进行驱动编写与功能测试，保证单一功能调试正常。其中包含了WIFI模块、接收机、GY-86模块、OLED显示模块、电调等模块驱动的编写以及其对应的基础功能TIM、I2C、USART等的使能配置。

最后是完成四轴飞行器硬件各外设部分的系统集成。每一个模块功能测试正常后，利用前后台模式进行系统的集成，然后进行应用层软件的编写，使飞行器能够利用DMP硬件进行姿态解算并将解算后数据发送给“匿名上位机”软件实时显示飞行器姿态，能够通过遥控器控制四个电机进行旋转与锁定。

**1.3.2硬件部分实施方案**

**1.硬件资源选型采购方案**

>   组装四轴飞行器除机架外，需要一系列硬件资源，主要包含以下物品：

-   STM32核心主控芯片

-   GY-86姿态传感器（内含MPU6050姿态检测与hmc5883l地磁传感器）

-   ESP8266无线通讯模块

-   OLED显示屏

-   接收机与遥控器

-   无刷电机\*4

-   电调\*4

-   电池与螺旋桨

下面对每个模块的选型采购进行详细方案设计。

首先是最重要的STM32主控芯片的选择。主控芯片的选型要求首先要满足基础的功能，引脚数量要足够，外设功能得齐全，同时资料丰富。同时考虑到四轴飞行器未来的拓展需求，以及对处理速度的要求，最终小组讨论决定使用型号为STM32F411CEU6的核心处理器。该处理器采用Cotex-M4内核架构，最大主频100MHz，Flash大小512KB，RAM大小128KB，GPIO口有36个。同时支持3组USART，5组SPI，3组I2C，能够同时支持32路PWM信号采集，整体性能满足要求。

处理器型号确定后，秉持集成一体，使飞行器尽可能小的原则，经过网上资料搜索，最终确定了使用一款搭载F411芯片的最小系统板WeActV3.0。该公司设计制作的这款最小核心板结构小巧，引脚资源丰富，采用TypeC接口比较主流，最终要的是资料丰富且开源，能够在Github上查询到这款核心版的硬件设计电路图等资料，有助于进行核心系统的学习。

该核心版的渲染图和片上外设资源图下图：

![](media/b5b8afa810d04b3042415006ac67dbe5.png)

图1-3 最小系统版选型



然后是相关传感器的选购。传感器的选择是尽量经济实惠，性价比高，性能稳定。市面上传感器鱼龙混杂，很容易购买到假传感器徒增烦恼，因此尽量选择规模比较大的机构进行采购。经过调研，最终在淘宝店铺选定了相应传感器：

![](media/9883917a46b6167496a80f1d621246fd.png)

![](media/cb62bf4fc9c72f2e72c6a790ae07eb44.jpg)

![](media/857876876d3b1d2c55fc01df726cf17d.png)



图1-4 ESP8266、GY-86、OLED显示（从上到下）



接下来是动力系统装置的选择。考虑到飞机飞行所需要的升力，在学长前辈的帮助下，我们选择了SUNNYSKY品牌的X2212-13型号无刷电机。2212表示该无刷电机定子直径22mm，定子高度为12mm。该电机KV为980CW，意为每升高1V电压电机每分钟转速增加980转，该电机空载电流0.3A，最大连续功率高达300W，KV值低但扭力大，搭配电机建议的型号为APC9450的桨叶，最大载重3KG，符合飞行器起飞要求。同时按照电机建议，我们选用了天行者20A电调，能够达到令人满意的性能，电池也选用了格氏ACE25C2200mAh 3S航模Li-Po电池，保证稳定电流输出与安全性。

![](media/21806d7bc94619915bdcdb6e2793fa8d.png)

![](media/f25d0b59c7874cbb017fbe9a54144875.png)

![](media/0a891902a4a880e0e70b0d8aebb1c26f.jpg)

图1-5 电机 螺旋桨 电调 电池（从左到右）



最后是飞行器的控制部分，在小组成员对飞行器的综合性能以及未来拓展需求进行评估过后，我们最终决定采用FS-iA10B接收机。该接收机拥有10个通道，能够同时进行10组不同数据信号的接收，采用26mm双天线设计，支持140个2.4GHz波段，输入电压4.0-6.5V，支持数据回传，支持iBus协议，支持PPM输出，可扩展性强。

配套我们采购了Jumper-T18遥控器，该遥控器使用OpenTX开源操作系统，内部集成射频芯片5合1，能够支持2.4GHz和915MHz频率，可控制范围最大可达10KM，并且配备了4.3inch彩屏可供数据显示，支持67种通讯协议，16个通道输出，性能优秀，能够操控多种设备。

![](media/7011f08f672129b18bab3db3b58da0eb.png)

![](media/9e4b98c7c439ee2949bc96999df6509f.png)

图1-6 接收机与遥控器



至此，所有硬件资源选型采购方案设计完毕，下面为四轴飞行器机架选型。

**2.机架选型方案**

我们都知道机架是承载整个飞行器的核心部件，所有的控制设备，动力设备，传感器等都搭载在机架上，因此机架的选择也是非常重要的。

机架的选择需要考虑以下因素：首先是机架的大小，市面上的机架主要是F330与F450两种规格，当然也有新一代的380规格的机架，即电机对角轴距380mm。我们计划选用的即为这种大小的机架。其次为机架的材质，有普通的塑料玻纤机架，也有更高级的碳纤机架。最后该考虑机架的造型以及功能。

考虑以上因素，我们第一款机架选择了一款可折叠设计的碳纤机架，但因改机架做工原因与可拓展性，最终放弃使用该机架。第二款机架选用了图腾Q380穿越机机架，可扩展性强，内部空间较大，能够满足线材的收纳，整体形象美观，机臂兼容大部分电机，底座PCB自带分电功能，开孔合理，重量仅仅165g，性价比高。

![](media/93050cd6fa736569a5eba57e36707d32.jpg)

图1-7 飞行器机架

**3.四轴飞行器的组装方案**

在四轴飞行器硬件与机架选型采购完毕后，接下来的工作则为整个四轴飞行器硬件部分的组装。

第一步应该将机架底部分电板与电源接口连接，选取一个较大焊点将XT60接口对应正负极完成焊接。

第二步为四轴机架的组装，将机臂固定到底部分电板上，然后再将上载板合拢，拧紧螺栓，检查整体稳定性。

第三步为固定四个电机。将四个电机分别安装到机臂四角，拧紧底部螺丝，固定稳定。

第四步为连接并固定电调。先将电调与电机三个控制线进行连接，可以暂时不关注电机转向。然后将四个电调电源正负极量好长度裁切后，在分电板底部进行焊接，最后将电调用绑扎带固定到机臂上，理顺走线。

最后一步为将电池进行固定。用绑扎带将充满电的电池固定到飞机底部后，连接电池于底部分电板XT60接口。正常情况下电调通电将会自检，电机也会发出固定频率的鸣叫。至此说明硬件部分组装正常。

**4.PCB转接板设计方案**

考虑到如果将核心系统板直接固定在飞行器上，相关的传感器位置将无法确定，同时走线难度大且不易管理。因此我们计划设计PCB转接板，功能为连接所有的硬件装置，包括核心板，传感器，电调，接收机等外设，能够进行数据信号的传递。

我们尝试了两套工具来做PCB的设计，分别是Altium
Designer和立创EDA，下面是这两款软件的使用感受：

|                 | 优点                                              | 缺点                                            |
|-----------------|---------------------------------------------------|-------------------------------------------------|
| Altium Designer | 1.可自定义程度更高 2.使用稳定                     | 1.操作复杂、上手困难 2.原理图库、封装库难以寻找 |
| 立创EDA         | 1.面向国区用户，上手简单 2.有在线封装库，便捷高效 | 1.高级功能不如AD多 2.有一些小BUG                |

表1-1 硬件开发工具使用感受

最终我们选定的制作PCB转接板使用的软件为Altium
Designer，是一款电子产品开发系统，具体的使用说明在此不再赘述，详细使用流程可参见网上资料。我们在复杂问题归纳里面提到过相关知识的学习，因此在本部分只对设计进行解释。

首先我们经过测量和观察，发现机架上盖中心有一直径18mm的圆形打孔，可以解决线路隐藏问题。因此我们转接板底板设计采用中心打孔方式。

然后我们在计划将传感器采用可拔插设计与转接板连接，这样便于更换和调试，所有对于所有的传感器都需要设计相应母座。

在结合软件设计部分需要的引脚后，采取核心板底隐藏电调接口，接收机通道从板底引入的双面设计办法。同时单独将核心板引脚进行二次引出，便于后期机载调试。也将电源进行了扩展，3V、5V、GND接口各多引出两组便于使用。

排布好焊盘和焊孔口，利用AD进行自动布线，最后手动绘制丝印层和元器件选型，生成3D模型。

**5.一体化飞控设计方案**

转接板适用于开发阶段，当代码调试成功后，一体化的飞控能够最大限度缩小空间，不使用也不引出无效引脚，便于批量生成和制作。因此第二阶段我们设计制作了一体化飞控，并在次基础上进行迭代更新，设计制作了一款能够搭载更强规格处理器，据备如GPS、光流、激光测距、图传等多模块的集成式一体飞控开发板。

一体化飞控详细设计流程如下：

首先是原理图绘制。原理图的绘制是极其关键的一步，不仅要正确表示各部件之间的逻辑关系，还要考虑接线是否有利于PCB的布线，布局。根据MCU上的外设的功能和引脚复用，我们先根据外设的通讯协议为其分配引脚，根据MCU某项外设的稀缺性确定分配的顺序，比如FSMC协议的引脚是唯一且不可更改的，所以我们先为需要FSMC的SDRAM分配引脚。因为I2C是可以软件模拟的，所以只需要普通的GPIO引脚即可，可以最后分配引脚。分配完引脚后开始具体绘制原理图。

原理图阶段需要制作好相关的原理图库和PCB库，这些可以从淘宝卖家的百度网盘和Altium designer内置的搜索插件获取。

原理图绘制阶段中，我们先放置主芯片STM32F407ZGT6，并放置退耦电容，晶振等器件。接下来设计各外设模块电路。模块与MCU连接依靠网络标号。每个模块大同小异，都是信号线连接上拉电阻再接入外设芯片。外设芯片再连一些地线，控制线，电源线，电容即可。一些通讯模块比如串口CH340或WIFI模块可能还需要连接天线和晶振，接上即可。很多电阻，电容数值都有特殊的要求，这些数据主要来自外设芯片数据手册，网上资料，和淘宝芯片卖家资料。供电模块方面，我们选择LIPO电池通过2次降压，先降到5V，再降到3.3V再给MCU和外设供电。这部分需要线性稳压模块和降压模块，资料主要来自电子工程网站。画完原理图经过仔细的检查就可以开始绘制PCB了。

将原理图导入到PCB文件中，即可开始绘制PCB图。PCB最关键的问题是布局与布线，布线能否成功依赖布局是否隔离。首先，先确定要投板的厂家，从厂家的客服和网站获取相关工艺参数，比如过孔大小，导线最小宽度，板子大小，元器件到线最小间距等。得到正确的工艺参数才好开始设置编译规则。设定完规则之后，开始布局布线。先确定板子大小边框，然后将MCU放到板子中央，接着在靠近规定引脚的地方放置晶振，晶振要尽量靠近MCU，防止频率的延迟。接着在MCU周围放置各个模块，模块可以放在MCU的四边或者背后，模块放在哪一边取决于连接的引脚会靠近芯片的哪个边上，哪边靠近就放在芯片的哪一边。布局还需要考虑板子与其他机械结构和具体功能，比如机架之间的耦合。布局的最后一步是放置各模块的电阻，电容，二极管，接口，然后再微调一下即可开始布线。

布线最重要的是遵循正面走竖线，反面走横线原则。因为复杂的电路需要很多导线，如果正反面同时走了很多竖线或横线会导致某些跨度较远的线无法通过。此外，还要注意走线的宽度，通大电流的线要有更大的宽度，否则板子会烧坏。还有不能吝惜过孔，在合适的位置打孔会让走线更加整齐美观。因为线路较复杂，无法使用自动布线，自动布线器的布线也很丑。所以决定自己动手布线，过程及其枯燥。最后加上泪滴，敷铜，通过编译测试即可交给厂家投板。

至此，硬件部分实施方案结束，下面将对软件部分的设计方案进行说明。

**1.3.3软件部分实施方案**

在我们完成硬件部分的搭建后，接下来最重要的工作便是进行软件部分的设计。软件部分的设计将从总体设计、基础外设驱动、功能模块驱动以及系统集成四个方面进行说明。

**1.软件部分总体设计方案**

软件部分总体上采用HAL库编程方案，三层封装。

首先利用CubeMX软件进行基础外设的配置，如时钟、I2C、USART、TIM等，对上层功能模块留出接口。

然后建立在接口基础上，对GY-86、ESP8266、OLED、接收机、电调等功能模块的驱动进行编写。

完成各个单独功能模块驱动编写后，对系统进行集成，使用前后台逻辑，后台程序为OLED屏幕的刷新和与上位机的交互，前台程序为串口接收中断、输入捕获中断、姿态更新中断等。

最后连接“匿名上位机”，能够实时采集飞行器姿态，同时也可以用遥控器控制四个电机的解锁与转速。

**2.基础外设驱动原理与设计方案**

**时钟配置**

时钟部分是一个系统的基础，所有的操作都是建立在时钟正常工作基础之上的所以第一步为配置一个稳定并且稳定的时钟系统，能够供所有外设使用。

考虑到时钟配置的原理性问题超出了我们本部分讨论的目的，所以我们最终使用了CubeMX软件的时钟树功能进行自动配置，配置界面和结果如下图：

![](media/e35adf277056db41376b9ec44df490a9.png)

图1-8 STM32F411CEU6时钟树

接下来对时钟树中一些关键节点配置进行解释：

**HSE高速外部时钟信号配置：**

HSE是高速的外部时钟信号，可以由有源晶振或者无源晶振提供，频率从4-26MHZ不等。当使用有源晶振时，时钟从OSC_IN
引脚进入，OSC_OUT引脚悬空，当选用无源晶振时，时钟从OSC_IN和
OSC_OUT进入，并且要配谐振电容。

**PLL多路复用器配置：**

HSE使用的是25M的无源晶振。在设置主PLL时钟的时候，需要设置一系列的分频系数和倍频系数，配置RCC_PLLCFGR寄存器。经过设置后，我们的主PLL时钟为100MHz

**系统时钟SYSCLK多路复用器配置：**

系统时钟来源可以是:HSI、PLLCLK、HSE，具体的时钟配置寄存器**RCC_CFGR**的位1-0：
**SW[1:0]**设置。我们这里设置系统时钟:**SYSCLK=PLLCLK=100MHz**。

**AHB总线时钟HCLK配置：**

系统时钟SYSCLK经过AHB预分频器分频之后得到时钟叫APB总线时钟，即HCLK,分频因子可以是：[1，2，4，8，16，64，128，256，512]，具体的由
**RCC_CFGR**的位7-4：**HPRE[3:0]**
设置。片上大部分外设的时钟都是经过HCLK分频得到，至于AHB总线上的外设的时钟设置为多少，得等到我们使用该外设的时候才设置，我们这里只需粗线条的设置好APB的时钟即可。我们这里设置为1分频，即**HCLK=SYSCLK=100MHz**。

**APB1总线时钟HCLK1配置：**

APB1总线时钟PCLK1由HCLK经过低速APB预分频器得到，分频因子可以是:[1，2，4，8，16]，具体的由
**RCC_CFGR**的位12-10：**PRRE1[2:0]**决定。HCLK1属于低速的总线时钟，最高为50MHz，片上低速的外设就挂载到这条总线上，至于APB1总线上的外设的时钟设置为多少，得等到我们使用该外设的时候才设置，我们这里只需设置好APB1的时钟即可。我们这里设置为2分频，即**PCLK1=HCLK/2=50MH**z。

**APB2总线时钟HCLK2配置：**

APB2总线时钟PCLK2由HCLK经过高速APB2预分频器得到，分频因子可以是：[1，2，4，8，16]，具体由**RCC_CFGR**的位15-13：**PPRE2[2:0]**决定。HCLK2属于高速的总线时钟，片上高速的外设就挂载到这条总线上。至于APB2总线上的外设的时钟设置为多少，得等到我们使用该外设的时候才设置，我们这里只需设置好APB2的时钟即可。我们这里设置为1分频，即**PCLK2=HCLK=100MHz**。

**I2C配置**

I2C协议有硬件I2C和软件I2C两种。最开始我们选择的是软件I2C，从学习的角度上来考虑,我们从寄存器级实现了软件I2C，但是在实际使用的时候，特别是在高速模式下难以避免软件I2C时序不规范所带来的稳定性问题，所以我们最终选择了使用更加稳定的硬件I2C。

I2C配置分为物理层和协议层。物理层需要设置I2C使用的GPIO引脚，定义SDL和SCK并且初始化引脚时钟。因为STM32有固定复用给I2C的引脚，所以我们只需要查询手册进行对应的配置即可。协议层配置则利用CubeMX自带的配置系统，选定I2C号后，设置时钟为快速模式，然后设置时序占空比，四轴I2C采用7地址模式，自有地址1和2都设置为0，不使能双倍地址和通用呼叫模式，但是使能I2C中断。


**USART配置**

在四轴飞行器中，USART主要用于O与上位机进行串口通信，为WIFI模块提供下层接口。

USART的配置也分为物理层与协议层。

物理层配置中，我们需要按照手册要求使用对应的USART复用引脚，进行USART时钟和GPIO时钟的使能，然后设置引脚的输出模式以及配置USART的中断。

在协议层配置中，主要任务为和上位机约定好串口交互协议。四轴飞行器与上位机交互的过程波特率使用115200，字长8位，包含1位停止位，无校验位。采用全双工模式，不使用硬件流，采样率选择16倍过采样。

最后我们还得对printf进行重定向，这样方便我们编程人员通过printf函数向USART串口打印信息。

**TIM配置**

TIM定时器在四轴控制系统中起到很重要的作用。

主要功能有3个。第一是基础的计数功能，产生1us的节拍来模拟程序运行的时许，控制不同模块程序运行的频率。第二是进行接收机回传PWM波的捕获。第三是进行电调电机的PWM输出。

因此我们使用TIM1进行计数中断，利用TIM2与TIM4进行六个通道的输入捕获，利用TIM3进行四个电机控制信号的输出。

首先是计数中断的配置。我们直到TIM是挂载到APB2总线上的，在时钟配置中我们以及将APB2的时钟配置为100MHz。为达到1us的节拍，经过公式计算，我们将TIM1的PSC配置为99，将ARR配置为999，设置向上计数模式，不分频。这样就能够每隔1us产生一个中断了。然后我们将这个中断配置到用户程序的最高优先级，即不能被其他的外设中断打断。

然后是配置TIM2与TIM4的PWM输入捕获。在物理层中，设置PA0\~PA3四个引脚对应TIM2的四个通道，PB8与PB9对饮TIM4的一二通道。对相关时钟进行使能，然后对GPIO引脚模式进行配置，最后初始化GPIO，配置并使能中断。在协议层中，我们也将节拍调整到1us，然后将ARR寄存器配置为最大值，用于准备捕获中断。

最后是配置TIM3的PWM输出。物理层配置与之前类似，四个电机分别对应PA6、PA7、PB0、PB1.协议层中，查询了电机和电调的说明书，配置频率为50Hz的PWM，用于电机转速的控制。

至此，基础外设驱动原理与设计方案说明完毕。

**3.外设功能模块原理与设计方案**

**OLED驱动**

OLED显示驱动部分，我们采用了开源方案，使用IIC协议进行接口的读写，接口包含了以下功能：

>   1.OLED初始化。
>   2.显存写操作。
>   3.数字定点显示。
>   4.字符定点显示。
>   5.字符串定点显示。
>   6.汉字显示。
>   7.BMP位图显示。

我们利用以上资源实现了：

>   1.飞控界面的绘制。
>   2.自定义数据显示方式。

**GY-86驱动**

GY-86包含两个传感器，分别是MPU6050，用于测量角度与加速度，HMC5883L用于测量地磁场。两个传感器与STM32进行通讯都是使用I2C协议。

因此我们对接I2C读写接口，封装GY-86寄存器的读写方式。对照GY-86的数据手册，对传感器进行初始化设置：

配置路径为：MPU6050初始化配置-\>开启MPU6050旁路模式-\>将HMC5883L挂载到MPU6050的I2C总线并初始化。具体实现代码我们在工程实现一章进行详细展示。

经过逐步迭代，我们最终的GY-86驱动实现了以下功能：

>   1.MPU6050初始化
>   2.HMC5883L地磁仪初始化
>   3.GY-86初始化
>   4.I2C基本读写接口对接
>   5.陀螺仪数据获取
>   6.加速度计数据获取
>   7.温度计数据获取和解析
>   8.地磁仪数据获取
>   9.宏定义所需寄存器表
>   10.陀螺仪零偏矫正
>   11.磁力计校准
>   12.陀螺仪显式数据输出
>   13.磁力计显式数据输出
>   14.提供DMP接口

**ESP8266驱动**

ESP8266作为与上位机通讯的模块，我们采用了TCP/IP协议进行数据收发，以下是相关的配置流程和功能模块。

在配置之前需要先将ESP8266的读写操作对接到USART的读写接口，在完成printf重定向后直接调用printf即可。

通过查询AT指令集手册可以获得TCP_Client透传模式的配置方法：首先发送复位命令，然后将ESP8266设置为AP模式，并且设定wifi基本信息。然后设置为单链路联络，配置TCP服务，选择透传模式，最后开始透传。

**电机驱动**

电机驱动的主要目的是封装对电调的控制函数，在加载电机驱动之前需要先对接TIM3的相关接口。

电机驱动主要实现了以下功能：

>   1.电机初始化。
>   2.设定电机转速。
>   3.电机测试。
>   4.电机锁定。

**接收机驱动**

接收机驱动主要是完成对接收机信号的捕获和解析，其重点是PWM的输入捕获。

在使用之前需要先初始化捕获中断，将时钟频率置为100MHz，这样CNT每经过1us计数一次。

中断程序流程图为：

![TIM](media\TIM.svg)

图1-9 中断捕获流程图



**DMP姿态解算驱动**

如果选择了使用DMP姿态解算，那么前面的GY-86初始化实际上可以直接调用DMP固件库函数完成。

DMP姿态解算采用的MPU6050官方固件库，我们需要完成的工作就是将官方固件库移植到STM32上，需要完成的工作如下;

>   1.预留I2C读写接口。
>   2.预留毫秒延迟接口。
>   3.预留节拍获取接口。

完成接口对接后，我就即可使用DMP固件库完成模块的初始化。

**4.系统集成方案**

在完成基础外设驱动的编写以及功能模块驱动的编写后，我们的任务则为进行整体系统的集成。实现的功能目标是能够利用遥控器进行四个电机转速的控制，同时能够在上位机中实时查看到飞行器的姿态。

因此我们设计采用前后台系统，后台程序为OLED屏幕的刷新和与上位机的交互，前台程序为串口接收中断、输入捕获中断、姿态更新中断等，最终前后台逻
辑如图所示：

![INT](media\INT.svg)

图1-20 前后台系统示意图



**1.3.4 实施方案可行性研究**

前两个小节分别对四轴飞行器的硬件部分和软件部分进行的实施方案设计，本章将对该实时方案进行提炼归纳，并且给出该方案的可行性研究说明。

1.  **实施方案归纳**

完成需求分析以及任务制定：进行综合课程设计I需求分析，绘制相关数据流图。结合小组成员时间以及知识掌握水平制定任务，完成分工。

相关基础知识学习：通过网上各大论坛（野火，正点原子，CSDN等）提供的开源免费学习资料和相关实战视频，理解STM32内部设计和使用方法。确定并学习使用课程设计需要用到的IDE和软件，如MDK5等进行工程开发调试，Altium
Designer绘制PCB转接板，STM32CubeMX软件进行引脚的选定等。通过和学长前辈进行沟通，学习经验教训，少走弯路。在有限的时间里掌握更多知识。

机械部件选型采购搭建：按照3学期课程设计相关要求，选择合适的STM32主控芯片型号，采购机架、螺旋桨、电机、电调、GY-86传感器、接收机、遥控器、WIFI模块、供电模块等相关机械部件，认真阅读相关部件说明手册，确定搭建顺序，和同学、学长交流，结合自己的想法完成机械部件搭建固定与调试。

PCB转接板绘制打样焊接：学习Altium
Designer官网提供的说明手册，查找网络文字资料以及视频资源进行入门学习。有一定基础后开始尝试绘制较为简单的PCB
板，熟悉软件的使用方法。在完成STM32引脚分配，以及明确项目外设引脚需求后，设计符合要求的转接板。并制作实物验证其电路设计的正确性。调试正常后固定到四轴飞行器机架上，完成与各个部分的连线。

WIFI模块相关设计：首先将STM32最小系统板与WIFI模块进行连接，认真阅读官方说明文档完成接线。通过串口调试，上位机发送AT指令加以检验。确保WIFI模块能够正确稳定地传动数据信息。最后将WIFI模块接入PCB转接板进行集成测试。

I2C以及GY-86相关设计：STM32板载硬件I2C电路，当然也可以通过GPIO引脚实现软件模拟I2C信号协议进行通信，出于工程开发和效率考虑，我们选择使用STM32板载硬件I2C进行与GY-86姿态传感器的通信。结合GY-86相关参考手册与说明文档，完成回传数据采集与含义解释。软件I2C功能作为备用，如果后续阶段有需求，或者遇到了难以解决的硬件稳定性问题，在低内聚高耦合的设计框架下，我们可以快速更改为软件
I2C 并且不影响其他功能。

接收机电调相关PWM输入输出设计：接收机工作后可将遥控器发送的通道信号转换为PWM波进行输出，STM32利用TIM外设进行PWM的输入捕获。四个与电机相连的电调通过STM32输出PWM波进行控制，同样使用TIM外设产生可控PWM驱动电调调节电机转速。我们采用中断的方式捕获接收机传来的PWM信号，在STM32中进行信号处理，将接收机采集到的四个通道信号分别输出给4个电调，单独完成四个电机转速的控制。

串口与OLED屏幕显示设计：为便于实机调试数据的显示，我们计划在转接板上为OLED显示屏预留位置，设计相应的硬件接口连接OLED显示屏和STM32最小系统板，编写相关屏幕驱动，在系统集成过程中将每一步骤的设备情况显示在OLED屏幕上，也将GY-86采集陀螺仪、加速度、地磁信号、六个TIM通道捕获到的PWM数据和电机转速实时显示在屏幕上，便于调试。

**2. 可行性研究**

**技术可行：**在指导老师带领下每周开会共同讨论疑难点；由经验丰富的同学给出建议或者解惑；分小组讨论共同学习进步；小组成员利用C语言编写硬件驱动程序，使用MDK软件进行交叉开发，学习使用Altium Designer绘制PCB转接板。同时时间充足，技术成熟，网络资源丰富。加之有经验丰富的老师指导。所涵盖技术有绘制PCB转接板，机架电机选型和安装、电调接收机选型安装等硬件操作。

**课题合理性：**本课题研究理论假设合理，目标内容清晰，实施的可操作性较强，阶段实施过程与目标明确，完成课题的可行性有充分保证。

**经济可行性：**四轴飞行器与航空飞机等大型飞行器相比价格低很多。在学生可承担范围。利用较少的经费动手实践学习更多的知识。

**法律可行性**：对于项目的开发和使用来说，必须符合法律规定要求。该软件完全符合法律要求，可以自由开发使用。

**3. 需求规则**

**可靠性需求：**四轴飞行器属于嵌入式软件项目开发。要求安全且可靠。应确保产品在正常环境中的可靠稳定以及在恶劣环境下相对安全。不应该出现电池漏电、飞机失控等安全隐患和安全事故。

**实时性需求：**要求飞行器在遥控器做出操作指令后应该立即响应。

**功能性需求：**

硬件搭建完成后，四个电机能够按照正常方向旋转；

PCB转接板和最小系统板连接后，所有硬件能够完成焊接、插接并且正常通电；

PCB转接板模块分布合理，符合审美。避免出现硬件连接后线路杂乱交错情况；

能够利用连接调试好的WIFI模块进行地面端上位机和飞行器之间的信息交互；

传感器实时数据（如GY-86等）能实时向地面端上位机传送飞行器所处状态的陀螺仪、加速度计、地磁仪、电机转速等状态。

**4. 开发运行环境：**

**编程语言为C语言：**飞行控制系统由C语言编写

**开发板/STM32最小系统板：**STM32F411CEU6单片机性能高、成本低、功耗低，适用于嵌入式开发，是四轴飞行器的处理核心；

**采用交叉开发模式**：交叉开发为在一台通用PC上对软件进行编辑、编译、链接，然后下载到嵌入式设备中运行调试的开发过程。

# 第二章 工程实现

## 2.1 工程实现任务	

**硬件部分任务：**

-   完成机架搭建

-   PCB制作与焊接调试

**软件部分任务：**

基础外设驱动代码编写：

-   时钟配置

-   I2C驱动

-   TIM驱动

-   USART驱动

功能模块驱动代码编写：

-   OLED驱动

-   GY-86驱动

-   ESP8266驱动

-   电机驱动

-   接收机驱动

-   DMP姿态解算驱动

应用层软件程序编写与测试：

-   上位机数据传送程序

-   测试程序

## 2.2 硬件部分实现

我们在第一章将硬件部分的设计制作进行了详细的描述，因此本部分将对详细设计进行实现，完成四轴飞行器的硬件搭建。

**1、硬件设备采购焊接**

首先是采购STM32核心系统、传感器元器件等并引脚焊接，焊接完成后实物如下页图：

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231451.jpg](media/35dd30739385ed41e36e50911ecb62f2.png)

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231411.jpg](media/4e30b6a883e566338b72f653744426c3.png)

图2-1 STM32最小核心系统实物图



核心板引脚采用反焊，便于连接面包板进行调试，也适合接入转接板进行集成。

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231626.jpg](media/6a256a72ce76111b9e3492f5357db4a3.png)

图2-2 ESP8266 GY-86 OLED实物图



ESP8266出厂自带排针，不用焊接。GY-86与OLED屏幕引脚焊接时需注意与水平板面呈90度直角焊接，便于安装和校准。

然后是将电机、电调与接收机固定到机臂上：

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231932.jpg](media/d609f57c1551cb8127ed8dedb2e5c739.png)

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231821.jpg](media/8a3080cdcc0ca8dd54561dd44816743b.png)

图2-3 电机 电调 接收机实物完成固定



**2.机架与组装与搭建实现**

收到机架散件后，完成分电板的焊接，再利用螺丝刀将上下盖与机臂固定，然后连接电调后进行电调供电的焊接。最后结合转接板设计方案，将所有数据传输线从中间穿孔引出。

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231940.jpg](media/140673edc807b53ce090d86148852c28.jpg)

![D:\\Huawei Share\\HwMirror\\IMG_20210119_232011.jpg](media/ac3c4e37c362ce1bad2a4df1d180f8f5.jpg)

图2-5 电源焊接处于机架走线实物



**3.PCB转接板设计实现**

PCB转接板片上资源分配情况如下：	

-   ESP8266：USART1_RX、USART1_TX

-   OLED：I2C1_SDA、I2C1_SCL

-   GY-86：I2C2_SDA、I2C2_SCL

-   Motor Output：TIM3_CH1、TIM3_CH2、TIM3_CH3、TIM3_CH4

    （分别为电机一至电机四）

-   Controller Input：TIM2_CH1、TIM2_CH2、TIM2_CH3、TIM2_CH4

    （接收机通道1\~4）

    TIM4_CH3、TIM4_CH4（接收机通道五、六）

-   调试接口两个，时钟接口4个，供电接口10个左右

![](media/26dbb5c77e938cab0eaa1c08a13303ad.png)

图2-6 片上资源分配情况



**按照设计要求进行原理图绘制，成果如下**

![](media/4758bf31461126f488dd6425cdadebc7.png)

图2-7 V1原理图绘制成果



完成PCB绘制后，2D平面图与3D渲染图结构如下：

2D平面图：

![](media/4b026ef2dc690bfcfa6c57ce75d2b717.png)

图2-8 V1 PCB绘制成果（2D）



3D渲染图：

![](media/dac701e053908dcfefd68dc2085bca58.png)

图2-9 V1渲染图成果



我们将最终设计好的PCB工程文件进行电路编译，编译通过后即可将工程文件发送给生产厂商，商家会将制作好的PCB板实物发给你。收到后我们按照3D渲染图进行焊接。最终V1转接板实物效果如图：

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231720.jpg](media/b1173b52e1fb001c413d0522bce45cea.png)

![D:\\Huawei Share\\HwMirror\\IMG_20210119_231709.jpg](media/4c156ed7e31fd564d88112b31ab69ea8.png)

图2-10 转接板焊接实物图



将传感器和核心板与转接板连接，上电测试，测试正常，证明设计合理无误。

![](media/d1d555d4f467980606f62c3af027091f.jpg)

图2-11 转接板调试图



最后我们将所有硬件进行组装，得到真机实物图如下：

![](media/2aa1722b29e9835742549e03d6cff3a9.png)

图2-12 四轴飞行器硬件实现



**4.一体化PCB设计制作**

V2版本主要完成了MCU最小系统的自主设计，可以极大程度上地缩小模块体积，同时也让模块的整体布局更加自由。

V2原理图绘制：

![](media/afca027687da82f93b89b103ebb3eb08.png)

图2-13 V2原理图



V2版本 2D平面图与3D渲染图：

![](media/9696ae248dbe974f40b8345247248aeb.png)

![](media/23a109bd093559a7f9aabab0d7174dda.png)

图2-14 V2 2D平面图与3D渲染图



PCB_V3—最终集成版本

该版本为最终目标版本，在自主设计的最小系统基础上完全重新设计外围硬件模块电路，根据各个功能模块的原理图重新设计PCB布局，实现了硬件模块的完全自主设计。除了第一期所载外设功能模块，该版本还添加了光流传感器、摄像头图传模块、GPS定位模块、SRAM存储器、SD卡等部件。在保持体积的基础上，使模块的功能性尽可能地完善。但是由于设计较为复杂，投产成本较高，目前还在仿真验证阶段，验证完成后就进行初步投产。

原理图绘制：

![](media/58b68e931268cb642bf2038c6e8afcae.png)



绘制V3版本2D平面图与3D渲染图：

![](media/19a2ecad923b313bdb88608b7a90151f.png)

![](media/a9c921051b325e45109d70a1bd44fc7b.png)

图2-16 V3 2D平面图与3D渲染图



V3版本打样完成实物图如下，至此硬件实现结束，下节为四轴软件部分实现。

![](media/daf733a46da14adc284e46fe7c7f6ee8.png)

图2-17 V3打样实物图



## 2.3软件部分实现

我们在第一章将软件部分的设计制作进行了详细的描述，因此本部分将对详细设计进行实现，完成四轴飞行器的软件系统。

软件部分整体的系统逻辑如下图：

![INT](media\INT.svg)

图2-18 前后台系统逻辑图



1.  **基础外设驱动编码实现**

在了解时钟、I2C、USART、TIM的工作原理与配置思路后，我们分别对以上基础外设进行配置实现。

注：所有基础配置代码都有CubeMX预先生成，此处只针对核心代码与业务层代码进行说明。

**时钟代码实现：**

首先利用宏定义设置PLL多路复用器的分频系数：

*代码2-1 PLL分频系数*

```c
#define PLL_M 12
#define PLL_N 96
#define PLL_P 2
#define PLL_Q 4
```

经过配置后我们的主PLL时钟为：*PLL=25/12\*96/2=100MHz*

代码2-2 系统时钟配置函数

```c
void SystemClock_Config(void)//设置系统时钟为100MHz
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}

  __HAL_RCC_PWR_CLK_ENABLE();//开启时钟
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); 
//电压缩放
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;//使用HSE
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//开启HSE
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;//开启PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;//选择HSE做为PLL时钟源
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;//设置倍频与分频系数
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)//使用HAL函数初始化时钟
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;//选择要配置的时钟
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;//将PLL做为系统时钟源
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;//设置AHB总线分频系数
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;//设置APB1分频系数
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;//设置APB2分频系数

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}
```

上面一段代码首先新建了两个时钟初始化结构体，然后利用HAL库开启时钟并完成电压的缩放。接下来配置结构体相关参数，使用HSE外部高速，开启HSE与PLL，选择HSE作为PLL是中原，然后设置对应的分频系数。最后将初始化结构体导入初始化配置中，完成配置。然后再配置另一个结构体，以同样的方式完成系统时钟的初始化。

**IIC驱动代码实现**

IIC协议层配置：

代码2-3 I2C协议层实现

```c
void MX_I2C1_Init(void)
{  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;//设置时钟为FAST MODE
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;//设置时序占空比为50%
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;//采用7位地址模式
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {Error_Handler(); }
}

```

该协议层配置利用CubeMX自带的配置系统，选定I2C号后，设置时钟为快速模式，然后设置时序占空比，四轴I2C采用7地址模式，自有地址1和2都设置为0，不使能双倍地址和通用呼叫模式，但是使能I2C中断。

下面进行I2C物理层配置：

代码2-4 I2C物理层实现

```c
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
  	GPIO_InitTypeDef GPIO_InitStruct = {0};//定义初始化结构体
    __HAL_RCC_GPIOB_CLK_ENABLE();//使能GPIO时钟
    //PB6     ------> I2C1_SCL
    //PB7     ------> I2C1_SDA
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;//选择引脚
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;//复用开漏输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;//上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//高速模式
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;//引脚复用
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//初始化GPIO
    __HAL_RCC_I2C1_CLK_ENABLE();//使能IIC时钟
}

```

物理层需要设置I2C使用的GPIO引脚，定义SDL和SCK并且初始化引脚时钟。因为STM32有固定复用给I2C的引脚，所以我们只需要查询手册进行对应的配置即可。

**TIM驱动代码实现**

TIM1协议层配置（SoftTick计数中断）:

代码2-5 TIM1协议层配置

```c
htim1.Instance = TIM1;
htim1.Init.Prescaler = 99;//PSC设置为99
htim1.Init.CounterMode = TIM_COUNTERMODE_UP;//向上计数
htim1.Init.Period = 999;//设置ARR为999
htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//不分频
htim1.Init.RepetitionCounter = 0;
htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

```

代码2-6 TIM1中断配置

```c
HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 0);
HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
```

首先是计数中断的配置。我们直到TIM是挂载到APB2总线上的，在时钟配置中我们以及将APB2的时钟配置为100MHz。为达到1us的节拍，经过公式计算，我们将TIM1的PSC配置为99，将ARR配置为999，设置向上计数模式，不分频。这样就能够每隔1us产生一个中断了。然后我们将这个中断配置到用户程序的最高优先级，即不能被其他的外设中断打断。

TIM2/4协议层配置（输入捕获中断）：

代码2-7 TIM2协议层配置

```c
//TIM2
htim2.Instance = TIM2;
htim2.Init.Prescaler = 99;//将节拍调整到1us
htim2.Init.CounterMode = TIM_COUNTERMODE_UP;//向上计数
htim2.Init.Period = 4294967295;//32位寄存器默认最大值
htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//不分频
htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
```

代码2-8 TIM4协议层配置

```c
//TIM4
htim4.Instance = TIM4;
htim4.Init.Prescaler = 99;//将节拍调整到1us
htim4.Init.CounterMode = TIM_COUNTERMODE_UP;//向上计数
htim4.Init.Period = 65535;//16位寄存器默认最大值
htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//不分频
htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
```

配置TIM2与TIM4的PWM输入捕获。在物理层中，设置PA0\~PA3四个引脚对应TIM2的四个通道，PB8与PB9对饮TIM4的一二通道。对相关时钟进行使能，然后对GPIO引脚模式进行配置，最后初始化GPIO，配置并使能中断。在协议层中，我们也将节拍调整到1us，然后将ARR寄存器配置为最大值，用于准备捕获中断。

TIM2/4物理层配置：

代码2-9 TIM2/4物理层配置

```c
GPIO_InitTypeDef GPIO_InitStruct = {0};
if(tim_icHandle->Instance==TIM2)
{
  __HAL_RCC_TIM2_CLK_ENABLE();//使能TIM2时钟
  __HAL_RCC_GPIOA_CLK_ENABLE();//使能GPIOA时钟
  //PA0-WKUP     ------> TIM2_CH1
  //PA1     ------> TIM2_CH2
  //PA2     ------> TIM2_CH3
  //PA3     ------> TIM2_CH4
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;//选择引脚（对应接收机1、2、3、4通道）
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;//下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;//低速模式
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;//引脚复用
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//GPIO初始化
  //TIM2中断配置
  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);//中断优先级3
  HAL_NVIC_EnableIRQ(TIM2_IRQn);//使能中断
}
else if(tim_icHandle->Instance==TIM4)
{
  __HAL_RCC_TIM4_CLK_ENABLE();//使能TIM4时钟
  __HAL_RCC_GPIOB_CLK_ENABLE();//使能GPIOB时钟
  //PB8     ------> TIM4_CH3
  //PB9     ------> TIM4_CH4
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;//选择引脚（对应接收机5、6通道）
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;//下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;//低速模式
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;//引脚复用分组
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//GPIO初始化
  //TIM4中断配置
  HAL_NVIC_SetPriority(TIM4_IRQn, 2, 0);//中断优先级2
  HAL_NVIC_EnableIRQ(TIM4_IRQn);//使能中断
}

```

TIM3物理层配置：

代码2-10 TIM3物理层配置

```c
__HAL_RCC_GPIOA_CLK_ENABLE();//GPIOA时钟使能
__HAL_RCC_GPIOB_CLK_ENABLE();//GPIOB时钟使能
//PA6     ------> TIM3_CH1
//PA7     ------> TIM3_CH2
//PB0     ------> TIM3_CH3
//PB1     ------> TIM3_CH4
GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;//引脚选择
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//复用推挽输出
GPIO_InitStruct.Pull = GPIO_PULLDOWN;//下拉
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;//低速模式
GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;//引脚复用分组
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//GPIOA初始化

GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;//引脚选择
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//复用推挽输出
GPIO_InitStruct.Pull = GPIO_PULLDOWN;//下拉
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;//低速模式
GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;//引脚复用分组
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//GPIOB初始化
```

最后是配置TIM3的PWM输出。物理层配置与之前类似，四个电机分别对应PA6、PA7、PB0、PB1.协议层中，查询了电机和电调的说明书，配置频率为50Hz的PWM，用于电机转速的控制。

代码2-11 TIM3协议层配置

```c
//输出50HzPWM波用于电机的转速控制。
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 199;//PSC设置199
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;//向上计数
  htim3.Init.Period = 9999;//ARR设置9999
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//不分频
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
```

**USART驱动代码实现**

USART物理层配置：

代码2-12 USART物理层配置

```c
__HAL_RCC_USART1_CLK_ENABLE();//USART1时钟使能
__HAL_RCC_GPIOA_CLK_ENABLE();//GPIOA时钟使能
//PA9     ------> USART1_TX
//PA10     ------> USART1_RX
GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;//引脚选择
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//复用推挽输出
GPIO_InitStruct.Pull = GPIO_NOPULL;//浮空
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//高速模式
GPIO_InitStruct.Alternate = GPIO_AF7_USART1;//引脚复用分组
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//GPIOA初始化

//USART1中断配置
HAL_NVIC_SetPriority(USART1_IRQn, 4, 0);//中断优先级4
HAL_NVIC_EnableIRQ(USART1_IRQn);//使能中断
```

USART协议层配置：

代码2-13 USART协议层配置

```c
huart1.Instance = USART1;
huart1.Init.BaudRate = 115200;//波特率
huart1.Init.WordLength = UART_WORDLENGTH_8B;//8位字长
huart1.Init.StopBits = UART_STOPBITS_1;//1停止位
huart1.Init.Parity = UART_PARITY_NONE;//无校验位
huart1.Init.Mode = UART_MODE_TX_RX;//全双工模式
huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
huart1.Init.OverSampling = UART_OVERSAMPLING_16;//16倍过采样
```

Printf函数重定向

代码2-14 输出函数重定向

```c
//重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
```

**2.功能模块驱动编码实现**

在本节并没有将所有外设功能模块的驱动代码予以详细分析，这样太过于啰嗦，因此只对核心配置流程进行阐述，主要涉及GY-86、ESP8266、接收机与电机。

**GY-86驱动**

首先利用I2C通信协议封装GY-86的读写函数，能够实现读写功能。

代码2-15 MPU写函数

```c
//IIC写一个字节 reg:寄存器地址data:数据
//返回值:0,正常
//      其他,错误代码
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data) 				 
{ 
  extern I2C_HandleTypeDef MPU_I2C;
  unsigned char W_Data=0;
  W_Data = data;
  HAL_I2C_Mem_Write(&MPU_I2C, (addr<<1), reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  return 0;
}
```

代码2-16 MPU读函数

```c
//IIC读一个字节 reg:寄存器地址 返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg,uint8_t *data)
{	extern I2C_HandleTypeDef MPU_I2C;
 	 HAL_I2C_Mem_Read(&MPU_I2C, (addr<<1), reg, I2C_MEMADD_SIZE_8BIT, 
data, 1, 0xfff);
  return 0;
}
```

配置完成基础读写操作后，则可以进行对照手册进行MPU的初始化。

代码2-17 MPU6050初始化

```c
//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU6050_Init(void)
{ 
  uint8_t res;
  extern I2C_HandleTypeDef MPU_I2C;
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_1,0X80);//复位MPU6050
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_1,0X00);//唤醒MPU6050 
  MPU_Set_Gyro_Fsr(3);//陀螺仪传感器,±2000dps
  MPU_Set_Accel_Fsr(0);//加速度传感器,±2g
  MPU_Set_Rate(50);//设置采样率50Hz
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_INT_ENABLE,0X00);//关闭所有中断
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_USER_CTRL,0X00);//I2C主模式关闭
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_FIFO_EN,0X00);//关闭FIFO
  MPU_Write_Byte(MPU_ADDR,MPU6050_RA_INT_PIN_CFG,0X80);//INT引脚低电平有效
  MPU_Read_Byte(MPU_ADDR,MPU6050_RA_WHO_AM_I,&res);
  if(res==MPU_ADDR)//器件ID正确
  {
    MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_1,0X01);//设置CLKSEL,PLL X轴为参考
    MPU_Write_Byte(MPU_ADDR,MPU6050_RA_PWR_MGMT_2,0X00);//加速度与陀螺仪都工作
    MPU_Set_Rate(50);//设置采样率为50Hz
  }else
  {return 1;}
  return 0;
}
```

完成MPU6050初始化后，紧接着进行HMC的初始化，原理相同。

代码2-18 HMC5885L初始化

```c
//HMC初始化配置
void HMC_Init(void)
{
	HMC_Write_Byte(HMC_CONFIGA, 0x58);   //01011000/采样平均数4，输出速率75Hz，正常测量配置
	HMC_Write_Byte(HMC_CONFIGB, 0xE0);   //11100000/将增益调至最小
	HMC_Write_Byte(HMC_MODE, 0x00);      //00000000/设置为连续模式
}
```

最后再次对以上两个函数进行封装，完成GY-86模块的初始化。

代码2-19 GY-86初始化

```c
//GY-86初始化配置
void GY86_Init(void)
{
	MPU6050_Init();
	MPU_Write_Byte(MPU_ADDR,MPU_CFG, 0x02);//将MPU的CFG寄存器的第二位设置为1，其他位在使用MPU时配置
	MPU_Write_Byte(MPU_ADDR,MPU_CTRL, 0x00);//将MPU的CTRL寄存器的第六位设置为0，与上面一步共同开启bypass模式
	HAL_Delay(200);
	HMC_Init();//HMC初始化
	GY86_SelfTest();
}
```

配置完成以上代码后，则可以交给业务层进行相关业务的处理，如采集GY-86姿态数据，或者利用DMP进行硬件解算等。硬件解算功能模块代码后面会详讲。

**ESP8266驱动**

ESP8266主要基于USART通讯协议，在正常WIFI传输之前要使用一系列的AT指令进行配置，比如说配置TCP/IP协议，配置从机模式，配置透传模式等。相关驱动代码如下：

在完成配置之后只需要调用串口输出即可输出到上位机显示。

代码2-20 ESP8366驱动代码

```c
//8266的TCP_Client透传模式配置
void esp8266_ap_cipsend_init(void)
{	
	esp8266_cmd("AT+RST\r\n");  //复位模块
    HAL_Delay(500);
    esp8266_cmd("AT+CWMODE=1\r\n"); //设置为AP模式
    HAL_Delay(500);
    esp8266_cmd("AT+CWJAP=\"WuhuTakeOff\",\"uestc404\"\r\n");   //WiFi基本设置
    HAL_Delay(15000);//此处可以通过判定返回信息来替换
    esp8266_cmd("AT+CIPMUX=0\r\n");   //单链路设置
    HAL_Delay(500);
    esp8266_cmd("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8080\r\n"); //TCP服务器
    HAL_Delay(500);
    esp8266_cmd("AT+CIPMODE=1\r\n"); //透传
    HAL_Delay(500);
	esp8266_cmd("AT+CIPSEND\r\n"); //透传开始
    HAL_Delay(500);
}
```

**接收机PWM捕获**

接收机驱动主要是完成对接收机信号的捕获和解析，其重点是PWM的输入捕获。在使用之前需要先初始化捕获中断，将时钟频率置为100MHz，这样CNT每经过1us计数一次。

在之前接收机驱动设计的时候已经详细绘制过接收机中断捕获的流程图，下面对该流程图进行代码实现。

代码2-21 接收机中断捕获

```c
//输入捕获中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
      for(i = 0; i < 4; i++)    
      {
        if(htim->Channel == ActiveChannel[i] && Duty[5] > 0.07)
        {
           cap = 1;    //标志是否进行了一次捕获
           switch(Flag[i])//捕获状态
           {
              case 0://第一次捕获到上升沿
              __HAL_TIM_SET_COUNTER(htim,0);  //计数器置0
              TIM_RESET_CAPTUREPOLARITY (htim, Channel[i]);  //重置触发条件
              TIM_SET_CAPTUREPOLARITY(htim,Channel[i],
              TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
              Flag[i]++;  //修改捕获状态
              break;

              case 1://第一次捕获到下降沿
              CapVal[i] = HAL_TIM_ReadCapturedValue(htim,Channel[i]); //读取计数器值
              if(CapVal[i] <= 2500)   //过滤超时情况
              Duty[i] = CapVal[i] / Cycle;    //计算占空比，Cycle = 20000us
              Motor_Set(Duty[i], Channel[i]); //更改单机转速
              //准备进行下一次捕获
              TIM_RESET_CAPTUREPOLARITY(htim, Channel[i]);   //重置触发条件
              TIM_SET_CAPTUREPOLARITY(htim,Channel[i],
TIM_ICPOLARITY_RISING);   //设置为下降沿捕获
              Flag[i] = 0;    //重置捕获状态
              break;
            }
          }
          if(cap)
          {
              cap = 0;
              break;
          }
        }
    }

```

按照中断捕获的原理，用标志位判断是否进行一次捕获，以及是否捕获到上升沿或者下降沿中断。

如果捕获到了上升沿中断，则表示中断首次开始，将修改标志位，然后重置触发条件，设置为下降沿捕获，期间一直在计时。

如果第一次捕获到了下降沿中断，则表示一个PWM捕获完成了，则可以计算占空比了。计算完占空比立即更新电机的转速，然后重置捕获，准备进行下一次捕获。

如果在过程中遇到了上升沿开始后计数器数满了但是还是没捕获到下降沿，则应该采用周期性计数，记录下当前的循环状态，然后清零计数器，重新进行下降沿捕获，直到捕获到了为止。结合周期进行占空比的计算与更新。

续

```c
else if(htim->Instance == TIM4)
    {
        for(i = 4; i < 6; i++)
        {
            if(htim->Channel == ActiveChannel[i-2])
            {
                cap = 1;
                switch(Flag[i])//捕获状态
                {
                    case 0:
                    __HAL_TIM_SET_COUNTER(htim,0);
                    TIM_RESET_CAPTUREPOLARITY (htim, Channel[i-2]);
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i-2],
TIM_ICPOLARITY_FALLING);
                    Flag[i]++;
                    break;

                    case 1:
                    CapVal[i] = HAL_TIM_ReadCapturedValue(htim,Channel[i-2]);
                    if(CapVal[i] <= 2500)   //过滤超时情况
                    Duty[i] = CapVal[i] / Cycle;//Cycle = 20000us
                    if(Duty[5] <= 0.055)
                    Motor_Lock();   //电机锁定
                    TIM_RESET_CAPTUREPOLARITY(htim, Channel[i-2]);
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i-2],
TIM_ICPOLARITY_RISING);
                    Flag[i] = 0;
                    break;
                }
            }
            if(cap)
            {
                cap = 0;
                break;
            }
        }
    }
}
```

**DMP硬件姿态解算驱动**

如果选择了使用DMP姿态解算，那么前面的GY-86初始化实际上可以直接调用DMP固件库函数完成。

可以用以下代码完成基本接口的对接

代码2-22 DMP接口对接

```c
#define i2c_write   MPU_Write_Len
#define i2c_read    MPU_Read_Len
#define delay_ms    HAL_Delay
#define get_ms      mget_ms
```

下面利用DMP固件库完成模块的初始化：

代码2-23 DMP初始化

```c
u8 mpu_dmp_init(void) //mpu6050,dmp初始化  /返回值:0,正常   其他,失败
{	u8 res=0;
	if(mpu_init()==0)	//初始化MPU6050
	{	 res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//设置所需要的传感器
		if(res)return 1; 
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置FIFO
		if(res)return 2; 
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//设置采样率
		if(res)return 3; 
		res=dmp_load_motion_driver_firmware();		//加载dmp固件
		if(res)return 4; 
	res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(res)return 5; 
	res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL);//设置DMP功能
		if(res)return 6; 
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//设置DMP输出速率		if(res)return 7;   
		res=run_self_test();		//自检
		if(res)return 8;    
		res=mpu_set_dmp_state(1);	//使能DMP
		if(res)return 9;     
	}return 0;
}
```

先设置所需要的传感器，在设置FIFO，然后规定采样率，接着加载DMP固件。完成以上步骤后我们将设置陀螺仪方向，然后设置DMP功能。这是功能的函数有点长，有许多参数，需要重点掌握。然后是设置DMP的速率，最大不能超过200MHz。最后进行DMA的自建和使能。在以上过程中，每一步都是哟res进行返回值的接收，如果返回值为0则代表初始化失败，需要重新初始化，则执行return函数退出。

至此所有外设功能模块驱动编写完成。

**3.应用层软件编码实现与系统测试**

完成所有外设功能模块的驱动编写后，我们下一步将进行系统联调，使所有模块按照前后台程序进行运行，然后上位机中能收到四轴飞行器发来的数据，并且能够解算出实时的姿态。最后通过测试程序完成软件部分的测试。

**上位机数据传送程序**

上位机程序采用“匿名上位机”的方案，调用串口收发进行源数据的传输。但是数据的结构需要按照匿名上位机的数据帧格式进行安排，以Status功能为例：

| 帧名称 | 帧头 1byte | 发送设备 1byte | 目标设备 1byte | 功能字 1byte | 数据长度 1byte | 数据 N byte                                                                                   | 和校验 1byte | 注释                              |
|--------|------------|----------------|----------------|--------------|----------------|-----------------------------------------------------------------------------------------------|--------------|-----------------------------------|
| VER    | 0xAA       | S_ADDR         | D_ADDR         | 00           | LEN            | uint8 HardwareType uint16 HardwareVER\*100  uint16 SoftwareVER\*100  uint16BootloaderVER\*100 | SUM          | 版本信息                          |
| STATUS | 0xAA       | S_ADDR         | D_ADDR         | 01           | LEN            | int16 ROL\*100  int16 PIT\*100  int16 YAW\*100  int32 ALT_USE  uint8 FLY_MODEL  uint8 ARMED   | SUM          | 姿态等基本信息 ARMED：0锁定 1解锁 |

表格 2-1 匿名上位机数据帧



对应代码如下页

代码2-24 传送数据帧

```c
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
    u8 _cnt=0;vs16 _temp;vs32 _temp2 = alt;
    data_to_send[_cnt++]=0xAA;//数据
    data_to_send[_cnt++]=0x05;//发送设备编
	data_to_send[_cnt++]=0xAF;//接收设备编
    data_to_send[_cnt++]=0x01;//功能
    data_to_send[_cnt++]=0x0C;//数据长度
    _temp = (int)(angle_rol*100)
    data_to_send[_cnt++]=BYTE1(_temp);  data_to_send[_cnt++]=BYTE0(_temp)
    _temp = (int)(angle_pit*100)
    data_to_send[_cnt++]=BYTE1(_temp);  data_to_send[_cnt++]=BYTE0(_temp)
    _temp = (int)(angle_yaw*100)
    data_to_send[_cnt++]=BYTE1(_temp);  data_to_send[_cnt++]=BYTE0(_temp);
    data_to_send[_cnt++]=BYTE3(_temp2);  data_to_send[_cnt++]=BYTE2(_temp2);
    data_to_send[_cnt++]=BYTE1(_temp2);  data_to_send[_cnt++]=BYTE0(_temp2);
    data_to_send[_cnt++] = fly_model;  data_to_send[_cnt++] = armed;
    u8 sum = 0
    for(u8 i=0;i<_cnt;i++)
        sum += data_to_send[i]
    data_to_send[_cnt++]=sum;
    ANO_DT_Send_Data(data_to_send, _cnt)
}
```


在完成姿态解算后，我们将用前后台逻辑使得能够利用遥控器控制四个电机的旋转。

**软件联调测试**

在实施方案一章我们描述过整个软件系统的运行逻辑，在此处我们再次用一张图表的形式展示以下整个软件的前后台关系：

![](media/a53fdd8325f196b5fdb82cb282d727d1.png)

图2-19 前后台逻辑示意图



最后的测试代码如下：

代码2-25 前后台程序测试

```c
while (1)//后台程序
{	if(SoftTick%50==2)//OLED屏幕刷新
	{	OLED_Show_3num(Gx, Gy, Gz, 0);
		OLED_Show_3num(Ax, Ay, Az, 1);
		OLED_Show_3num(Mag_x, Mag_y, Mag_z, 2);
		OLED_Show_3num(Cap[0], Cap[1], Cap[2], 3);
		OLED_Show_3num(Cap[3], Cap[4], Cap[5], 4);
		OLED_ShowNum(24, 7, MPU_Get_Temperature(),2,12);
	}	if(SoftTick%2==1)//上位机数据传输
	{	for(int i = 0; i < 6; i++)
			Cap[i] =  (Duty[i] * 100 - 5) / 0.05;
		ANO_DT_Send_Status(roll,pitch,yaw,2333,122,(Cap[5]>50));
		ANO_DT_Send_Senser(Ax,Ay,Az,Gx,Gy,Gz,2333,2333,2333);
		ANO_DT_Send_RCData(0,0,0,0,Cap[0],Cap[1],Cap[2],Cap[3],Cap[4],Cap[5]);
	}
	read_Gyroscope_DPS();
	read_Accelerometer_MPS();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//溢出中断，用于1ms计时。
{	
	if(SoftTick>1000)SoftTick=0;
	SoftTick++;
	mpu_dmp_get_data(&pitch,&roll,&yaw);//读取姿态数据  
}
```



#  第三章 存在问题与解决方案

本章将归纳并解决我们在实际操作中遇到的问题，以供大家参考，避免踩坑

## 3.1 存在的主要问题

**硬件问题**

1.设计绘制PCB板对于小组来讲是全新的内容，期间遇到硬件选型，绘制错误等诸多问题。

2.外设功能模块能够通过单元测试，但是在集成测试时出现运行不稳定的情况。

**软件问题**

1.在集成测试中发现软件执行路径错误、陷入中断、数据丢失等问题。

**调试问题**

1.程序运行情况与想象不一致。

2.上位机通信数据更新慢，导致画面卡顿。

## 3.2 问题原因与解决方案

## PCB板制作过程中的问题

>问题1：一开始找不到一些元器件的原理图库。

解决方案：下载一个Altium designer
Samacsys插件，输入元器件具体型号可以直接搜索到原理图和各种封装图。或者可以使用立创EDA设计PCB，利用立创EDA的在线库的完成原理图库、封装图的搜索。

>问题2：布线无法完成，有一些线无论如何都没有通路连接起来。

解决方案：重新调整布局，重新布线，适当缩小过孔大小，尽可能通过增加过孔遵循正面走竖线，反面走横线原则，留出更多空间。

>问题3：因为一些非常规的设计导致需要重新映射引脚到PCB，易引起出错。

解决方案：需要重新映射的引脚要对应PCB图进行仔细检查，基于现实中的硬件布局、排布方向进行核对。

>问题4：在上电之后出现部分电路网络发热过大。

解决方案：将特殊网络如：3v3、5v、GND的线宽增加，避免发热过大烧坏电路。

**I2C部分问题**

>问题：在使用软件I2C时，如果模拟FAST模式会导致稳定性变差。

问题原因：软件I2C需要模拟I2C协议的时序，按照协议要求要么2:1要么16:9，但是在实际运行的过程中当提高频率后时序误差会越来越大，虽然I2C时序有一定的容错率但是误差过大则会影响I2C运行的稳定性。

解决方案：在我们讨论、研究和进行相关测试后，最终选择更换为硬件I2C来提供I2C相关的底层支持。

**TIM部分问题**

>问题：无法退出TIM的捕获中断回调函数。

问题原因：最初在配置捕获中断时GPIO引脚采用的浮空输入，在实际运行的时候引脚会出现杂波波动，导致一直在触发PWM捕获中断，从而无法从中断回调函数中退出。

解决方案：在配置TIM物理层时将GPIO引脚下拉或者上拉，避免电平的跳变即可。

**GY-86驱动部分问题**

>问题：运行一段时间后出现无法读取数据的情况。

问题原因：1.MPU6050对电压要求较高，电压不稳定会导致模块重启，但是又未初始化导致无法运行。2.MPU6050的FIFO溢出导致停止运行。

解决方案：1.保证电池电量充足，同时在后期我们又重新设计了电源稳压模块可以解决这个问题。2.严格控制程序执行时间，尽力保证程序的实时性，所以引入了SoftTick提供程序运行的心跳，并利用TIM的CNT来严格测量了每一个模块执行所需时间。

**ESP8266部分问题**

>问题：在回传数据至上位机的过程中发现速度不到10fps，导致实时数据更新太慢，模拟的飞控的画面看起来有一些卡顿。

问题原因：最初采用了下位机作为TCP_Server，而上位机作为TCP_Client。依据AT指令集的使用手册，在向上位机传输数据之前需要先指定Client，也就是需要ESP8266应答一次，那么每进行一次数据传输都需要ESP8266应答，但是由于ESP8266的性能有限，应答速度大概在50ms一次拉，极大地低了MCU数据帧的发送速率。

解决方案：在研究了AT指令集之后，我们发现TCP协议传输中有透传模式，但是其限制了只有Client允许使用透传，所以我们将下位机更改为了Client而上位机为Server，这样在向上位机传输数据的时候就免去了ESP8266的应答过程。使数据传输速度能达到50帧以上每秒，呈现出比较流畅的模拟画面。

# 第四章 执行情况与完成度

| **任务阶段**            | **执行情况**                                                                               | **完成度** |
|-------------------------|--------------------------------------------------------------------------------------------|------------|
| **分工与时间安排**      | 根据组员时间合理安排课程设计进度，大致执行情况与安排一致，小任务有几天出入                 | 已完成     |
| **技能学习与 知识准备** | 因为组内成员都有一定功底，所以学习时间安排了一个周，边学边做，在实践中进行探索式的学习     | 已完成     |
| **可行性分析**          | 经过对技术、环境等各方面的综合考量，确定了任务的可行性分析                                 | 已完成     |
| **需求分析**            | 进行了该项目的需求分析和数据流图、结构框架图的绘制                                         | 已完成     |
| **方案设计**            | 在指导老师和学长前辈们的建议和组内成员的讨论后，确定了项目的概要设计                       | 已完成     |
| **硬件设计**            | 确定项目所需要的硬件设施，采购并完成硬件的单独调试                                         | 已完成     |
| **软件设计**            | 确定项目需要的软件与驱动模块，对详细功能点和技术点进行细化                                 | 已完成     |
| **硬件搭建与 代码实现** | 小组成员根据硬件设计和软件设计的要求，分别进行对应硬件搭建工作和程序编写                   | 已完成     |
| **项目测试**            | 将所有硬件和对应的软件集成，在交叉开发环境中进行测试。确保实现所有的功能，调试提升系统性能 | 已完成     |
| **文档报告撰写**        | 按照项目要求整合所有材料，撰写报告书，制作答辩PPT并准备展示和提交                          | 已完成     |

# 第五章 分工协作与交流情况

**知识技能学习情况**

我们小组认真学习并掌握了 STM32
编程的基本语法，深刻理解并充分认识到了四轴飞行器所包含的技术价值以及发展空间。

我们本学期的任务就是搭建出设计一架四轴飞行器所需要的硬件设施并对各个模块进行初始化，使四轴飞行器能够实现在遥控器操作下四个电机不同方向和速度的转动，以及GY-86传感器通过
ESP8266 模块将传感器采集的数据信息传送到电脑端打印输出的操作。

在整个实验过程中，我们不断学习了 STM32
固件库相关函数的调用，并阅读部分固件库源代码，尝试使用直接对寄存器进行操作，深刻理解了
STM32 的内部结构，不断调成驱动程序，最终选择了最优的程序完成课程设计。

**分工合作与交流情况**

1.  小组成员：3 人

    组长：徐洋

    组员：余子潇，唐昊哲

2.  主要负责模块：

    软件技术负责：徐洋，唐昊哲

    硬件技术负责：余子潇

    报告文案负责：余子潇

3.  每人完成的详细任务

    徐洋：ESP8266，I2C，接收机等相关模块驱动编写，系统总体设计。

    唐昊哲：一体化飞控板制作，辅助开发，材料采购。

    余子潇：辅助开发，硬件焊接调试，资料收集，课设报告等文案撰写。

4.  交流情况：

在整个项目开发和测试阶段，我们小组主要依靠程序和文档实时共享的模式进行开发。利用NAS网络附属存储管理所有的程序，图片以及视频。组内人员各司其职分工明确，当遇到问题时，一般采用网上讨论+线下实践，学习开发文档以及网络资源后，再当面交流沟通的方式，高效完成开发任务。

# 参考文献

[1] 廖勇.四轴飞行器设计[J].2019.2

[2] STM32F4xx 中文参考手册[J].意法半导体

[3] STM32 固件库使用手册[J].意法半导体

[4] STM32F4 开发指南-寄存器版本[J].正点原子

[5] 零死角玩转 STM32—F103[J].野火

# 致谢

本报告的工作是在我们敬爱的指导教师廖勇老师的悉心指导下完成的。廖老师在整个综合课程设计中主动帮助我们分析问题。组织大家共同讨论，制定任务，让同学们互相监督，共同进步。遇到无法解决的问题时廖老师都一一耐心讲解。对整个课程设计的指导从知识讲授、文档撰写到答辩技巧无微不至。在此，谨向老师表达我们衷心的感谢和崇高的敬意！

另外，感谢学院给予我们这样的机会，能够独立的完成一个课程设计，并在这个过程当中，为我们提供教师资源以及学习资源的帮助，使我们能够学以致用，真正地将课堂中学习到的知识转为实际的计算机系统结构能力，增强了我们的实践操作水平以及合作交流分工能力。

还要感谢在课程设计完成的过程中帮助过我们的老师、前辈和同学们。感谢你们的提出的宝贵经验和建议，让我们的工作少走弯路，没有你们的耐心指导，就没有项目的顺利完成。

最后再次对帮助给我们的所有人表示由衷的感谢。
