
include make_envs.mk
obj: 
	@if [ ! -d $(TARGET_DIR) ]; then mkdir $(TARGET_DIR); fi
	$(CC) $(CFLAGS) $(SRCS) $(INCLUDES) $(LFLAGS) -o $(TARGET_DIR)/$(OBJS) $(LIBS)
debug: $(GEN_BIN_DEBUG) 
	@if [ ! -d $(TARGET_DIR) ]; then mkdir $(TARGET_DIR); fi
	$(CC) $(CFLAGS) $(DFLAGS) $(SRCS) $(INCLUDES) $(LFLAGS) -o $(TARGET_DIR)/$(OBJS) $(LIBS)
bin_debug: 
	@if [ ! -d $(TARGET_DIR) ]; then mkdir $(TARGET_DIR); fi
	$(CC) $(BIN_CFLAGS) $(SRCS) $(INCLUDES) $(LFLAGS) -o $(TARGET_DIR)/$(TARGET) $(LIBS)
	
all: obj
	@if [ ! -d $(TARGET_DIR) ]; then mkdir $(TARGET_DIR); fi
	$(CC) $(BIN_CFLAGS) $(SRCS) $(INCLUDES) $(LFLAGS) -o $(TARGET_DIR)/$(TARGET) $(LIBS)

clean:
	@if [ -d $(TARGET_DIR) ]; then rm -rf $(TARGET_DIR); fi
