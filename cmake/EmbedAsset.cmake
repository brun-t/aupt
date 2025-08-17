function(EmbedAsset INPUT_FILE OUTPUT_HEADER VAR_NAME)
  if(NOT Python3_Interpreter_FOUND)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
  endif()

  get_filename_component(ABS_INPUT_FILE "${INPUT_FILE}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
  get_filename_component(ABS_OUTPUT_HEADER "${OUTPUT_HEADER}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
  set(GENERATOR "${CMAKE_SOURCE_DIR}/gen_files.py")

  add_custom_command(
    OUTPUT ${ABS_OUTPUT_HEADER}
    COMMAND ${CMAKE_COMMAND} -E echo "Embedding asset '${INPUT_FILE}' as '${VAR_NAME}'..."
    COMMAND ${Python3_EXECUTABLE} ${GENERATOR}
            ${ABS_INPUT_FILE}
            ${ABS_OUTPUT_HEADER}
            ${VAR_NAME}
    DEPENDS ${ABS_INPUT_FILE} ${GENERATOR}
    COMMENT "Generating ${OUTPUT_HEADER} from ${INPUT_FILE}"
    VERBATIM
  )

  # Create a named target for this specific embed (optional but convenient)
  get_filename_component(TGT_NAME "${VAR_NAME}" NAME)
  string(REPLACE "." "_" TGT_NAME "${TGT_NAME}")
  add_custom_target(embed_${TGT_NAME} DEPENDS ${ABS_OUTPUT_HEADER})

  # Return the generated header path and target name to caller (optional)
  set(${VAR_NAME}_HEADER ${ABS_OUTPUT_HEADER} PARENT_SCOPE)
  set(${VAR_NAME}_TARGET embed_${TGT_NAME} PARENT_SCOPE)
endfunction()
