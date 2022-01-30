#include "MeshBuffer.h"

hagame::graphics::MeshBuffer::MeshBuffer()
{
}

void hagame::graphics::MeshBuffer::initializeBuffers(Mesh* mesh)
{
	m_meshBuffer = VertexBuffer<hagame::graphics::MeshTransform>::Dynamic(MAX_MESH_COUNT);
	m_mesh = mesh;
	auto vao = m_mesh->getVAO();
	m_meshBuffer->bind();
	vao->bind();
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 3, 4, sizeof(Vec4) * 0);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 4, 4, sizeof(Vec4) * 1);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 5, 4, sizeof(Vec4) * 2);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 6, 4, sizeof(Vec4) * 3);

	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 7, 4, sizeof(Vec4) * 0);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 8, 4, sizeof(Vec4) * 1);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 9, 4, sizeof(Vec4) * 2);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 10, 4, sizeof(Vec4) * 3);

	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 11, 4, sizeof(Vec4) * 0);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 12, 4, sizeof(Vec4) * 1);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 13, 4, sizeof(Vec4) * 2);
	vao->defineAttribute(m_meshBuffer.get(), DataType::Float, 14, 4, sizeof(Vec4) * 3);

	vao->setInstanced(3, 14);
}

void hagame::graphics::MeshBuffer::clear()
{
	m_meshBuffer->clear();
	m_meshCount = 0;
}

int hagame::graphics::MeshBuffer::insert(Mat4 pos, Mat4 rot, Mat4 scale)
{
	assert(getLineCount() < MAX_LINE_COUNT);
	int index = getMeshCount();
	m_meshBuffer->update(m_meshCount++, hagame::graphics::MeshTransform{ pos.transpose(), rot.transpose(), scale.transpose() });
	return index;
}

void hagame::graphics::MeshBuffer::update(int index, Mat4 pos, Mat4 rot, Mat4 scale)
{
	assert(index < getLineCount());
	m_meshBuffer->update(index, hagame::graphics::MeshTransform{ pos.transpose(), rot.transpose(), scale.transpose() });
}

hagame::graphics::MeshTransform hagame::graphics::MeshBuffer::get(int index)
{
	assert(index < getLineCount());
	return m_meshBuffer->read(index);
}

void hagame::graphics::MeshBuffer::draw(bool clearBuffer)
{
	m_mesh->getVAO()->bind();
	m_meshBuffer->bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, m_mesh->vertices.size(), getMeshCount());
	if (clearBuffer) {
		clear();
	}
}
