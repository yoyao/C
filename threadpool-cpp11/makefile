
#先用-E预处理 然后用预处理的文件加-S变成汇编 由汇编变成so库  或者用汇编文件.s编成.o 然后用ar打包成静态库也行

CXX=g++

FLAGS= -std=c++11 -g -Wall -pthread -Wno-deprecated
dirs=$(shell find ./threadpool -type d)
hpps=$(foreach tdir,$(dirs),$(wildcard $(tdir)/*.hpp))

objs=$(patsubst %.hpp,%.o,$(hpps))
pres=$(patsubst %.hpp,%.ii,$(hpps))
assms=$(patsubst %.ii,%.s,$(pres))




#相比-g选项， -rdynamic 却是一个 连接选项 ，它将指示连接器把所有符号（而不仅仅只是程序已使用到的外部符号）都添加到动态符号表（即.dynsym表）里，以便那些通过 dlopen() 或 backtrace() （这一系列函数使用.dynsym表内符号）这样的函数使用。

#target=libthreadpool.so
target=libthreadpool.a

all:$(target)

#编成目标 即库文件
$(target):$(objs)
	#$(CXX) -o $@ -fPIC -shared -rdynamic $^ $(FLAGS)
	ar -crv $@  $^ 

#把汇编编成.o文件
$(objs):%.o:%.s
	$(CXX) -c -o $@  $< $(FLAGS)


#把.hpp文件预处理
$(pres):%.ii:%.hpp
	$(CXX) -E -o $@  $< $(FLAGS)

#预处理文件编成汇编
$(assms):%.s:%.ii
	$(CXX) -S -o $@  $< $(FLAGS)

clean:
	find . -name "*.ii"| xargs rm -rf
	find . -name "*.s"| xargs rm -rf
	find . -name "*.o"| xargs rm -rf
	rm -rf $(target)



.PHONY:clean all





























