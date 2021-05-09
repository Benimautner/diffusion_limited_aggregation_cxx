cmake_minimum_required(VERSION 3.3)
project(diffusion_limited_aggregation_c++)
set(APP_NAME diffusion_limited_aggregation_c++)

# ===================== oF Directory =====================
set(OF_DIRECTORY /home/beni/install/of_v0.11.2_linux64gcc6_release)


# ===================== Compiler Flags =====================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# =========== Set Global Properties ==============
include(addons)
include(linux64)

# ===================== Source Files =====================
set(SOURCE_FILES
        ${ADDON_SOURCE_FILES}
        libs/ThreadPool/ThreadPool.h
        src/main.cpp
        src/ofApp.cpp)


# ===================== System Frameworks =====================

# =============================================================

# ===================== Include Dirs =====================

# ========================================================


# ===================== Libs =====================

# ================================================

# ============ Compile and Link ==================

add_executable(${APP_NAME} ${SOURCE_FILES})
add_dependencies(${APP_NAME} of_shared)
target_link_libraries(${APP_NAME} $<TARGET_FILE:of_shared>)
target_link_libraries(${APP_NAME} ${OF_CORE_LIBS})


set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
