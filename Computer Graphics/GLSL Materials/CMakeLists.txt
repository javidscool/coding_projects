set(SOURCES
	BrdfHash.h
	BrdfHash.cc
	Brdf.h
	Brdf.cc
	Brdf.tcc
	BRDFRead.cpp
	)

s_lib(jcs.colorconverter ${SOURCES})
target_link_libraries(jcs.colorconverter se.math se.graphics.image.formats se.sim)

s_binary(jcs.brdf_hash GenerateHash.cc)
s_binary(jcs.color_image ProcessImage.cc)
target_link_libraries(jcs.brdf_hash jcs.colorconverter)
target_link_libraries(jcs.color_image jcs.colorconverter)
