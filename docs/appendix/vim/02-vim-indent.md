## 方式1 gg=G
`vim`里保存文件之后，然后执行`gg=G`则自动格式化代码
## 方式2 indent
退出 `vim` 执行 `indent`
```bash
indent -linux xx.c
```
如果没有安装就 `yum install indent -y` 安装即可。

我是在测试脚本里每次编译之前，先格式化所有的代码。
```bash
ls *c *.h|xargs indent -linux
```
## 方式3 快捷键
```vim
map <F6> :call IndentCode() <CR>

func! IndentCode()
    exec "w"
    exec '!indent -linux %'
endfunc
```