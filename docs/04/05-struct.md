```c
// 1.第一种方式直接定义
struct coord
{
	int x;
	int y;
};
// 实例化声明变量
struct coord first, second;
```
```c
// 2.第二种方式声明方式直接定义结构附带声明两个该结构的变量
struct coord
{
	int x;
	int y;
} first, second;
```
```c
// 3.第三种方式使用 typeof 给结构体或共用体创建别名
typedef struct
{
	int x;
	int y;
} coord;
// 如此一来创建变量的时候就相对于第一种要简洁一些
coord firsti, second;
```
```c
// 4.还可以在结构体声明完以后给结构体添加别名,例如下面 nginx 里面的代码
struct ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};
 
typedef struct ngx_command_s     ngx_command_t;
 
static ngx_command_t xxx...;
```
```c
typedef struct odbc_param_info {
	SQLSMALLINT sqltype;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLULEN precision;
} odbc_param_info;
```

我们后面大多使用第三种方式