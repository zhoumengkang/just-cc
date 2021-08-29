如果使用的是 vimplus 在 `~/.vimrc.local` 后面追加一段自定义快捷键代码，常规 vim 则在`~/.vimrc`中添加。代码如下
```bash
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Quickly Run
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

map <F5> :call CompileRunGcc()<CR>

func! CompileRunGcc()
    exec "w"
    if &filetype == 'c'
        exec '!gcc -Wall -std=gnu99 -g % -o %<'
        exec '! ./%<'
    elseif &filetype == 'cpp'
        exec '!g++ % -o %<'
        exec '! ./%<'
    elseif &filetype == 'python'
        exec '! python %'
    elseif &filetype == 'php'
        exec '! php %'
    elseif &filetype == 'sh'
        :! bash %
    endif
endfunc
```
这样 F5 就是我们的编译运行快捷键了。在运行完之后会在当前目录生成一个同名的可执行文件，而不是默认的`a.out`了。