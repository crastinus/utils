
#TODO: find in code
function(FindFileIntoProperties fileName propertyName result)
    set(fileName ${${fileName}})
    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY ${propertyName})
    set(${result} 0 PARENT_SCOPE)

    foreach(p ${dirs})
        file(GLOB FileList ${p}/*)
        foreach(f in ${FileList})
            get_filename_component(cleanedFileName ${f} NAME)
            if ("${cleanedFileName}" STREQUAL "${fileName}")
                set(${result} 1 PARENT_SCOPE)
                return()
            endif()
        endforeach()
    endforeach()
endfunction()

function(FindLibraryIntoProperties fileName result)

    set(fileName ${${fileName}})

    #if have no filename then file must not be existed
    if ("${fileName}" STREQUAL "")
        set(${result} 1 PARENT_SCOPE)
        return()
    endif()

    if (WINDOWS)
        # TODO: Make it for all possible compilers (Cygwin, MINGW)
        # windows has just one available extension
        list(APPEND Extensions "lib")
    elseif(LINUX)
        list(APPEND Extensions "so")
        list(APPEND Extensions "a")
    endif()

    foreach(ex ${Extensions})
        set(FileNameToFound "${fileName}.${ex}")
        FindFileIntoProperties(fileName "LINK_DIRECTORIES" result)
        if (${result} EQUAL 1)
            set(${result} 1 PARENT_SCOPE)
            return()
        endif()
    endforeach()

endfunction()

#  income list of strings
#  "IncludeFileName|DefineVariable or IncludeFileName|LibFileName|DefineVariable
#  if IncludeFileName present in INCLUDE_DIRECTORIES for a first case then
# DefineVariable add to CMAKE_CXX_FLAGS as define and add into cmake as variable
#  for a second case LibFileName must be present in LIBRARY_DIRECTORIES.
macro(DefineForFiles ListOfAtoms) 


    foreach(atom ${${ListOfAtoms}})
        string(REPLACE "|" ";" elements ${atom})

        list(LENGTH elements len)
        if (${len} EQUAL 2)
            list(GET elements 0 IncludeFileName)
            list(GET elements 1 DefineVariableName)
        elseif(${len} EQUAL 3)
            list(GET elements 0 IncludeFileName)
            list(GET elements 1 LibraryFileName)
            list(GET elements 2 DefineVariableName)
        else()
            message(FATAL "  Include/Library atom must be contains 2 or 3 elements. (${elements})")
        endif()

        FindFileIntoProperties(IncludeFileName "INCLUDE_DIRECTORIES" HaveInclude)
        #FindFileIntoProperties(LibraryFileName "LINK_DIRECTORIES" HaveLibrary)
        FindLibraryIntoProperties(LibraryFileName  HaveLibrary)

        if ((${HaveInclude} EQUAL 1) AND (${HaveLibrary} EQUAL 1) )
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${DefineVariableName}")
            set(${DefineVariableName} 1)
            message(STATUS "Define variable and preprocessor definissions ${DefineVariableName}")
        endif()
                
    endforeach()
endmacro()
