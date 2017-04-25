

if (METASTUFF_INCLUDE)
    include_directories(${METASTUFF_INCLUDE})
endif()

if (RAPIDJSON_INCLUDE AND METASTUFF_INCLUDE) 
    set(GENERATORS_SRC ${GENERATORS_SRC}
            json/rj_writing_visitor.cpp
            json/rj_writing_visitor.hpp
            json/rj_reading_visitor.hpp
            json/rj_reading_visitor.cpp)
    include_directories(${RAPIDJSON_INCLUDE})
 endif()


 
add_library(generators generators.cpp ${GENERATORS_SRC})
