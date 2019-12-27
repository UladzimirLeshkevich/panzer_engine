cmake_minimum_required(VERSION 3.12)
project(panzer_engine)

add_executable(${PROJECT_NAME} game.cxx
                               engine.cxx
                               bullet.cxx
                               bullet.hxx
							   log.h
                               log.cpp
                               decorator.cpp
                               decorator.h
                               my_log.cpp
                               my_log.h
                               log_manager.cpp
                               log_manager.h
                               )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#add_subdirectory(logger_lib)

if (MINGW)
    find_library(SDL2_LIB NAMES libSDL2)
    find_library(SDL2_MIXER NAMES SDL2_mixer.dll)
    find_library(SDL2_IMAGE NAMES SDL2_image.dll)

    if(NOT SDL2_LIB)
        message(FATAL_ERROR "Error: find_library(...) did not find libSDL2")
    endif()

    if(NOT SDL2_MIXER)
        message(FATAL_ERROR "Error: find_library(...) did not find SDL2_mixer.dll")
    endif()

    if(NOT SDL2_IMAGE)
        message(FATAL_ERROR "Error: find_library(...) did not find SDL2_image.dll")
    endif()

    # find out what libraries are needed for staticaly linking with libSDL.a
    # using mingw64 cross-compiler

    #$> $ /usr/x86_64-w64-mingw32/sys-root/mingw/bin/sdl2-config --static-libs
    #-L/usr/x86_64-w64-mingw32/sys-root/mingw/lib -lmingw32 -lSDL2main
    #-lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32
    #-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid
    #-static-libgcc

    target_link_libraries(${PROJECT_NAME}
               #SDL2::SDL2-static
               #SDL2::SDL2main
               -lmingw32
               -lSDL2main
               -lSDL2
               ${SDL2_LIB}
               ${SDL2_MIXER}
               ${SDL2_IMAGE}
               -mwindows
               -Wl,--no-undefined
               -lm
               -ldinput8
               -ldxguid
               -ldxerr8
               -luser32
               -lgdi32
               -lwinmm
               -limm32
               -lole32
               -loleaut32
               -lshell32
               -lversion
               -luuid
               -static-libgcc
               -lopengl32
               )
elseif(UNIX)

    find_library(SDL2_LIB_ NAMES libSDL2.a)

    if(NOT SDL2_LIB_)
        message(FATAL_ERROR "Error: find_library(...) did not find libSDL2")
    endif()

    target_link_libraries(${PROJECT_NAME}
               ${SDL2_LIB_}
               -lSDL2
               -lSDL2_mixer
               -lSDL2_image
               -lGL

               -lm
               -ldl
               -lpthread
               -lrt
               -libmf1_logger_lib.dll
               )
endif()


#target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libmf1_logger_lib.dll)

file(GLOB resources "res/*")
file(COPY ${resources} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)









