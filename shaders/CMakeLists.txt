project(shaders)

file(GLOB_RECURSE GLSL *.glsl)
file(GLOB_RECURSE VERT *.vert *.vertex *.vs)
file(GLOB_RECURSE TCS *.tcs *.cont *.t_cont)
file(GLOB_RECURSE TES *.tes *.eval *.t_eval)
file(GLOB_RECURSE GEOM *.geom *.geo *.geometry *.gs)
file(GLOB_RECURSE FRAG *.frag *.fragment *.fs *.ps)
file(GLOB_RECURSE COMP *.comp *.compute *.cs)

SOURCE_GROUP(VertexShaders FILES ${VERT})
SOURCE_GROUP(TessellationControlShaders FILES ${TCS})
SOURCE_GROUP(TessellationEvaluationShaders FILES ${TES})
SOURCE_GROUP(GeometryShaders FILES ${GEOM})
SOURCE_GROUP(FragmentShaders FILES ${FRAG})
SOURCE_GROUP(ComputeShaders FILES ${COMP})

add_custom_target(SHADERS SOURCES ${GLSL} ${VERT} ${TCS} ${TES} ${GEOM} ${FRAG} ${COMP})