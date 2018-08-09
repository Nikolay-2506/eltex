Компиляция:
	make

sudo insmod mod_proc.ko

После компиляции:
	echo "new string" > /proc/mod_proc
	cat /proc/mod_proc

	sudo rmmod mod_proc
