.PHONY: plane controller clean

plane:
	cp src_plane/main.cpp src/main.cpp
	pio run -e plane -t upload
	rm -f src/main.cpp

controller:
	cp src_controller/main.cpp src/main.cpp
	pio run -e controller -t upload
	rm -f src/main.cpp

clean:
	pio run -t clean
	rm -f src/main.cpp