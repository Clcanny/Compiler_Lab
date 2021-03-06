# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99 -ggdb

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: clean syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test: parser
	 for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do echo; echo Now testing test$$i.cmm; valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="mem_log_$$i" ./parser ../Test/OneError/test$$i.cmm > test_result_$$i.output; echo Comparing; diff test_result_$$i.output ../Expect/OneError/$$i.output; wc -l mem_log_$$i; done;
clean:
	- rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	- rm -f $(OBJS) $(OBJS:.o=.d)
	- rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	- rm -f *~
	- rm mem_log_*
	- rm test_result_*.output
