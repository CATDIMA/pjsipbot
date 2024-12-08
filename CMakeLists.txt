cmake_minimum_required(VERSION 3.15.0)
project(pjsipbot)

add_executable(${PROJECT_NAME} 
                main.cpp 
                auxiliary_functions.cpp
                inherited_classes.cpp)

INCLUDE(FindPkgConfig)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

pkg_check_modules(PJSIP REQUIRED libpjproject)
add_definitions(${PJSIP_CFLAGS})

#Build type (Debug or Release)
set(CMAKE_BUILD_TYPE Debug)

target_include_directories(${PROJECT_NAME}
        PUBLIC dep/pjsip/pjlib/include
        PUBLIC dep/pjsip/pjlib-util/include
        PUBLIC dep/pjsip/pjmedia/include
        PUBLIC dep/pjsip/pjnath/include
        PUBLIC dep/pjsip/pjsip/include)

target_link_directories(${PROJECT_NAME}
        PUBLIC dep/pjsip/pjlib/lib
        PUBLIC dep/pjsip/pjlib-util/lib
        PUBLIC dep/pjsip/pjmedia/lib
        PUBLIC dep/pjsip/pjnath/lib
        PUBLIC dep/pjsip/pjsip/lib
        PUBLIC dep/pjsip/third_party/lib)

target_link_libraries(${PROJECT_NAME} ${PJSIP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} uuid)
target_link_libraries(${PROJECT_NAME} speex-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} webrtc-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} gsmcodec-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} ilbccodec-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} resample-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} srtp-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} yuv-x86_64-unknown-linux-gnu)
target_link_libraries(${PROJECT_NAME} g7221codec-x86_64-unknown-linux-gnu)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY res/ DESTINATION bin/res)
install(FILES account_conf.txt DESTINATION bin)