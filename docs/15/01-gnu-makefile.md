每次我们下载一个开源软件包的时候，看到里面文件特别多，其实仔细一个看 C 语言代码只有一个文件，代码量也不多。很多都是工具生成为了编译安装的，比如这个软件`spawn-fcgi`，只有一个文件`src/spawn-fcgi.c`

```bash
$ tree spawn-fcgi-1.6.4
spawn-fcgi-1.6.4
├── AUTHORS
├── CMakeLists.txt
├── COPYING
├── Makefile.am
├── Makefile.in
├── NEWS
├── README
├── aclocal.m4
├── autogen.sh
├── compile
├── config.h.in
├── configure
├── configure.ac
├── depcomp
├── doc
│   ├── Makefile.am
│   ├── Makefile.in
│   ├── apparmor.d-abstractions-spawn-fcgi
│   ├── run-generic
│   ├── run-php
│   └── run-rails
├── install-sh
├── missing
├── spawn-fcgi.1
└── src
    ├── CMakeLists.txt
    ├── Makefile.am
    ├── Makefile.in
    ├── config.h.cmake
    └── spawn-fcgi.c
```
我们这一章节就把 `mywget` 项目包装下，我们首先手写 gnu makefile，然后了解 makefile 之后，我们再通过 autotools 来生成 makefile