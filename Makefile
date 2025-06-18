UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
else ifeq ($(UNAME_S),Linux)
    OS_NAME := Linux
endif

OUT_DIR := output
ASSETS_DIR := assets

ifeq ($(OS_NAME),Windows)
	MKDIR_OUT := @if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	COPY_ASSETS := @xcopy /E /Y /I $(ASSETS_DIR)\. $(OUT_DIR)\$(ASSETS_DIR)
else ifeq ($(OS_NAME),Linux)
	MKDIR_OUT := @mkdir -p $(OUT_DIR)
	COPY_ASSETS := @cp -r $(ASSETS_DIR)/. $(OUT_DIR)/$(ASSETS_DIR)
endif

install: mkdir_output _build copy_assets
	@echo Installing to $(OUT_DIR)...
	cmake --install build

mkdir_output:
	@echo Creating output directory...
	$(MKDIR_OUT)

_build:
	@echo Building project...
	cmake --build build

copy_assets:
	@echo Copying assets to $(OUT_DIR)/$(ASSETS_DIR)...
	$(COPY_ASSETS)