## 项目目录
```bash
.
├── http_download.c
├── http_download.h
├── log.c
├── log.h
├── main.c
├── progress.c
├── progress.h
├── test
├── url_parse.c
├── url_parse.h
├── util.c
├── util.h
├── wget_opt.c
└── wget_opt.h
```

## 原始方案
```bash
gcc `ls *.c` -std=gnu99 -o mywget
```
一步到位，写着很简单，但是在大型软件工程中，可能有成千个c文件，如果修改一行代码所有的代码都需要重新编译，无异于噩梦。所以这才有了`*.o`文件。
### gcc 模块化
```bash
$ ls *.c|awk '{print "gcc -Wall -std=gnu99 -c "$1}'
gcc -Wall -std=gnu99 -c http_download.c
gcc -Wall -std=gnu99 -c log.c
gcc -Wall -std=gnu99 -c main.c
gcc -Wall -std=gnu99 -c progress.c
gcc -Wall -std=gnu99 -c url_parse.c
gcc -Wall -std=gnu99 -c util.c
gcc -Wall -std=gnu99 -c wget_opt.c
```
所以我们可以这样执行，初次编译
```bash
$ ls *.c|awk '{print "gcc -Wall -std=gnu99 -c "$1}'|sh
$ gcc `ls *.o` -o mywget
```
假如又更新了`http_download.c`，那么我们只需要
```bash
$ gcc -Wall -std=gnu99 -c http_download.c
$ gcc `ls *.o` -o mywget
```
但是又有了新问题，如果一个文件更新所有依赖的文件的编译都需要更新，肯定有办法能扫描出来呢？
`gcc -M` 可以把依赖的头文件输出出来，包括系统头文件；`gcc -MM` 可以把依赖的头文件输出出来，排除系统头文件。
```bash
$ gcc -MM *.c
http_download.o: http_download.c log.h url_parse.h http_download.h progress.h util.h
log.o: log.c log.h
main.o: main.c url_parse.h log.h http_download.h wget_opt.h
progress.o: progress.c progress.h
url_parse.o: url_parse.c url_parse.h log.h
util.o: util.c util.h
wget_opt.o: wget_opt.c wget_opt.h
```
从上面的结果可以看出，如果更新了`log.h`，依赖的模块很多。命令手动操作也还是比较麻烦，所以如此看来必须得有个新的工具来管理项目的编译。那就是 Makefile 了。

## Makefile 基础
> 官方手册 https://www.gnu.org/software/make/manual/html_node/#toc-Overview-of-make
内容太多，官方也不推荐一个新手上来就细读，https://www.gnu.org/software/make/manual/html_node/Reading.html#Reading 
### 规则
`Makefile`由n组规则组成，每组规则如下
```makefile
target ... : prerequisites ... 
	command1
	command2
	...
```
> 目标（target）和条件（Prerequisite）之间的关系是：欲更新目标，必须首先更新它的所有条件；所有条件中只要有一个条件被更新了，目标也必须随之被更新。所谓“更新”就是执行一遍规则中的命令列表，命令列表中的每条命令必须以一个Tab开头，注意不能是空格，Makefile的格式不像C语言的缩进那么随意，对于Makefile中的每个以Tab开头的命令，make会创建一个Shell进程去执行它。

那么我们最初的 gcc 的编译方式就可以这样来写

```
mywget : http_download.c  log.c  main.c  progress.c  url_parse.c  util.c  wget_opt.c
	gcc http_download.c  log.c  main.c  progress.c  url_parse.c  util.c  wget_opt.c -Wall -std=gnu99 -o mywget
```
?> 复制以上代码到你的项目中应该会报错，记得把第二行的缩进删掉，在 vim 敲 tab 键才行

### 特殊变量
> https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html#Automatic-Variables
> https://mengkang.net/book-c/ch22s03.html

`$@`，表示规则中的目标。
`$<`，表示规则中的第一个条件。
`$?`，表示规则中所有比目标新的条件，组成一个列表，以空格分隔。
`$^`，表示规则中的所有条件，组成一个列表，以空格分隔。

所以我们的 Makefile 可以变为
```makefile
mywget : http_download.c  log.c  main.c  progress.c  url_parse.c  util.c  wget_opt.c
	gcc $^ -Wall -std=gnu99 -o $@
```
这样当我们项目增加c文件时，需要编辑的就少了一行，有没有办法把条件里的文件也动态替换呢？

### 变量赋值
makefile 变量赋值的套路和我们c程序赋值不一样，有单行的、多行的；单行和多行的又分为 6 种，具体大家可以看官方手册 https://www.gnu.org/software/make/manual/html_node/Reading-Makefiles.html#Reading-Makefiles

根据需要我们介绍2种，`:=`运算符和`=`运算符的
```
a := 1$(c)
b  = 1$(c)
c  = 2

all :
	@echo a:$(a)
	@echo b:$(b)
```
```bash
$ make
a:1
b:12
```
解释
```
a := 1$(c) // 简单展开，此时 c 没有赋值所以 a = 1
b  = 1$(c) // 递归展开，c 后面赋值为 2 所以这里 b = 12
```
> https://mengkang.net/book-c/ch22s03.html

