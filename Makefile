# 注意修改安装目录和Mathematica版本
MMA_DIR=D:\Program Files\Wolfram Research\Mathematica\11.3

## 编译器和编译选项根据实际情况修改
CC=gcc
CFLAGS=-O2 -s -DNDEBUG

MMA_INC=$(MMA_DIR)\SystemFiles\IncludeFiles\C
MMA_DLL=$(MMA_DIR)\SystemFiles\Libraries\Windows-x86-64\WolframRTL.dll
MMA_STR=-I "$(MMA_INC)" "$(MMA_DLL)"

ffmslink.dll: main.c ffms2.dll
	$(CC) main.c ffms2.dll $(CFLAGS) $(MMA_STR) -shared -o ffmslink.dll
