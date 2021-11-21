
if(NOT "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-subbuild/actor_framework-populate-prefix/src/actor_framework-populate-stamp/actor_framework-populate-gitinfo.txt" IS_NEWER_THAN "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-subbuild/actor_framework-populate-prefix/src/actor_framework-populate-stamp/actor_framework-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-subbuild/actor_framework-populate-prefix/src/actor_framework-populate-stamp/actor_framework-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/actor-framework/actor-framework.git" "actor_framework-src"
    WORKING_DIRECTORY "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/actor-framework/actor-framework.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 1b29bf10 --
  WORKING_DIRECTORY "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '1b29bf10'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-subbuild/actor_framework-populate-prefix/src/actor_framework-populate-stamp/actor_framework-populate-gitinfo.txt"
    "/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-subbuild/actor_framework-populate-prefix/src/actor_framework-populate-stamp/actor_framework-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/tobi/VSP/vslab-caf/cmake-build-debug/_deps/actor_framework-subbuild/actor_framework-populate-prefix/src/actor_framework-populate-stamp/actor_framework-populate-gitclone-lastrun.txt'")
endif()

