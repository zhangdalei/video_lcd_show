v4l2_fb: camera_pic.o fb.o
	arm-linux-gcc -o $@ $^ -static
%.o: %.c
	arm-linux-gcc -c -o $@ $< -static
clean:
	rm -f *.o v4l2_fb