F1_TARGETS  += $(TARGET)
FLASH_SIZE  = 128
FEATURES    = VCP

TARGET_SRC = \
            drivers/accgyro/accgyro_mpu.c \
            drivers/accgyro/accgyro_mpu6050.c \
            blackbox/blackbox.c \
            blackbox/blackbox_io.c 

