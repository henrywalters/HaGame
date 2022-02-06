#include "BatchRenderer.h"

void hagame::graphics::BatchRenderer::clear()
{
	m_meshBuffers.clear();
}

void hagame::graphics::BatchRenderer::enqueue(BatchMesh mesh, Vec3 pos, Quat rot, Vec3 scale)
{
	auto id = hashMesh(mesh);

	if (m_meshBuffers.find(id) == m_meshBuffers.end()) {
		m_meshBuffers.insert(std::make_pair(id, std::make_shared<MeshBuffer>()));
		m_meshBuffers[id]->initializeBuffers(mesh.mesh);
	}

	m_meshBuffers[id]->insert(Mat4::Translation(pos), Mat4::Rotation(rot), Mat4::Scale(scale));

	insertIfMissing(m_shaders, id, mesh.shader);
	insertIfMissing(m_borderShaders, id, mesh.borderShader);
	insertIfMissing(m_materials, id, mesh.material);
	insertIfMissing(m_colors, id, mesh.color);
	insertIfMissing(m_borderColors, id, mesh.borderColor);
	insertIfMissing(m_textures, id, mesh.texture);
	insertIfMissing(m_normal, id, mesh.normal);
	insertIfMissing(m_opacity, id, mesh.opacity);
	insertIfMissing(m_specular, id, mesh.specular);
}

void hagame::graphics::BatchRenderer::draw()
{
}

uint64_t hagame::graphics::BatchRenderer::hashMesh(BatchMesh mesh)
{
	unsigned int ids[9] = {
		mesh.shader->id,
		mesh.borderShader == NULL ? 0 : mesh.borderShader->id,
		mesh.material.id,
		mesh.color.id,
		mesh.borderColor.id,
		mesh.texture->id,
		mesh.normal == NULL ? 0 : mesh.normal->id,
		mesh.opacity == NULL ? 0 : mesh.opacity->id,
		mesh.specular == NULL ? 0 : mesh.specular->id
	};

	return HashIds<unsigned int, 9>(ids);
}
