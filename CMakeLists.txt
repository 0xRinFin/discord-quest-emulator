cmake_minimum_required(VERSION 4.1)
project(discordQuestSpoofer)

set(CMAKE_CXX_STANDARD 23)

find_package(ixwebsocket CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(discordQuestSpoofer
        main.cpp
        Window.cpp
        Window.h
        discord.cpp
        discord.h
        websocket.cpp
        websocket.h
        Quests.cpp
        Quests.h
)

target_include_directories(discordQuestSpoofer PUBLIC includes)
target_link_libraries(discordQuestSpoofer PRIVATE
        ixwebsocket::ixwebsocket
        nlohmann_json::nlohmann_json
        "C:/Users/Rin/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic/lib/libmbedcrypto.a"
        "C:/Users/Rin/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic/lib/libmbedtls.a"
        "C:/Users/Rin/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic/lib/libmbedx509.a"
        winhttp
        -lbcrypt
)