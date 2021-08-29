如果还在习惯编辑`a`文件，然后保存退出编辑`b`文件，效率是非常低下的，在`vim`里有类似我们 ide 里多 tab 编辑的功能，叫`buffer`

- `:edit xxx.c` 新开一个 buffer
- `ctrl+p` 切换到上一个buffer
- `ctrl+n` 切换到下一个buffer
- `<leader>d` 删除当前buffer
- `<leader>D` 删除当前buffer外的所有buffer