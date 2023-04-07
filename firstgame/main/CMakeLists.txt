add_executable(main
        ../game/game_layer.cpp
        ../game/random.cpp
        ../game/level.cpp
        ../game/player.cpp
        ../game/particle_system.cpp
        main.cpp)
target_include_directories(main PRIVATE ${THIRD_PARTY_DIRECTORY}/chernoengine2)
target_link_libraries(main PRIVATE ${THIRD_PARTY_DIRECTORY}/chernoengine2/lib/libchernoengine2.a)

# dependencies
find_package(spdlog)
target_include_directories(main PRIVATE ${THIRD_PARTY_DIRECTORY}/chernoengine2/third_party)
target_link_libraries(main PRIVATE spdlog::spdlog glfw)
