# video_lcd_show
使用Makefile编译，编译命令：
make
make后生产v4l2_fb文件，
./v4l2_fb 0 表示使用/dev/video0
./v4l2_fb 1 表示使用/dev/video4
如果摄像头节点不同，自行修改打开摄像头节点的函数。
