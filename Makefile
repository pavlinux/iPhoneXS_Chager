ipchrg: ipchrg.c
	gcc -Wall -Wextra ipchrg.c -lusb-1.0 -o ipchrg

install: ipchrg
	install -o root -g root -m 755 -s ipchrg /usr/bin/
	install -o root -g root -m 644 98-ipchrg.rules /etc/udev/rules.d/

uninstall:
	rm -rf /usr/bin/ipchrg
	rm -rf /etc/udev/rules.d/98-ipchrg.rules

clean:
	rm -f ipchrg

