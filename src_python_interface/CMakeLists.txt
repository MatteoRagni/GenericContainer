
cmake_minimum_required(VERSION 3.10.2)

project(GenericContainer VERSION 1.0)

find_package(pybind11 REQUIRED)

find_package(pybind11)

	set(GENERIC_CONTAINER "${CMAKE_CURRENT_SOURCE_DIR}/../src")

	set(SOURCES
		"${GENERIC_CONTAINER}/GenericContainer.cc"
    "${GENERIC_CONTAINER}/GenericContainerCinterface.cc"
    "${GENERIC_CONTAINER}/GenericContainerSupport.cc"
    "${GENERIC_CONTAINER}/GenericContainerTables.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/python_generic_container.cc")

pybind11_add_module(${PROJECT_NAME} MODULE ${SOURCES} NO_EXTRAS)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

target_include_directories(${PROJECT_NAME}
	PUBLIC "${GENERIC_CONTAINER}")

set_property(TARGET ${PROJECT_NAME} PROPERTY
		POSITION_INDEPENDENT_CODE 1)