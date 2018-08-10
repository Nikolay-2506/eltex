Компиляция:
	make
Действия после компиляции:
	cd /sys/kernel/kobject_example
	chmod 0666 foo
	echo "15" > foo
	cat foo
