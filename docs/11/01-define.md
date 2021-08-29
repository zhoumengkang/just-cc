## 宏替换
宏定义的形式如下：
```c
#define 名字 替换文本
```
`#define` 和 `#include` 一样，均为预处理指令。预处理器是编译过程中单独执行的第一步。
```c
#include <stdio.h>

#define PAI 3.14

int main()
{
    printf("%0.2f", PAI);
    return 0;
}
```
使用`gcc xx.c -save-temps -o xx`，然后查看预编译文件`xx.i`最后的代码为
```c
int main()
{
    printf("%0.2f", 3.14);
    return 0;
}
```
### 可变参数宏
```c
#include <stdio.h>

#define debug(...) printf(__VA_ARGS__)

int main()
{
    debug("%d\n", 1);
    return 0;
}
```
`...`缺省号代表一个可以变化的参数表

### 使用 do{}while(0)
在很多开源项目里宏的定义都用`do{}while(0)`包裹，有几个好处（可以先忽略，这里不展开演示了）

- 存在一个独立的代码块，可以用来进行变量定义，进行比较复杂的实现，和宏外部互不影响
- 空的宏定义避免`warning`
- 把宏定义成一个函数的简写，习惯性的在后面加`;`不会报错