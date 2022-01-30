#include "LineBuffer.h"

void hagame::graphics::LineBuffer::initializeBuffers()
{
	m_lineBuffer = VertexBuffer<BatchLine>::Dynamic(MAX_LINE_COUNT);
	m_cube = std::make_shared<RectPrism>(Vec3(1.0f));
	auto vao = m_cube->getMesh()->getVAO();
	m_lineBuffer->bind();
	vao->bind();
	vao->defineAttribute(m_lineBuffer.get(), DataType::Float, 1, 4, offsetof(BatchLine, transform) + 0);
	vao->defineAttribute(m_lineBuffer.get(), DataType::Float, 2, 4, offsetof(BatchLine, transform) + sizeof(Vec4) * 1);
	vao->defineAttribute(m_lineBuffer.get(), DataType::Float, 3, 4, offsetof(BatchLine, transform) + sizeof(Vec4) * 2);
	vao->defineAttribute(m_lineBuffer.get(), DataType::Float, 4, 4, offsetof(BatchLine, transform) + sizeof(Vec4) * 3);
	vao->defineAttribute(m_lineBuffer.get(), DataType::Float, 5, 4, offsetof(BatchLine, color));
	vao->setInstanced(1, 5);
};

void hagame::graphics::LineBuffer::clear()
{
	m_lineBuffer->clear();
	m_lineCount = 0;
};

void hagame::graphics::LineBuffer::insert(math::Line line, Color color, float thickness)
{
	assert(getLineCount() < MAX_LINE_COUNT);
	BatchLine batchLine;

	Mat4 trans = Mat4::Translation(line.center());
	auto delta = line.b - line.a;
	Quat rotY = Quat(atan2(delta.y(), delta.x()), Vec3::Face());
	Quat rotZ = Quat(atan2(delta.z(), delta.x()), Vec3::Top());
	Mat4 rot = Mat4::Rotation(rotY * rotZ);
	Mat4 scale1 = Mat4::Scale(Vec3(line.magnitude(), thickness, thickness));
	Mat4 scale2 = Mat4::Scale(Vec3(1.0f, thickness, thickness));

	batchLine.transform = (trans * rot * scale1).transpose();

	batchLine.color = color;

	m_lineBuffer->update(m_lineCount++, batchLine);
}

void hagame::graphics::LineBuffer::draw(bool clearBuffer)
{
	m_cube->getMesh()->getVAO()->bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, getLineCount());
	if (clearBuffer) {
		clear();
	}
};