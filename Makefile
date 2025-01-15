CFLAGS = -lSDL2 -lm
TARGET = cloth_simulation

all:
	gcc main.c particle.c constraint.c input_handler.c -o $(TARGET) $(CFLAGS)

clean:
	rm -rf $(TARGET)