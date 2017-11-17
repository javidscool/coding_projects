get_filename_component(PSELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(EXISTS ${PSELF_DIR}/config/SoaperMacros.cmake)
	include(${PSELF_DIR}/config/SoaperMacros.cmake)
endif(EXISTS ${PSELF_DIR}/config/SoaperMacros.cmake)

include(${PSELF_DIR}/template_project/template_project.cmake)
include_directories(${PSELF_DIR}/include)

