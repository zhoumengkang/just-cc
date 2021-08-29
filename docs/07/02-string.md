接着上节的问题
```c
#include <stdio.h>

int main()
{
    char a = 1;
    char b = '1';
    char *c = "abcdef";

    printf("a:%d\n",a);
    printf("b:%d\n",b);
    printf("c:%s\n",c);
    return 0;
}
```
## 单引号
单引号用来引用单个字符，可以利用 ASCII 码映射表查出对应的数字。

## 双引号的秘密

双引号做了3件事：  
- 在**常量区**申请内存，存放字符串 
- 在字符串尾加上了`\0`    
- 返回字符串的首地址

所以这里的赋值操作是将`abcdef\0`的首地址赋值给了`c`。

