file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

if(APPLE) # mac
	enable_language(OBJC)
	enable_langauge(OBJCXX)

	file(GLOB_RECURSE OBJC_SOURCE_FILES CONFIGURE_DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm
	)
else()
	set(OBJCPP_SOURCE_FILES "")
endif()

add_library(spruce STATIC ${SOURCE_FILES} ${OBJC_SOURCE_FILES})

set_target_properties(spruce PROPERTIES
	VERSION ${PROJECT_VERSION}
	OUTPUT_NAME ${PROJECT_NAME}
	LINKER_LANGUAGE CXX
)

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(pkg_config "${generated_dir}/${PROJECT_NAME}.pc")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${version_config}"
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY AnyNewerVersion
)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in" "${project_config}" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/spruce.pc.in" "${pkg_config}" @ONLY)

install(
	TARGETS spruce
	EXPORT ${targets_export_name}
	LIBRARY DESTINATION "lib"
	ARCHIVE DESTINATION "lib"
	INCLUDES DESTINATION "include"
)

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.h"
)

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
	DESTINATION "include/${PROJECT_NAME}"
	FILES_MATCHING PATTERN "*.h"
)

install(
	FILES "${project_config}" "${version_config}"
	DESTINATION "lib/cmake/${PROJECT_NAME}"
)

install(
	FILES "${pkg_config}"
	DESTINATION "lib/pkgconfig"
)

install(
	EXPORT "${targets_export_name}"
	NAMESPACE "${namespace}"
	DESTINATION "lib/cmake/${PROJECT_NAME}"
)

target_link_libraries(spruce PUBLIC
	freetype
	freeimage
	freeimageplus
)

if(APPLE) # mac
	target_include_directories(spruce PUBLIC
		/usr/local/include
	)
	target_link_directories(spruce PUBLIC
		/usr/local/lib
	)
	find_library(OPENGL_LIB OpenGL REQUIRED)
	find_library(METAL_LIB Metal REQUIRED)
	find_library(COCOA_LIB Cocoa REQUIRED)
	find_library(QUARTZ_LIB Quartz REQUIRED)
	target_link_options(spruce PUBLIC
		${OPENGL_LIB}
		${METAL_LIB}
		${COCOA_LIB}
		${QUARTZ_LIB}
	)
endif()

if(UNIX AND NOT APPLE) # linux
	find_package(X11 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(Vulkan REQUIRED)

	target_link_libraries(spruce PUBLIC
		X11::X11
		Vulkan::Vulkan
		OpenGL::GL
		OpenGL::GLX
		GLEW::GLEW
		pthread
	)
endif()

target_include_directories(spruce
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		$<INSTALL_INTERFACE:include/spruce>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(spruce PUBLIC
	cxx_std_20
)

set_source_files_properties(${OBJC_SOURCE_FILES} PROPERTIES COMPILE_FLAGS -Wno-old-style-cast)
set_source_files_properties(../core/src/graphics/Font.cpp PROPERTIES COMPILE_FLAGS -Wno-old-style-cast)

target_compile_options(spruce
	PUBLIC
		-mavx2
		-mfma
		-fno-strict-aliasing
		-fno-exceptions
		-fno-rtti
		-mtune=znver2
	PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		-Wnon-virtual-dtor
		-Wold-style-cast
		-Wcast-align
		-Wunused
		-Woverloaded-virtual
		-Wconversion
		-Wsign-conversion
		-Wdouble-promotion
		-Wformat=2
		-Wno-deprecated-declarations
		-Wno-attributes
		-Wno-unknown-pragmas
		$<$<CONFIG:Debug>:-Og>
		$<$<CONFIG:Release>:-O3>
)


target_compile_definitions(spruce PUBLIC
	$<$<CONFIG:DEBUG>:DEBUG>
	GL_SILENCE_DEPRECATION
)

add_subdirectory(test)
add_subdirectory(bench)