### 使用 shell 函数
> https://www.gnu.org/software/make/manual/html_node/Shell-Function.html#Shell-Function

例如
```makefile
contents := $(shell cat foo)
files := $(shell echo *.c)
```
所以我们的 Makefile 可以写成这样
```makefile
src = $(shell echo *.c)

mywget : $(src)
	@echo src: $(src)
	gcc $^ -Wall -std=gnu99 -o $@
```  
执行
```
$ make
src: http_download.c log.c main.c progress.c url_parse.c util.c wget_opt.c
gcc http_download.c log.c main.c progress.c url_parse.c util.c wget_opt.c -Wall -std=gnu99 -o mywget
```
## Makefile 模块化
按照上面的命令行直接使用`gcc -MM *.c`生成`.o`文件的方式，照搬放在 Makefile
```makefile
CFLAGS = -Wall -std=gnu99

mywget: http_download.o log.o main.o progress.o url_parse.o util.o wget_opt.o
	gcc $(CFLAGS) -o $@ $^	
http_download.o: http_download.c log.h url_parse.h http_download.h progress.h util.h
    gcc $(CFLAGS) -c -o $@ $<
log.o: log.c log.h
    gcc $(CFLAGS) -c -o $@ $<
main.o: main.c url_parse.h log.h http_download.h wget_opt.h
    gcc $(CFLAGS) -c -o $@ $<
progress.o: progress.c progress.h
    gcc $(CFLAGS) -c -o $@ $<
url_parse.o: url_parse.c url_parse.h log.h
    gcc $(CFLAGS) -c -o $@ $<
util.o: util.c util.h
    gcc $(CFLAGS) -c -o $@ $<
wget_opt.o: wget_opt.c wget_opt.h
    gcc $(CFLAGS) -c -o $@ $<
```
### 使用内置函数完成模块化
官方提供字符串替换函数
> https://www.gnu.org/software/make/manual/html_node/Text-Functions.html#Text-Functions

```makefile
$(var:suffix=replacement)
$(patsubst %suffix,%replacement,$(var))
```
当我们想通过`*.c`获取对应的`*.o`文件列表，我们可以通过下面两种方式来获取
```makefile
src = $(shell echo *.c)

objects1 = $(src:.c=.o)
objects2 = $(patsubst %.c,%.o,$(src))

mywget :
	@echo $(objects1)
	@echo $(objects2)
```
执行测试下
```bash
$ make
http_download.o log.o main.o progress.o url_parse.o util.o wget_opt.o
http_download.o log.o main.o progress.o url_parse.o util.o wget_opt.o
```
### 隐含规则和模式规则

在 make 中定义了一些隐含规则，在隐含规则中有一条`%.o: %.c`的模式规则

> https://mengkang.net/book-c/ch22s02.html

```makefile
# default
OUTPUT_OPTION = -o $@

# default
CC = cc

# default
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.c
#  commands to execute (built-in):
        $(COMPILE.c) $(OUTPUT_OPTION) $<
```

所以`*.o: %.c`都会去默认去执行`cc`编译（把上面的`$(COMPILE.c)`和`$(OUTPUT_OPTION)`带入之后），**可以省略**
```makefile
cc $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<
```
> cc 链接到了 gcc （在GNU/Linux可以认为是同一个文件）


我们把前面用到的`gcc -MM *.c`的结果添加到`Makefile`里
```makefile
src = $(shell echo *.c)
objects = $(src:.c=.o)

mywget : $(objects)
	gcc $^ -Wall -std=gnu99 -o $@

http_download.o: http_download.c log.h url_parse.h http_download.h progress.h util.h
log.o: log.c log.h
main.o: main.c url_parse.h log.h http_download.h wget_opt.h
progress.o: progress.c progress.h
url_parse.o: url_parse.c url_parse.h log.h
util.o: util.c util.h
wget_opt.o: wget_opt.c wget_opt.h
```
我们需要把 `CFLAGS` 设置为 `-Wall -std=gnu99`，如果我们全局都需要使用这个参数，直接统一赋值即可；如果只希望某种匹配模式才会设置 `CFLAGS` 变量，可以这样
```makefile
%.o : CFLAGS = -Wall -std=gnu99
```
> https://www.gnu.org/software/make/manual/make.html#Pattern_002dspecific

所以我们的 Makefile 可以写成

```make
src = $(shell echo *.c)
objects = $(src:.c=.o)

mywget : $(objects)
	gcc $^ $(CFLAGS) -o $@

http_download.o: http_download.c log.h url_parse.h http_download.h progress.h util.h
log.o: log.c log.h
main.o: main.c url_parse.h log.h http_download.h wget_opt.h
progress.o: progress.c progress.h
url_parse.o: url_parse.c url_parse.h log.h
util.o: util.c util.h
wget_opt.o: wget_opt.c wget_opt.h

CFLAGS = -Wall -std=gnu99
```

