## 获取终端字符列数

动态输出进度条，动态的效果，我们可以通过`\r`回车转义字符，将光标移动到行首，然后主动刷新缓冲区，则可以起到覆盖上次输出的数据的效果。还有一个难题，就是如何获取终端的字符列数，以便做字符串输出时的百分比效果。

`shell`下，可以执行`tput cols`获取字符列数，比如
```bash
$ tput cols
128
```
当我们改变终端窗口大小之后运行的结果是变化的，在 C 中可以通过`ioctl`向设备发控制和配置命令，配合`TIOCGWINSZ`命令获得终端设备的窗口大小。

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
	struct winsize size;
	if (isatty(STDOUT_FILENO) == 0) {
		exit(1);
	}
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0) {
		perror("ioctl TIOCGWINSZ error");
		exit(1);
	}
	printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
	return 0;
}
```
## 具体实现
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main()
{
	struct winsize size;
	if (isatty(STDOUT_FILENO) == 0) {
		exit(1);
	}
	ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
	int width = size.ws_col;

	const char *progress = "[]100%";
	width = width - strlen(progress);

	char width_str[10] = { 0 };
	sprintf(width_str, "%d", width);

	char progress_format[20] = { 0 };

	strcat(progress_format, "[%-");
	strcat(progress_format, width_str);
	strcat(progress_format, "s]%d%%\r");

	char progress_bar[width + 1];
	memset(progress_bar, 0, width + 1);

	for (int i = 1; i <= width; i++) {
		strcat(progress_bar, "=");
		printf(progress_format, progress_bar, (i * 100 / width));
		fflush(stdout);
		usleep(10000);
	}

	printf("\n");
	return 0;
}
```
`progress_format`实际是`[%-ns]%%`，其中`n`就是终端的字符列数，也就是说输出的字符串总长度是`n`，不足的位用空格补充，这样在输出进度条的时候，最末尾的值的位置就是固定的；%%输出百分号。

赶快运行下吧，是不是很炫酷的动态进度条效果。