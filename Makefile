ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all:
	g++ -std=c++17 -lstdc++fs -o UsefulLCD src/main.cpp

install: all
	install -m 755 ./UsefulLCD $(PREFIX)/bin/
	install -m 644 ./systemd/usefullcd.service /etc/systemd/system/usefullcd.service
	systemctl enable --now usefullcd.service

uninstall: 
	rm $(PREFIX)/bin/UsefulLCD
	systemctl disable --now usefullcd.service
	rm /etc/systemd/system/usefullcd.service

clean:
	rm ./UsefulLCD