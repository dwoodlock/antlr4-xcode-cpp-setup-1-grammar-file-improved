
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Find the installed Node.js version using the node executable
find_program(NODE_EXECUTABLE node)
if (NOT NODE_EXECUTABLE)
    message(FATAL_ERROR "node executable not found.")
endif()

find_program(NPM_EXECUTABLE npm)
if (NOT NPM_EXECUTABLE)
    message(FATAL_ERROR "npm not found.")
endif()

# Get the installed Node.js version
execute_process(
    COMMAND ${NODE_EXECUTABLE} --version
    OUTPUT_VARIABLE NODE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(NODE_HEADERS_URL "https://nodejs.org/dist/${NODE_VERSION}/node-${NODE_VERSION}-headers.tar.gz")

set(NODE_HEADERS_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/node-headers")
set(NODE_HEADERS_DIR "${NODE_HEADERS_ROOT_DIR}/node-${NODE_VERSION}/include")

set(NODE_HEADERS_TARBALL "${CMAKE_CURRENT_BINARY_DIR}/node-headers-tar/${NODE_VERSION}/node-${NODE_VERSION}-headers.tar.gz")

if(NOT EXISTS ${NODE_HEADERS_DIR})
    message(STATUS "Node.js headers not found for ${NODE_VERSION}, downloading...")
    
    # Download the Node.js headers if not already downloaded
    if(NOT EXISTS ${NODE_HEADERS_TARBALL})
        message(STATUS "Downloading Node.js headers from ${NODE_HEADERS_URL}")
        file(DOWNLOAD ${NODE_HEADERS_URL} ${NODE_HEADERS_TARBALL} SHOW_PROGRESS)
    else()
        message(STATUS "Using cached Node.js headers tarball.")
    endif()

    # Extract the headers
    message(STATUS "Extracting Node.js headers...")
    file(MAKE_DIRECTORY ${NODE_HEADERS_DIR})
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf  ${NODE_HEADERS_TARBALL}
        WORKING_DIRECTORY ${NODE_HEADERS_ROOT_DIR}
    )
else()
    message(STATUS "Node.js (${NODE_VERSION}) headers already downloaded and extracted.")
endif()

execute_process(
    COMMAND npm install node-addon-api --no-save
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE NPM_INSTALL_STATUS
    OUTPUT_VARIABLE NPM_OUTPUT
    ERROR_VARIABLE NPM_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT ${NPM_INSTALL_STATUS}  EQUAL 0)
    message(FATAL_ERROR "npm install failed with status ${NPM_INSTALL_STATUS}\nOutput:\n${NPM_OUTPUT}\nError:\n${NPM_ERROR}")
endif()

execute_process(
    COMMAND ${NODE_EXECUTABLE} -e "console.log(require('node-addon-api').include_dir)"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE NODE_API_HEADER_STATUS
    OUTPUT_VARIABLE NODE_API_HEADER_DIR
    ERROR_VARIABLE NPM_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT ${NODE_API_HEADER_STATUS}  EQUAL 0)
    message(FATAL_ERROR "Failed to get header path for node-api-headers ${NODE_API_HEADER_STATUS}\nOutput:\n${NODE_API_HEADER_DIR}\nError:\n${NPM_ERROR}")
endif()

add_library(node-addon SHARED hello.cc)
target_include_directories(
    node-addon PRIVATE 
    ${NODE_HEADERS_DIR}/node
    ${NODE_HEADERS_DIR}/src
    ${NODE_HEADERS_DIR}/deps/openssl/config
    ${NODE_HEADERS_DIR}/deps/openssl/openssl/include
    ${NODE_HEADERS_DIR}/deps/uv/include
    ${NODE_HEADERS_DIR}/deps/zlib
    ${NODE_HEADERS_DIR}/deps/v8/include
    ${CMAKE_CURRENT_BINARY_DIR}/${NODE_API_HEADER_DIR}
)

target_compile_definitions(node-addon PRIVATE
    USING_UV_SHARED=1
    USING_V8_SHARED=1
    V8_DEPRECATION_WARNINGS=1
    V8_DEPRECATION_WARNINGS
    V8_IMMINENT_DEPRECATION_WARNINGS
    _GLIBCXX_USE_CXX11_ABI=1
    _DARWIN_USE_64_BIT_INODE=1
    _LARGEFILE_SOURCE
    _FILE_OFFSET_BITS=64
    OPENSSL_NO_PINSHARED
    OPENSSL_THREADS
    NAPI_DISABLE_CPP_EXCEPTIONS
    BUILDING_NODE_EXTENSION
    "$<$<CONFIG:DEBUG>:V8_ENABLE_CHECKS>"
)

target_link_libraries(node-addon antlr-cpp-static)
target_link_options(node-addon PRIVATE -undefined dynamic_lookup)

set_target_properties(node-addon PROPERTIES PREFIX "" SUFFIX ".node"
                                            XCODE_SCHEME_EXECUTABLE ${NODE_EXECUTABLE}
                                            XCODE_SCHEME_ARGUMENTS ${CMAKE_CURRENT_LIST_DIR}/hello.js
                                            XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
