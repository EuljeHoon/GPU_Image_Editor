build/ImageEditor: build/main.o build/glad.o build/Window.o build/Image.o build/Texture.o build/ShaderProgram.o build/TexturedRectangle.o build/Button.o build/ColorButton.o build/Application.o
	g++ build/main.o build/glad.o build/Window.o build/Image.o build/Texture.o build/ShaderProgram.o build/TexturedRectangle.o build/Button.o build/ColorButton.o build/Application.o -o build/ImageEditor -lglfw -lGL -ldl

build/main.o: src/main.cpp
	mkdir -p build
	g++ -Iinclude -Ilib -c src/main.cpp -o build/main.o

build/glad.o: lib/glad/glad.c 
	mkdir -p build
	g++ -Iinclude -Ilib -c lib/glad/glad.c -o build/glad.o

build/Image.o: src/Image.cpp include/Image.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/Image.cpp -o build/Image.o

build/Texture.o: src/Texture.cpp include/Texture.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/Texture.cpp -o build/Texture.o

build/ShaderProgram.o: src/ShaderProgram.cpp include/ShaderProgram.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/ShaderProgram.cpp -o build/ShaderProgram.o

build/TexturedRectangle.o: src/TexturedRectangle.cpp include/TexturedRectangle.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/TexturedRectangle.cpp -o build/TexturedRectangle.o

build/Button.o: src/Button.cpp include/Button.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/Button.cpp -o build/Button.o

build/ColorButton.o: src/ColorButton.cpp include/ColorButton.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/ColorButton.cpp -o build/ColorButton.o

build/Window.o: src/Window.cpp include/Window.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/Window.cpp -o build/Window.o

build/Application.o: src/Application.cpp include/Application.h
	mkdir -p build
	g++ -Iinclude -Ilib -c src/Application.cpp -o build/Application.o

clean:
	rm -rf build