all: compile link
compile:
	clang++ main.cpp Option.cpp Lobby.cpp Game.cpp VertexDeque.cpp Car.cpp -std=c++17 -c -I"C:\SFML-2.6.0\include"
	
link: 
	clang++ *.o -o main -L"C:\SFML-2.6.0\lib" -L"D:\sfmlcpp" -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-window -lsfml-system -lpugixml -lglfw3 -lUnitTest++
clean:
	rm -f *.o