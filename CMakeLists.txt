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

#find_package(yaml-cpp REQUIRED)
#include_directories(${YAML_CPP_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        main.cpp
        ${PROTO_SRCS} ${PROTO_HDRS}
        net/PacketTransformer.cpp net/PacketTransformer.hpp net/PacketManager.cpp net/PacketManager.hpp game/Game.cpp game/Game.hpp net/UdpClient.cpp net/UdpClient.hpp game/packetlistener/PacketListener.hpp game/packetlistener/auth/HandshakeResponseListener.cpp game/packetlistener/auth/HandshakeResponseListener.hpp game/packetlistener/lobby/CreateLobbyResponseListener.cpp game/packetlistener/lobby/CreateLobbyResponseListener.hpp game/graphics/common/Drawable.cpp game/graphics/common/Drawable.hpp game/graphics/common/Button.cpp game/graphics/common/Button.hpp game/graphics/mainmenu/MainMenuScreen.cpp game/graphics/mainmenu/MainMenuScreen.hpp game/graphics/mainmenu/MainMenuButton.cpp game/graphics/mainmenu/MainMenuButton.hpp game/GameState.hpp game/graphics/data/FontManager.cpp game/graphics/data/FontManager.hpp game/graphics/common/ColorSet.cpp game/graphics/common/ColorSet.hpp game/util/RenderUtils.cpp game/util/RenderUtils.hpp game/graphics/common/WaterBackground.cpp game/graphics/common/WaterBackground.hpp game/graphics/common/Dialog.cpp game/graphics/common/Dialog.hpp game/graphics/common/TextInputDialog.cpp game/graphics/common/TextInputDialog.hpp game/graphics/common/DialogListener.hpp game/graphics/mainmenu/MainMenuScreenListener.cpp game/graphics/mainmenu/MainMenuScreenListener.hpp game/graphics/common/ButtonListener.hpp game/graphics/common/Identifiers.hpp game/graphics/common/UniformGridBuilder.cpp game/graphics/common/UniformGridBuilder.hpp game/graphics/common/RectWrapper.hpp game/graphics/common/TextFieldListener.hpp game/graphics/common/TextField.cpp game/graphics/common/TextField.hpp game/FatalErrors.hpp game/graphics/common/RichText.cpp game/graphics/common/RichText.hpp game/util/StringUtils.cpp game/util/StringUtils.hpp net/PacketContainer.cpp net/PacketContainer.hpp net/NetworkHandle.cpp net/NetworkHandle.hpp net/SequenceNumberMath.cpp net/SequenceNumberMath.hpp net/NetworkService.cpp net/NetworkService.hpp game/util/TimeUtils.cpp game/util/TimeUtils.hpp game/packetlistener/auth/PingListener.cpp game/packetlistener/auth/PingListener.hpp game/graphics/common/ErrorDialog.cpp game/graphics/common/ErrorDialog.hpp game/graphics/common/Screen.cpp game/graphics/common/Screen.hpp game/graphics/lobby/LobbyScreen.cpp game/graphics/lobby/LobbyScreen.hpp game/graphics/lobby/LobbyScreenListener.cpp game/graphics/lobby/LobbyScreenListener.hpp game/graphics/common/LoadingOverlay.cpp game/graphics/common/LoadingOverlay.hpp game/lobby/Lobby.hpp game/graphics/common/TextButton.cpp game/graphics/common/TextButton.hpp game/graphics/lobby/PlayerCard.cpp game/graphics/lobby/PlayerCard.hpp game/graphics/data/TextureManager.cpp game/graphics/data/TextureManager.hpp game/packetlistener/lobby/LeaveLobbyResponseListener.cpp game/packetlistener/lobby/LeaveLobbyResponseListener.hpp game/packetlistener/lobby/JoinLobbyResponseListener.cpp game/packetlistener/lobby/JoinLobbyResponseListener.hpp game/packetlistener/lobby/UpdatedMembersListListener.cpp game/packetlistener/lobby/UpdatedMembersListListener.hpp game/packetlistener/lobby/KickedFromLobbyListener.cpp game/packetlistener/lobby/KickedFromLobbyListener.hpp game/graphics/play/PlayScreen.cpp game/graphics/play/PlayScreen.hpp game/graphics/play/PlayScreenListener.cpp game/graphics/play/PlayScreenListener.hpp game/entity/Entity.cpp game/entity/Entity.hpp game/entity/LivingEntity.cpp game/entity/LivingEntity.hpp game/entity/EntityPlayer.cpp game/entity/EntityPlayer.hpp game/world/World.cpp game/world/World.hpp game/world/WorldLoader.cpp game/world/WorldLoader.hpp game/world/WorldData.hpp game/util/ImageUtils.cpp game/util/ImageUtils.hpp game/world/TileData.cpp game/world/TileData.hpp game/packetlistener/play/SpawnEntityListener.cpp game/packetlistener/play/SpawnEntityListener.hpp game/packetlistener/play/UpdateEntityPositionListener.cpp game/packetlistener/play/UpdateEntityPositionListener.hpp game/packetlistener/lobby/BeginPlayStateResponseListener.cpp game/packetlistener/lobby/BeginPlayStateResponseListener.hpp game/packetlistener/play/UpdateDimensionCommandListener.cpp game/packetlistener/play/UpdateDimensionCommandListener.hpp game/packetlistener/play/JoinWorldCommandListener.cpp game/packetlistener/play/JoinWorldCommandListener.hpp game/packetlistener/play/DespawnEntityListener.cpp game/packetlistener/play/DespawnEntityListener.hpp game/profiling/TpsMeter.cpp game/profiling/TpsMeter.hpp game/profiling/FpsMeter.cpp game/profiling/FpsMeter.hpp game/entity/EntityStateSnapshot.hpp game/config/PhysicsConfig.cpp game/config/PhysicsConfig.hpp game/config/ConfigManager.cpp game/config/ConfigManager.hpp game/config/DumbYamlReader.cpp game/config/DumbYamlReader.hpp game/config/GameConfig.cpp game/config/GameConfig.hpp game/util/CrashReporter.cpp game/util/CrashReporter.hpp game/entity/Entities.hpp game/util/MathUtils.cpp game/util/MathUtils.hpp game/world/TileBlock.cpp game/world/TileBlock.hpp game/world/BoundingBox.cpp game/world/BoundingBox.hpp game/world/tile/TileHandler.hpp game/world/tile/VoidHandler.cpp game/world/tile/VoidHandler.hpp game/world/tile/SolidHandler.cpp game/world/tile/SolidHandler.hpp game/world/Collidable.hpp game/world/TerrainControls.cpp game/world/TerrainControls.hpp game/entity/Fallable.cpp game/entity/Fallable.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")

target_link_libraries(${PROJECT_NAME}
        sfml-audio
        sfml-graphics
        sfml-network
        sfml-system
        sfml-window
        ${Protobuf_LIBRARIES}
        #${YAML_CPP_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
