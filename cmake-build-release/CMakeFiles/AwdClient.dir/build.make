# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "E:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "E:\Program Files\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\CLionProjects\AsWeDrown\AwdClient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/AwdClient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AwdClient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AwdClient.dir/flags.make

packets.pb.h: ../net/packets.proto
packets.pb.h: E:/Program\ Files/CMake/bin/protoc.exe
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on net/packets.proto"
	"E:\Program Files\CMake\bin\protoc.exe" --cpp_out E:/CLionProjects/AsWeDrown/AwdClient/cmake-build-release -I E:/CLionProjects/AsWeDrown/AwdClient/net E:/CLionProjects/AsWeDrown/AwdClient/net/packets.proto

packets.pb.cc: packets.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate packets.pb.cc

CMakeFiles/AwdClient.dir/main.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/main.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/AwdClient.dir/main.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\main.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\main.cpp

CMakeFiles/AwdClient.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/main.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\main.cpp > CMakeFiles\AwdClient.dir\main.cpp.i

CMakeFiles/AwdClient.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/main.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\main.cpp -o CMakeFiles\AwdClient.dir\main.cpp.s

CMakeFiles/AwdClient.dir/packets.pb.cc.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/packets.pb.cc.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/packets.pb.cc.obj: packets.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/AwdClient.dir/packets.pb.cc.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\packets.pb.cc.obj -c E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\packets.pb.cc

CMakeFiles/AwdClient.dir/packets.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/packets.pb.cc.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\packets.pb.cc > CMakeFiles\AwdClient.dir\packets.pb.cc.i

CMakeFiles/AwdClient.dir/packets.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/packets.pb.cc.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\packets.pb.cc -o CMakeFiles\AwdClient.dir\packets.pb.cc.s

CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.obj: ../net/PacketTransformer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\net\PacketTransformer.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\net\PacketTransformer.cpp

CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\net\PacketTransformer.cpp > CMakeFiles\AwdClient.dir\net\PacketTransformer.cpp.i

CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\net\PacketTransformer.cpp -o CMakeFiles\AwdClient.dir\net\PacketTransformer.cpp.s

CMakeFiles/AwdClient.dir/net/PacketManager.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/net/PacketManager.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/net/PacketManager.cpp.obj: ../net/PacketManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/AwdClient.dir/net/PacketManager.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\net\PacketManager.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\net\PacketManager.cpp

CMakeFiles/AwdClient.dir/net/PacketManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/net/PacketManager.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\net\PacketManager.cpp > CMakeFiles\AwdClient.dir\net\PacketManager.cpp.i

CMakeFiles/AwdClient.dir/net/PacketManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/net/PacketManager.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\net\PacketManager.cpp -o CMakeFiles\AwdClient.dir\net\PacketManager.cpp.s

CMakeFiles/AwdClient.dir/game/Game.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/Game.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/Game.cpp.obj: ../game/Game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/AwdClient.dir/game/Game.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\Game.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\Game.cpp

CMakeFiles/AwdClient.dir/game/Game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/Game.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\Game.cpp > CMakeFiles\AwdClient.dir\game\Game.cpp.i

CMakeFiles/AwdClient.dir/game/Game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/Game.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\Game.cpp -o CMakeFiles\AwdClient.dir\game\Game.cpp.s

CMakeFiles/AwdClient.dir/net/UdpClient.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/net/UdpClient.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/net/UdpClient.cpp.obj: ../net/UdpClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/AwdClient.dir/net/UdpClient.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\net\UdpClient.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\net\UdpClient.cpp

CMakeFiles/AwdClient.dir/net/UdpClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/net/UdpClient.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\net\UdpClient.cpp > CMakeFiles\AwdClient.dir\net\UdpClient.cpp.i

CMakeFiles/AwdClient.dir/net/UdpClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/net/UdpClient.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\net\UdpClient.cpp -o CMakeFiles\AwdClient.dir\net\UdpClient.cpp.s

CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.obj: ../game/packetlistener/HandshakeResponseListener.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\packetlistener\HandshakeResponseListener.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\packetlistener\HandshakeResponseListener.cpp

CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\packetlistener\HandshakeResponseListener.cpp > CMakeFiles\AwdClient.dir\game\packetlistener\HandshakeResponseListener.cpp.i

CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\packetlistener\HandshakeResponseListener.cpp -o CMakeFiles\AwdClient.dir\game\packetlistener\HandshakeResponseListener.cpp.s

CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.obj: ../game/packetlistener/CreateLobbyResponseListener.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\packetlistener\CreateLobbyResponseListener.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\packetlistener\CreateLobbyResponseListener.cpp

CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\packetlistener\CreateLobbyResponseListener.cpp > CMakeFiles\AwdClient.dir\game\packetlistener\CreateLobbyResponseListener.cpp.i

CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\packetlistener\CreateLobbyResponseListener.cpp -o CMakeFiles\AwdClient.dir\game\packetlistener\CreateLobbyResponseListener.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.obj: ../game/graphics/Drawable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\Drawable.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\Drawable.cpp

CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\Drawable.cpp > CMakeFiles\AwdClient.dir\game\graphics\Drawable.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\Drawable.cpp -o CMakeFiles\AwdClient.dir\game\graphics\Drawable.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.obj: ../game/graphics/Button.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\Button.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\Button.cpp

CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\Button.cpp > CMakeFiles\AwdClient.dir\game\graphics\Button.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\Button.cpp -o CMakeFiles\AwdClient.dir\game\graphics\Button.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.obj: ../game/graphics/mainmenu/MainMenuScreen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\mainmenu\MainMenuScreen.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\mainmenu\MainMenuScreen.cpp

CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\mainmenu\MainMenuScreen.cpp > CMakeFiles\AwdClient.dir\game\graphics\mainmenu\MainMenuScreen.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\mainmenu\MainMenuScreen.cpp -o CMakeFiles\AwdClient.dir\game\graphics\mainmenu\MainMenuScreen.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.obj: ../game/graphics/mainmenu/MainMenuButton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\mainmenu\MainMenuButton.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\mainmenu\MainMenuButton.cpp

CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\mainmenu\MainMenuButton.cpp > CMakeFiles\AwdClient.dir\game\graphics\mainmenu\MainMenuButton.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\mainmenu\MainMenuButton.cpp -o CMakeFiles\AwdClient.dir\game\graphics\mainmenu\MainMenuButton.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.obj: ../game/graphics/FontManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\FontManager.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\FontManager.cpp

CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\FontManager.cpp > CMakeFiles\AwdClient.dir\game\graphics\FontManager.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\FontManager.cpp -o CMakeFiles\AwdClient.dir\game\graphics\FontManager.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.obj: ../game/graphics/ColorSet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\ColorSet.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\ColorSet.cpp

CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\ColorSet.cpp > CMakeFiles\AwdClient.dir\game\graphics\ColorSet.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\ColorSet.cpp -o CMakeFiles\AwdClient.dir\game\graphics\ColorSet.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.obj: ../game/graphics/RenderUtils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\RenderUtils.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\RenderUtils.cpp

CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\RenderUtils.cpp > CMakeFiles\AwdClient.dir\game\graphics\RenderUtils.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\RenderUtils.cpp -o CMakeFiles\AwdClient.dir\game\graphics\RenderUtils.cpp.s

CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.obj: CMakeFiles/AwdClient.dir/flags.make
CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.obj: CMakeFiles/AwdClient.dir/includes_CXX.rsp
CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.obj: ../game/graphics/WaterBackground.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.obj"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\AwdClient.dir\game\graphics\WaterBackground.cpp.obj -c E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\WaterBackground.cpp

CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.i"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\WaterBackground.cpp > CMakeFiles\AwdClient.dir\game\graphics\WaterBackground.cpp.i

CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.s"
	"E:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\CLionProjects\AsWeDrown\AwdClient\game\graphics\WaterBackground.cpp -o CMakeFiles\AwdClient.dir\game\graphics\WaterBackground.cpp.s

# Object files for target AwdClient
AwdClient_OBJECTS = \
"CMakeFiles/AwdClient.dir/main.cpp.obj" \
"CMakeFiles/AwdClient.dir/packets.pb.cc.obj" \
"CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.obj" \
"CMakeFiles/AwdClient.dir/net/PacketManager.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/Game.cpp.obj" \
"CMakeFiles/AwdClient.dir/net/UdpClient.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.obj" \
"CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.obj"

# External object files for target AwdClient
AwdClient_EXTERNAL_OBJECTS =

AwdClient.exe: CMakeFiles/AwdClient.dir/main.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/packets.pb.cc.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/net/PacketTransformer.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/net/PacketManager.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/Game.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/net/UdpClient.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/packetlistener/HandshakeResponseListener.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/packetlistener/CreateLobbyResponseListener.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/Drawable.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/Button.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuScreen.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/mainmenu/MainMenuButton.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/FontManager.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/ColorSet.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/RenderUtils.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/game/graphics/WaterBackground.cpp.obj
AwdClient.exe: CMakeFiles/AwdClient.dir/build.make
AwdClient.exe: E:/CLionProjects/libs/sfml/SFML-2.5.1-build/lib/libsfml-audio.a
AwdClient.exe: E:/CLionProjects/libs/sfml/SFML-2.5.1-build/lib/libsfml-graphics.a
AwdClient.exe: E:/CLionProjects/libs/sfml/SFML-2.5.1-build/lib/libsfml-network.a
AwdClient.exe: E:/CLionProjects/libs/sfml/SFML-2.5.1-build/lib/libsfml-window.a
AwdClient.exe: E:/Program\ Files/CMake/bin/libprotobuf.a
AwdClient.exe: E:/CLionProjects/libs/sfml/SFML-2.5.1-build/lib/libsfml-system.a
AwdClient.exe: CMakeFiles/AwdClient.dir/linklibs.rsp
AwdClient.exe: CMakeFiles/AwdClient.dir/objects1.rsp
AwdClient.exe: CMakeFiles/AwdClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Linking CXX executable AwdClient.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\AwdClient.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AwdClient.dir/build: AwdClient.exe

.PHONY : CMakeFiles/AwdClient.dir/build

CMakeFiles/AwdClient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\AwdClient.dir\cmake_clean.cmake
.PHONY : CMakeFiles/AwdClient.dir/clean

CMakeFiles/AwdClient.dir/depend: packets.pb.cc
CMakeFiles/AwdClient.dir/depend: packets.pb.h
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\CLionProjects\AsWeDrown\AwdClient E:\CLionProjects\AsWeDrown\AwdClient E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release E:\CLionProjects\AsWeDrown\AwdClient\cmake-build-release\CMakeFiles\AwdClient.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AwdClient.dir/depend

