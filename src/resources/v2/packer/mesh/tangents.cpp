#include "tangents.h"

#include "mesh.h"
#include "mikktspace.h"

basic_vertex& getVertex(mesh_ir& m, i32 face, i32 vert)
{
    return m.vertices[m.indices[face * 3 + vert]];
}

i32 getNumFaces(const SMikkTSpaceContext* ctx)
{
    mesh_ir& m = *static_cast<mesh_ir*>(ctx->m_pUserData);

    return i32(m.indices.size()) / 3;
}

i32 getFaceNumVertex(const SMikkTSpaceContext* ctx, const i32 face)
{
    return 3;
}

void getVertexPos(const SMikkTSpaceContext* ctx, f32 posOut[], const i32 face, const i32 vert)
{
    mesh_ir& m = *static_cast<mesh_ir*>(ctx->m_pUserData);

    auto& v = getVertex(m, face, vert);
    posOut[0] = v.x;
    posOut[1] = v.y;
    posOut[2] = v.z;

}

void getVertexNorm(const SMikkTSpaceContext* ctx, f32 normOut[], const i32 face, const i32 vert)
{
    mesh_ir& m = *static_cast<mesh_ir*>(ctx->m_pUserData);

    auto& v = getVertex(m, face, vert);
    normOut[0] = v.nx;
    normOut[1] = v.ny;
    normOut[2] = v.nz;
}

void getVertexUV(const SMikkTSpaceContext* ctx, f32 uvOut[], const i32 face, const i32 vert)
{
    mesh_ir& m = *static_cast<mesh_ir*>(ctx->m_pUserData);

    auto& v = getVertex(m, face, vert);
    uvOut[0] = v.u;
    uvOut[1] = v.v;
}

void setVertexTan(const SMikkTSpaceContext* ctx, const f32 tan[], const f32 sign, const i32 face, const i32 vert)
{
    mesh_ir& m = *static_cast<mesh_ir*>(ctx->m_pUserData);

    auto& v = getVertex(m, face, vert);
    v.tx = tan[0];
    v.ty = tan[1];
    v.tz = tan[2];
    v.tw = tan[3];
}

SMikkTSpaceInterface impl{
    .m_getNumFaces = getNumFaces,
    .m_getNumVerticesOfFace = getFaceNumVertex,

    .m_getPosition = getVertexPos,
    .m_getNormal = getVertexNorm,
    .m_getTexCoord = getVertexUV,

    .m_setTSpaceBasic = setVertexTan
};

void calculate_tangents(mesh_ir& m)
{
    SMikkTSpaceContext ctx{
        &impl,
        &m
    };

    genTangSpaceDefault(&ctx);
}
