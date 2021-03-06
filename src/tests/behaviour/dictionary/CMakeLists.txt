cmake_minimum_required(VERSION 3.5)
project(behaviour_dictionary)

set(SOURCE_FILES
		behaviour_dictionary.c
		behaviour_dictionary.h
		../../../dictionary/ion_master_table.c
		../../../dictionary/ion_master_table.h
		)

if(USE_ARDUINO)
	set(${PROJECT_NAME}_BOARD       ${BOARD})
	set(${PROJECT_NAME}_PROCESSOR   ${PROCESSOR})
	set(${PROJECT_NAME}_MANUAL      ${MANUAL})
	set(${PROJECT_NAME}_SRCS		${SOURCE_FILES})
	set(${PROJECT_NAME}_LIBS        planck_unit bpp_tree skip_list flat_file open_address_hash open_address_file_hash)

	generate_arduino_library(${PROJECT_NAME})
else()
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

	target_link_libraries(${PROJECT_NAME}   planck_unit bpp_tree skip_list flat_file open_address_hash open_address_file_hash)

	# Required on Unix OS family to be able to be linked into shared libraries.
	set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()