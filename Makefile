UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
else ifeq ($(UNAME_S),Linux)
    OS_NAME := Linux
endif

ASSETS_DIR := assets
DOTENV_DEFAULT := .default.env
DOTENV := .env
EXECUTABLE := dungeonGame

ifeq ($(OS_NAME),Windows)
	OUT_DIR := output
	MKDIR_OUT := @if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	MKDIR_ASSETS := @if not exist $(OUT_DIR)\$(ASSETS_DIR) mkdir $(OUT_DIR)\$(ASSETS_DIR)
	COPY_ASSETS := powershell -Command "Get-ChildItem -Recurse $(ASSETS_DIR) | ForEach-Object {$$dest = $$_.FullName.Replace('$(ASSETS_DIR)', '$(OUT_DIR)\$(ASSETS_DIR)'); if (!(Test-Path $$dest)) {Copy-Item $$_.FullName -Destination $$dest} }"
	SETUP_ENV := @if not exist $(ASSETS_DIR)\$(DOTENV) @copy $(ASSETS_DIR)\$(DOTENV_DEFAULT) $(ASSETS_DIR)\$(DOTENV)
	EXECUTABLE_PATH := $(OUT_DIR)\$(EXECUTABLE)
else ifeq ($(OS_NAME),Linux)
	OUT_DIR := .output
	SETUP_ENV := @if not exist $(ASSETS_DIR)/$(DOTENV) @cp $(DOTENV_DEFAULT) $(DOTENV)
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
	$(SETUP_ENV)
	$(COPY_ASSETS)
	$(RM_DEFAULT_ENV)

install:
	@echo Installing to $(OUT_DIR)...
	cmake --install build

run: $(EXECUTABLE_PATH)
	@echo Running the game...
	$(EXECUTABLE_PATH)

$(EXECUTABLE_PATH): install