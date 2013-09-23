SET PATH=d:\Keil\ARM\ARMCC\bin\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\Common Files\Thunder Network\KanKan\Codecs;C:\Program Files\TortoiseSVN\bin;C:\Program Files\Java\jdk1.7.0_07\bin;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files\TortoiseGit\bin;C:\Program Files\Microsoft Visual Studio\Common\Tools\WinNT;C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin;C:\Program Files\Microsoft Visual Studio\Common\Tools;C:\Program Files\Microsoft Visual Studio\VC98\bin;C:\Program Files\android\tools;C:\Program Files\IDM Computer Solutions\UltraCompare\;C:\Program Files\IDM Computer Solutions\UEStudio\
SET CPU_TYPE=STM32F103ZG
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=myos
SET CPU_CLOCK=0x007A1200
"d:\Keil\ARM\ARMCC\bin\ArmAsm" --Via "..\flashout\stm32f10x._ia"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\main.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\os_task.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\os_core.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\stm32f10x_gpio.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\stm32f10x_lib.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\stm32f10x_nvic.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\stm32f10x_pwr.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\stm32f10x_rcc.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\stm32f10x_systick.__i"
"d:\Keil\ARM\ARMCC\bin\ArmAsm" --Via "..\flashout\cortexm3_macro._ia"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\bsp.__i"
"d:\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\flashout\cpu.__i"
"d:\Keil\ARM\ARMCC\bin\ArmAsm" --Via "..\flashout\cpu_a._ia"
