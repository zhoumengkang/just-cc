为了满足最基本的使用条件，首先我们需要了解如下知识，以下文件都可以在`/usr/include/getopt.h`中，我们直接用 vim 就能看到
```bash
vim /usr/include/getopt.h
```
> 下面我的介绍说的比较简单，找到一篇介绍比较完整的，大家可以对比着看 https://blog.ibaoger.com/2017/08/08/getopt-long/

## 两个全局变量
```c
char *optarg;  //当前选项的值
int optind;  //argv 中要处理的下一个元素的索引
```
## 长选项结构体
```c
struct option
{
  const char *name; // 长选项的名称
  int has_arg; // 是否必须传值、可选传值、不接受值
  int *flag; // 一般都设置为 NULL
  int val; // 对应的短选项的名称
};
```
## 三个常量
```c
# define no_argument        0
# define required_argument  1
# define optional_argument  2
```
## 获取选择函数
```c
extern int getopt_long (int ___argc, char *const *___argv,
            const char *__shortopts,
                const struct option *__longopts, int *__longind)
       __THROW;
```
## 具体实现
### 选项结构体初始化
```c
static struct option const long_opts[] = {
        {"output-document", required_argument, NULL, 'O'},
        {"debug",           no_argument,       NULL, 'd'},
        {"version",         no_argument,       NULL, 'V'},
        {"help",            no_argument,       NULL, 'h'},
        {NULL,              0,                 NULL, 0}
};
```
?> 为什么最后要多插入一行空数据呢？其实是为了遍历数组的时候不发生越界，这是 c 里面的一个常用小套路

### 循环获取选项的值
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#define PROGRAM_NAME "mywget"


static struct option const long_opts[] = {
        {"output-document", required_argument, NULL, 'O'},
        {"debug",           no_argument,       NULL, 'd'},
        {"version",         no_argument,       NULL, 'V'},
        {"help",            no_argument,       NULL, 'h'},
        {NULL,              0,                 NULL, 0}
};

void usage(int status) {
    printf("Usage %s [OPTION]... [URL]...\n", PROGRAM_NAME);

    if (status != 0) {
        fprintf(stderr, "Try '%s --help' for more information.\n", PROGRAM_NAME);
    } else {
        fprintf(stdout, "  -O,  --output-document=FILE  write documents to FILE\n");
        fprintf(stdout, "  -d,  --debug                 print debug output\n");
        fprintf(stdout, "  -h,  --help                  display this help and exit\n");
        fprintf(stdout, "  -V,  --version               output version information and exit\n");
    }

    exit(status);
}


int main(int argc, char *argv[]) {
    int  optc;
    char *filename;
    bool debug = false;

    for (int i = 0; i < argc; i++) {
        printf("argv[%d]\t%s\n", i, argv[i]);
    }

    while ((optc = getopt_long(argc, argv, "O:dVh", long_opts, NULL)) != -1) {
        printf("optc:%c\toptarg:%s\toptind:%d\n", optc, optarg, optind);
        switch (optc) {
            case 'O':
                filename = optarg;
                break;
            case 'd':
                debug = true;
                break;
            case 'V':
                printf("%s 0.0.1\n", PROGRAM_NAME);
                exit(0);
            case 'h':
                usage(0);
                break;
            case '?':
                usage(1);
                break;
        }
    }

    printf("optind:%d\targc:%d\n", optind, argc);

    if (optind + 1 != argc) {
        usage(1);
    }

    for (int   i    = 0; i < argc; i++) {
        printf("argv[%d]\t%s\n", i, argv[i]);
    }

    const char *url = argv[optind];

    printf("url\t%s\nfilename\t%s\ndebug\t%d\n", url, filename, debug);

    return 0;
}
```
## 测试
```
$ ./usage -d --output-document bb -O AA http://aaa.com
argv[0]	./usage
argv[1]	-d
argv[2]	--output-document
argv[3]	bb
argv[4]	-O
argv[5]	AA
argv[6]	http://aaa.com
optc:d	optarg:(null)	optind:2
optc:O	optarg:bb	optind:4
optc:O	optarg:AA	optind:6
optind:6	argc:7
argv[0]	./usage
argv[1]	-d
argv[2]	--output-document
argv[3]	bb
argv[4]	-O
argv[5]	AA
argv[6]	http://aaa.com
url	http://aaa.com
filename	AA
debug	1
```