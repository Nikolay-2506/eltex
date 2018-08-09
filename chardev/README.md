Компиляция:
	make

sudo insmod chardev_2_0
После компиляции нужно прописать следующие команды:
	sudo mknod /dev/chardev_2_0 c 700 0
	sudo chmod a+rw /dev/chardev_2_0
	
	echo "!" > /dev/chardev_2_0
	cat /dev/chardev_2_0

	sudo dmesg | tail -n20
	
	sudo rmmod chardev_2_0
