.PHONY: build

EXE = NEXUS_ENGINE
CONFIG = Debug

all: build compile run

build:
	cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=$(CONFIG)

compile:
	cmake --build build

run:
	./build/NEXUS_EDITOR/NEXUS_EDITOR.exe	
