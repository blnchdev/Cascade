cmake_minimum_required(VERSION 4.4)
project(Cascade)
set(CMAKE_CXX_STANDARD 26)

add_executable(Cascade main.cpp
        Rules/Archives.hpp
        Rules/Coding.hpp
        Rules/Documents.hpp
        Rules/GitHubRepos.hpp
        Rules/Images.hpp
        Rules/Torrents.hpp
        Rules/Symbols.hpp
        Rules/DiskImages.hpp
        Rules/Fonts.hpp
        Rules/SoftwareInstallers.hpp
        Rules/Videos.hpp
        Rules/Audios.hpp
        Rules/Libraries.hpp
        Rules/EBooks.hpp)

target_link_libraries(Cascade PRIVATE stdc++exp)
target_link_options(Cascade PRIVATE -static-libgcc -static-libstdc++ -static)