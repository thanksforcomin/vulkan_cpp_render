glslc depth.vert -o depth.spv
glslc fwdplus.frag -o frag.spv
glslc fwdplus.vert -o vert.spv
glslc -fshader-stage=comp light_culling.comp.glsl -o comp.spv