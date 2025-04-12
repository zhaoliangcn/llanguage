ARCH = $(shell getconf LONG_BIT) # 获取系统架构位数
BUILD?=DEBUG # 默认构建类型为DEBUG
CC := g++ # 使用g++编译器
CFLAGS := -Wno-attributes -Wno-deprecated -fPIC # 编译选项
ifeq ($(BUILD),DEBUG)
CFLAGS+=-g # 如果是DEBUG构建，添加-g选项
endif
TARGET := libScriptEngineLib.a # 目标库文件
SRCS := $(wildcard *.cpp) # 获取所有cpp文件
OBJS := $(patsubst %cpp,%o,$(SRCS)) # 将cpp文件替换为o文件
all:$(TARGET) # 默认目标
%.o:%.cpp
	$(CC) $(CFLAGS) -DLinux$(ARCH) -c $< # 编译cpp文件为o文件
$(TARGET):$(OBJS)
	ar -rc $(TARGET) $^ ../EasyJson/EasyJson/CppEasyJson.o # 创建静态库
clean:
	rm -rf $(TARGET) *.o # 清理生成的文件
