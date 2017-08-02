  macro(configure_msvc_runtime)
    if(MSVC)
      # Default to statically-linked runtime.
      if("${MSVC_RUNTIME}" STREQUAL "")
        set(MSVC_RUNTIME "static")
      endif()
      # Set compiler options.
      set(variables
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
      )
      if(${MSVC_RUNTIME} STREQUAL "static")
        message(STATUS
          "MSVC -> forcing use of statically-linked runtime."
        )
        foreach(variable ${variables})
          if(${variable} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
            set(${variable} "${${variable}} /wd4503")
            message(STATUS "${variable}=${${variable}}")
          endif()
        endforeach()
      else()
        message(STATUS
          "MSVC -> forcing use of dynamically-linked runtime."
        )
        foreach(variable ${variables})
          if(${variable} MATCHES "/MT")
            string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
            set(${variable} "${${variable}} /wd4503")
            message(STATUS "${variable}=${${variable}}")
          endif()
        endforeach()
      endif()
    endif()
  endmacro()

macro(group_files SRCS) 
foreach(source IN LISTS ${SRCS})
     get_filename_component(source_path "${source}" PATH)
     string(REPLACE "/" "\\" source_path_msvc "${source_path}")
     source_group("${source_path_msvc}" FILES "${source}")
     message(STATUS "${source_path_msvc}\t ${source}")
endforeach()
endmacro(group_files)