PROG=llcmd
CC=g++
BINDIR=$(DESTDIR)/usr/bin
INSTALL=cp

CURRENT_DIR = $(shell pwd)

BASESRC=$(CURRENT_DIR)/src/basecmd/*.cpp
SPECSRC=$(CURRENT_DIR)/src/speccmd/*.cpp
UTILSSRC=$(CURRENT_DIR)/src/utils/*.cpp
OBJ = $(CURRENT_DIR)/*.o

#添加指定目录的头文件和 链接库
INCL_PATH+=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/x86_64-linux-gnu
LINKFLAGS+=-lglib-2.0 -lgio-2.0 -lcurl

$(PROG):llmain.cpp basecmd speccmd utils 
	$(CC) -o $(PROG) llmain.cpp $(OBJ) $(LINKFLAGS) $(INCL_PATH)

basecmd:$(BASESRC)
	$(CC) -c $(BASESRC)
speccmd:$(SPECSRC)
	$(CC) -c $(SPECSRC) $(INCL_PATH)
utils:$(UTILSSRC)
	$(CC) -c $(UTILSSRC) $(INCL_PATH)

print:
#echo 前面添加@符号是为了避免命令本身输出到终端
	@echo $(BINDIR)
	@echo $(CURRENT_DIR)
clean:
	rm -rf $(PROG) *.o
install:
	mkdir -p $(BINDIR)
	$(INSTALL) $(PROG) $(BINDIR)
uninstall:
	rm -rf $(BINDIR)/$(PROG) 	


