CXX = g++
CXXFLAGS = -std=c++11 -Iimgui -Iimgui/backends -Isrc
LDFLAGS = -lglfw -lGL -ldl

SOURCES = src/main.cpp \
          src/PatientRecordsBST.cpp \
          imgui/imgui.cpp \
          imgui/imgui_demo.cpp \
          imgui/imgui_draw.cpp \
          imgui/imgui_tables.cpp \
          imgui/imgui_widgets.cpp \
          imgui/backends/imgui_impl_glfw.cpp \
          imgui/backends/imgui_impl_opengl3.cpp

OBJS = $(SOURCES:.cpp=.o)
TARGET = hospital_gui

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@