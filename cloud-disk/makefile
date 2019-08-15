cfiles=$(wildcard ./*.c)
objfiles=$(patsubst %.c,%.out,$(cfiles))


all:$(objfiles)


$(objfiles):%.out:%.c
	gcc $< -o $@ -Wall -g 
