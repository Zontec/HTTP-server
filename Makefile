PROJECT=server

include makefiles/Makefile.config
include makefiles/Makefile.include


all: $(BIN_DIR)/$(PROJECT)

$(BIN_DIR)/$(PROJECT):$(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(LD) $(LFLAGS) $^ -o $@

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)
	@rm -rf $(TMP_DIR)

rebuild: clean all
	@echo Rebuild done!

build-debug: 
	DEBUG=-g make rebuild

debug:
	DEBUG=-g make rebuild
	@echo Compiled in debug mode!!!
	$(DBG) $(DBGFLAGS) $(BIN_DIR)/$(PROJECT)

disasm: rebuild
	mkdir -p $(TMP_DIR)
	@objdump -d -M intel $(BIN_DIR)/$(PROJECT) >$(OUTPUT)
	@echo Task done! Saved in: $(OUTPUT)

start:rebuild
	@./$(BIN_DIR)/$(PROJECT)

start-log: 
	make LOG=DEBUG rebuild
	@./$(BIN_DIR)/$(PROJECT)


