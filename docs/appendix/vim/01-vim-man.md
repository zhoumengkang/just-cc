快速查看手册找到需要引入的头文件

使用 `man.vim`插件

插件配置
```bash
" 查看方法输入:Man api_name
source $VIMRUNTIME/ftplugin/man.vim
" 映射之后就可以少按一下 Shift 键。
cmap man Man
" 在普通模式下按下 K （大写）即可启动 man 查看光标下的函数。
nmap K :Man 3 <cword><CR>
```