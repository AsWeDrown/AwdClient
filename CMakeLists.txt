cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME AwdClient)
project(${PROJECT_NAME} VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SFML_DIR "${PROJECT_SOURCE_DIR}/lib/SFML-2.5.1-build")
find_package(
        SFML 2.5
        COMPONENTS
                audio
                graphics
                network
                system
                window
        REQUIRED
)

set(Protobuf_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/protobuf-3.15.8-build/include")
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS net/packets.proto)

add_executable(${PROJECT_NAME}
        main.cpp
        ${PROTO_SRCS} ${PROTO_HDRS}
        net/PacketTransformer.cpp net/PacketTransformer.hpp
        net/PacketManager.cpp net/PacketManager.hpp
        game/Game.cpp game/Game.hpp
        net/UdpClient.cpp net/UdpClient.hpp
        game/packetlistener/PacketListener.hpp
        game/packetlistener/HandshakeResponseListener.cpp game/packetlistener/HandshakeResponseListener.hpp
        game/packetlistener/CreateLobbyResponseListener.cpp game/packetlistener/CreateLobbyResponseListener.hpp game/screen/ScreenComponent.cpp game/screen/ScreenComponent.hpp game/screen/Button.cpp game/screen/Button.hpp game/screen/Screen.cpp game/screen/Screen.hpp game/screen/mainmenu/MainMenuScreen.cpp game/screen/mainmenu/MainMenuScreen.hpp game/screen/mainmenu/MainMenuButton.cpp game/screen/mainmenu/MainMenuButton.hpp game/screen/mainmenu/MainMenuBackground.cpp game/screen/mainmenu/MainMenuBackground.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")

target_link_libraries(${PROJECT_NAME}
        sfml-audio
        sfml-graphics
        sfml-network
        sfml-system
        sfml-window
        ${Protobuf_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
