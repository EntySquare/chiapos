# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(relic-populate NONE)

include(ExternalProject)
ExternalProject_Add(relic-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://github.com/relic-toolkit/relic.git" "GIT_TAG" "1885ae3b681c423c72b65ce1fe70910142cf941c"
                    SOURCE_DIR          "/Users/luo/Documents/chia/chiapos/lib/bls-signatures/cmake-build-debug/_deps/relic-src"
                    BINARY_DIR          "/Users/luo/Documents/chia/chiapos/lib/bls-signatures/cmake-build-debug/_deps/relic-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


