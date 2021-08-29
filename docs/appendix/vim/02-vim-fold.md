## 新增 vim 配置项
如果使用的是 vimplus 在 `~/.vimrc.local` 后面追加一段自定义快捷键代码，常规 vim 则在`~/.vimrc`中添加。代码如下

```bash
set foldmethod=marker
```
当用 vim 打开如下代码时则默认是折叠中
```c
/* {{{ deliverer_module_entry
 */
zend_module_entry deliverer_module_entry = {
    STANDARD_MODULE_HEADER,
    "deliverer",
    deliverer_functions,
    PHP_MINIT(deliverer),
    PHP_MSHUTDOWN(deliverer),
    PHP_RINIT(deliverer),       /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(deliverer),   /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(deliverer),
    PHP_DELIVERER_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */
```

## 操作快捷键

快捷键 | 含义
----- | -----
zo      | open 打开折叠
zc      | close 关闭折叠
