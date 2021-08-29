这一节我们写一个最简单的 Makefile，我们的项目非常简单，项目仅有一个文件`hello.c`

我们可以通过下面的命令来编译
```bash
gcc -Wall -g -std=gnu99 -o hello hello.c
```

`Makefile`由n组规则组成，每组规则如下
```makefile
target ... : prerequisites ... 
	command1
	command2
	...
```
目标（target）和条件（Prerequisite）之间的关系是：欲更新目标，必须首先更新它的所有条件；所有条件中只要有一个条件被更新了，目标也必须随之被更新。所谓“更新”就是执行一遍规则中的命令列表，命令列表中的每条命令必须以一个Tab开头，注意不能是空格，Makefile的格式不像C语言的缩进那么随意，对于Makefile中的每个以Tab开头的命令，make会创建一个Shell进程去执行它。

那么我们用 Makefile 对齐进行封装

```Makefile
hello : hello.c
	gcc -Wall -g -std=gnu99 -o hello hello.c
```

?> 复制我的代码到你的项目中应该会报错，记得把第二行的缩进删掉，在 vim 敲 tab 键才行

目标文件命名上规则参考 https://www.gnu.org/prep/standards/html_node/Standard-Targets.html#Standard-Targets

按照规范，默认是需要支持`make all`的，用来表示生成二进制文件、库文件、文档等。再加上安装和卸载，最后命令如下：

```makefile
bindir = /usr/local/bin

all : hello

hello : hello.c
	gcc -Wall -g -std=gnu99 -o hello hello.c

clean :
	-rm hello

install :
	install -d $(bindir)
	install -m 0775 hello $(bindir)

uninstall :
	-rm $(bindir)/hello

.PHONY : clean install uninstall all
```

- `bindir`是变量定义，不过 Makefile 有多种变量复制的方式，这里先不展开说明
- `.PHONY` 表示伪目标文件，表示他里面指定的不是一个二进制文件
- `-rm`前面加上`-`用于抑制报错

把上面的代码写入文件`Makefile`，我们执行下我们熟知的 make 命令就可以自动识别该文件了

```bash
$ make clean
rm hello
$ ls
hello.c  Makefile
$ make all
  gcc hello.c -o hello
$ ls
  hello  hello.c  Makefile
$ sudo make install
install -d /usr/local/bin
install -m 0775 hello /usr/local/bin
```
我们执行`hello`而不是`./hello`，发现能够执行了，前提是确认`/usr/local/bin`在我们的环境变量 PATH 中
```bash
$ hello
hello, world
```
卸载
```bash
$ sudo make uninstall
rm /usr/local/bin/hello
$ hello
-bash: /usr/local/bin/hello: 没有那个文件或目录
```

通过这个实验相比大家对 Makefile 有简单的认知了。

> Makefile 的知识还有很多，还有兴趣学习的同学可以看下我的这篇博客，项目实战 [9 次迭代打造一个自动化的 Makefile](https://mengkang.net/1427.html)