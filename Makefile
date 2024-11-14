# Nome do executável
TARGET = mydht

# Compilador e flags de compilação
CXX = g++
CXXFLAGS = -Wall

# Arquivos de código-fonte
SOURCES = main.cpp dht.cpp

# Objetos compilados
OBJECTS = $(SOURCES:.cpp=.o)

# Regra principal
all: $(TARGET)

# Como construir o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Como compilar os arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos temporários
clean:
	rm -f $(TARGET) $(OBJECTS)

# Executa o programa
run: $(TARGET)
	./$(TARGET)
