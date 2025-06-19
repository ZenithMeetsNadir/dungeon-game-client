UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
else ifeq ($(UNAME_S),Linux)
    OS_NAME := Linux
endif

ASSETS_DIR := assets
EXECUTABLE := dungeonGame

ifeq ($(OS_NAME),Windows)
	OUT_DIR := output
	MKDIR_OUT := @if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	COPY_ASSETS := @xcopy /E /Y /I $(ASSETS_DIR)\. $(OUT_DIR)\$(ASSETS_DIR)
	EXECUTABLE_PATH := $(OUT_DIR)\$(EXECUTABLE)
else ifeq ($(OS_NAME),Linux)
	OUT_DIR := .
	EXECUTABLE_PATH := $(OUT_DIR)/$(EXECUTABLE)
endif

_build:
	@echo Building project...
	cmake -B build -D CMAKE_INSTALL_PREFIX=$(OUT_DIR)
	cmake --build build

up: _build mkdir_output copy_assets install
out: _build mkdir_output copy_assets install run

mkdir_output:
	@echo Creating output directory...
	$(MKDIR_OUT)

copy_assets:
	@echo Copying assets to $(OUT_DIR)/$(ASSETS_DIR)...
	$(COPY_ASSETS)

install:
	@echo Installing to $(OUT_DIR)...
	cmake --install build

run: $(EXECUTABLE_PATH)
	@echo Running the game...
	$(EXECUTABLE_PATH)

$(EXECUTABLE_PATH): install