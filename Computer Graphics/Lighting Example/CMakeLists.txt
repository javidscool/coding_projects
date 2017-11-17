
set(SOURCES
 aitMain.cpp )


s_lib(template ${SOURCES})

s_binary(runtemplate aitMain.cpp 
 
ScreenSpaceQuad.h ScreenSpaceQuad.tcc ScreenSpaceQuad.cpp
skybox.h skybox.cc
ShooterCamera.h
ShooterCamera.cc
transformations.cc
transformations.h

)

s_portable_binary_name(runtemplate "Template")

target_link_libraries(runtemplate se.graphics.model se.graphics.gl se.sim.physics se.graphics.image.formats)

