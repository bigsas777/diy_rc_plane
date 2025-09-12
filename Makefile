.PHONY: build upload test clean clean-main


TARGET ?= plane

SRC_DIR      = src_$(TARGET)
SRC_MAIN     = $(SRC_DIR)/main.cpp
DEST_MAIN    = src/main.cpp

build:
	cp $(SRC_MAIN) $(DEST_MAIN)
	pio run -e $(TARGET)
	$(MAKE) clean-main

upload:
	cp $(SRC_MAIN) $(DEST_MAIN)
	pio run -e $(TARGET) -t upload
	$(MAKE) clean-main

test:
	cp test/main.cpp $(DEST_MAIN)
	pio run -e test -t upload
	$(MAKE) clean

clean:
	pio run -t clean
	$(MAKE) clean-main

clean-main:
	rm -f $(DEST_MAIN)
