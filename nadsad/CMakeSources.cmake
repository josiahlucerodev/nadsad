set(HEADER_FILES
nadsad/ascii/deserializer.h
nadsad/ascii/lexer.h
nadsad/ascii/serializer.h
nadsad/binary/unalignedDeserializer.h
nadsad/binary/unalignedSerializer.h
nadsad/nadsad.h
)

set(SOURCE_FILES
nadsad/nadsad.cpp
)

foreach(target_header IN LISTS HEADER_FILES)
	target_sources(nadsad 
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target_header}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/nadsad/${target_header}>)
endforeach()

foreach(target_source IN LISTS SOURCE_FILES)
	target_sources(nadsad 
		PRIVATE
			$<BUILD_INTERFACE:${target_source}>
	)
endforeach()

target_precompile_headers(nadsad PUBLIC nadsad/pch.h)