`CFLAGS`放在`Makefile`代码的最前面，最后面都行

## 动态管理自动生成先决条件

同样`gcc -MM *.c`的结果也是会变的，也不应该是每次执行完，然后拷贝到 Makefile 里，肯定有办法在 Makefile 里动态的管理。

方案是把`gcc -MM xx.c`的结果存发送到`xx.d`文件里，然后再通过`include`引入进来

下面这条规则来源于官方手册推荐写法，每次编译都会重新生成`.d`文件
> https://www.gnu.org/software/make/manual/make.html#Automatic-Prerequisites
```make
%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
```
```bash
$ gcc -MM mywget.c
mywget.o: mywget.c url.h log.h wget.h wget_opt.h
$ cat mywget.d
mywget.o mywget.d : mywget.c url.h log.h wget.h wget_opt.h
```
> 具体解释可以参考 https://mengkang.net/book-c/ch22s04.html

所以最后我们的 Makefile 文件是
```make
src = $(shell echo *.c)
objects = $(src:.c=.o)

mywget : $(objects)
	gcc $^ $(CFLAGS) -o $@

include $(src:.c=.d)

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

CFLAGS = -Wall -std=gnu99
```


执行如下
```
$ make
Makefile:7: http_download.d: 没有那个文件或目录
Makefile:7: log.d: 没有那个文件或目录
Makefile:7: main.d: 没有那个文件或目录
Makefile:7: progress.d: 没有那个文件或目录
Makefile:7: url_parse.d: 没有那个文件或目录
Makefile:7: util.d: 没有那个文件或目录
Makefile:7: wget_opt.d: 没有那个文件或目录
set -e; rm -f wget_opt.d; \
cc -MM  wget_opt.c > wget_opt.d.$$; \
sed 's,\(wget_opt\)\.o[ :]*,\1.o wget_opt.d : ,g' < wget_opt.d.$$ > wget_opt.d; \
rm -f wget_opt.d.$$
set -e; rm -f util.d; \
cc -MM  util.c > util.d.$$; \
sed 's,\(util\)\.o[ :]*,\1.o util.d : ,g' < util.d.$$ > util.d; \
rm -f util.d.$$
set -e; rm -f url_parse.d; \
cc -MM  url_parse.c > url_parse.d.$$; \
sed 's,\(url_parse\)\.o[ :]*,\1.o url_parse.d : ,g' < url_parse.d.$$ > url_parse.d; \
rm -f url_parse.d.$$
set -e; rm -f progress.d; \
cc -MM  progress.c > progress.d.$$; \
sed 's,\(progress\)\.o[ :]*,\1.o progress.d : ,g' < progress.d.$$ > progress.d; \
rm -f progress.d.$$
set -e; rm -f main.d; \
cc -MM  main.c > main.d.$$; \
sed 's,\(main\)\.o[ :]*,\1.o main.d : ,g' < main.d.$$ > main.d; \
rm -f main.d.$$
set -e; rm -f log.d; \
cc -MM  log.c > log.d.$$; \
sed 's,\(log\)\.o[ :]*,\1.o log.d : ,g' < log.d.$$ > log.d; \
rm -f log.d.$$
set -e; rm -f http_download.d; \
cc -MM  http_download.c > http_download.d.$$; \
sed 's,\(http_download\)\.o[ :]*,\1.o http_download.d : ,g' < http_download.d.$$ > http_download.d; \
rm -f http_download.d.$$
gcc http_download.o log.o main.o progress.o url_parse.o util.o wget_opt.o -Wall -std=gnu99 -o mywget
```

### 增加 clean 规则
```make
src = $(shell echo *.c)
objects = $(src:.c=.o)

mywget : $(objects)
	gcc $^ $(CFLAGS) -o $@

include $(src:.c=.d)

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean :
	-rm mywget *.d *.o
	
CFLAGS = -Wall -std=gnu99

.PHONY: clean
```
这样就可以执行`make clean`

### 增加 install uninstall 规则


> 目录命名上规则参考 https://www.gnu.org/prep/standards/html_node/Directory-Variables.html#Directory-Variables
> 目标文件命名上规则参考 https://www.gnu.org/prep/standards/html_node/Standard-Targets.html#Standard-Targets

按照规范，默认是需要支持`make all`的，用来表示生成二进制文件、库文件、文档等。再加上安装和卸载，最后命令如下：

```make
src = $(shell echo *.c)
objects = $(src:.c=.o)
bindir = /usr/local/bin

all : mywget

mywget : $(objects)
	$(CC) $^ $(CFLAGS) -o $@

include $(src:.c=.d)

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean :
	-rm mywget *.d *.o
	
install :
    install -d $(bindir)
    install -m 0775 mywget $(bindir)
    
uninstall :
    rm $(bindir)/mywget
	
CFLAGS = -Wall -std=gnu99

.PHONY: clean install uninstall all
``` 
自测下吧
```
$ make
$ make all
$ make clean
$ sudo make install
$ sudo make uninstall
```
