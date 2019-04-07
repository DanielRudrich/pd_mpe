# Define your standard CMake header (for example):
cmake_minimum_required(VERSION 2.8)

set(project_source_dir /Users/Daniel/Projekte/2019_pd_MPE)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

# Include pd.cmake:
include(${project_source_dir}/pd.build/pd.cmake)

# Declare the name of the project:
project(MPE)

# Define the path to the Pure Data sources:
set_pd_sources(/Users/Daniel/SDKs/pd-0.49-0/src)

# Set the output path for the externals:
set_pd_external_path(${project_source_dir}/bin/)

# Add one or several externals:
add_pd_external(MPE mpe ${project_source_dir}/Source/mpe.cpp)
