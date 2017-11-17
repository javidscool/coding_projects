set(SOURCES
AttributeArray.cc AttributeArray.tcc AttributeArray.h
Mesh.h Mesh.tcc Mesh.cc
ScreenSpaceQuad.h ScreenSpaceQuad.tcc ScreenSpaceQuad.cpp
ShooterCamera.h
ShooterCamera.cc
transformations.cc
transformations.h
RayTracer.cc RayTracer.h
Object.h Object.cc
MyRay.h MyRay.cc
Misc.h
AABBox.h AABBox.cpp
BVH.h BVH.cpp
)

set(LIGHTMAPGEN_SOURCES
    lmgenMain.cpp
    Lightmap.h Lightmap.cc
    )

set(POINTTRACER_SOURCES
    pointMain.cpp
    PointTracer.h PointTracer.cc
    PointObject.h PointObject.cc
    )

s_lib(tracer ${SOURCES})
target_link_libraries(tracer se.graphics.gl se.graphics.image st.image.pano se.graphics.image.formats)

s_binary(runtracer aitMain.cpp)
s_portable_binary_name(runtracer "RayTracer")
target_link_libraries(runtracer se.sim.physics tracer)

s_binary(runlightmapgen ${LIGHTMAPGEN_SOURCES})
s_portable_binary_name(runlightmapgen "LightMapGen")
target_link_libraries(runlightmapgen se.sim.physics tracer)

s_binary(runpointtracer ${POINTTRACER_SOURCES})
s_portable_binary_name(runpointtracer "PointTracer")
target_link_libraries(runpointtracer se.sim.physics tracer)

