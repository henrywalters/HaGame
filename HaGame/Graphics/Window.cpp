#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "Window.h"

void hagame::graphics::Window::initGLAttribs() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 32);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

}

void hagame::graphics::Window::initGLContext() {

	glewExperimental = GL_TRUE;

	gl = SDL_GL_CreateContext(window);

	if (gl == NULL) {
		throw new std::exception("Failed to initialize OpenGL");
	}

	int k;
	SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &k);
	printf("Stencil size %d\n", k);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK) {
		throw new std::exception("Failed to initialize GLEW");
	}

	glewGetErrorString(glewError);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glErrorHandler, 0);

	hagame::graphics::textBuffer.initializeForGL();
	hagame::graphics::lineBuffer.initializeBuffers();

	m_renderQuad = std::make_shared<Quad>(Vec2(2.0f));
	m_renderQuad->setOrigin(Vec2(1.0f));

	m_lightBuffer = std::make_shared<FrameBuffer>();
	m_lightBuffer->initialize();
	m_lightBuffer->bind();
	m_lightBuffer->initializeRenderBufferObject(size.cast<int>());
	m_lightColorTexture = std::make_shared<RawTexture<GL_RGBA>>(size.cast<int>());
	m_lightBuffer->attachRawTexture(m_lightColorTexture.get());

	m_geometryBuffer = std::make_shared<FrameBuffer>();
	m_geometryBuffer->initialize();
	m_geometryBuffer->bind();
	m_geometryBuffer->initializeRenderBufferObject(size.cast<int>());
	m_geometryColorTexture = std::make_shared<RawTexture<GL_RGBA>>(size.cast<int>());
	m_normalTexture = std::make_shared<RawTexture<GL_RGBA16F>>(size.cast<int>());
	m_positionTexture = std::make_shared<RawTexture<GL_RGBA16F>>(size.cast<int>());

	m_geometryBuffer->attachRawTexture(m_geometryColorTexture.get());
	m_geometryBuffer->attachRawTexture(m_normalTexture.get(), 1);
	m_geometryBuffer->attachRawTexture(m_positionTexture.get(), 2);

	renderPasses.create(RenderMode::Geometry, size.cast<int>()); // Layout 0
	renderPasses.create(RenderMode::Light, size.cast<int>()); // Layout 1
	renderPasses.create(RenderMode::Normal, size.cast<int>()); // Layout 2
	renderPasses.create(RenderMode::Specular, size.cast<int>()); // Layout 3
	renderPasses.create(RenderMode::Unlit, size.cast<int>()); // Layout 4

	if (!m_geometryBuffer->isComplete()) {
		throw new std::exception("FAILED TO GENERATE GEOMETRY FRAMEBUFFER");
	}

	if (!m_lightBuffer->isComplete()) {
		throw new std::exception("FAILED TO GENERATE LIGHT FRAMEBUFFER");
	}

	m_geometryBuffer->unbind();

	//if (SDL_GL_SetSwapInterval(-1) < 0) {
	//	throw new std::exception("Failed to set VSync");
	//}
}

hagame::graphics::Window::Window()
{
	mode = WindowMode::FULLSCREEN;
	pos = Vec2::Zero();
	size = Vec2::Zero();
}

hagame::graphics::Window::Window(Vec2 _size)
{
	mode = WindowMode::BORDERLESS_CENTERED;
	size = _size;
	pos = Vec2::Zero();
}

hagame::graphics::Window::Window(Vec2 _pos, Vec2 _size)
{
	mode = WindowMode::BORDERLESS;
	size = _size;
	pos = _pos;
}

hagame::graphics::Window::Window(Vec2 _size, std::string _title)
{
	mode = WindowMode::BORDERED_CENTERED;
	size = _size;
	pos = Vec2::Zero();
	title = _title;
}

hagame::graphics::Window::Window(Vec2 _pos, Vec2 _size, std::string _title)
{
	mode = WindowMode::BORDERED;
	size = _size;
	pos = _pos;
	title = _title;
}

hagame::graphics::Window hagame::graphics::Window::ForMonitor(Monitor monitor)
{
	Rect bounds = MonitorManager::GetDisplayBounds(monitor.id);
	return Window(bounds.pos, bounds.size);
}

Vec2 hagame::graphics::Window::UltrawideResolution(int scale)
{
	return Vec2(21 * scale, 9 * scale);
}

Vec2 hagame::graphics::Window::StandardResolution(int scale)
{
	return Vec2(16 * scale, 9 * scale);
}

void hagame::graphics::Window::create() {
	
	initGLAttribs();

	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	int x = 0, y = 0, w = 0, h = 0;

	switch (mode) {
	case WindowMode::BORDERED:
		x = pos[0], y = pos[1], w = size[0], h = size[1];
		break;
	case WindowMode::BORDERED_CENTERED:
		x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED, w = size[0], h = size[1];
		break;
	case WindowMode::BORDERLESS:
		flags |= SDL_WINDOW_BORDERLESS;
		x = pos[0], y = pos[1], w = size[0], h = size[1];
		break;
	case WindowMode::BORDERLESS_CENTERED:
		flags |= SDL_WINDOW_BORDERLESS;
		x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED, w = size[0], h = size[1];
		break;
	case WindowMode::FULLSCREEN:
		x = pos[0], y = pos[1], w = size[0], h = size[1];
		flags |= SDL_WINDOW_FULLSCREEN;
		break;
	}

	window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

	//int width, height;

	//SDL_GetWindowSize(window, &width, &height);

	//size[0] = (float)width;
	//size[1] = (float)height;

	if (window == NULL) {
		std::cout << "Failed to create SDL Window: " << SDL_GetError() << "\n";
	}

	initGLContext();
}

void hagame::graphics::Window::destroy() {
	SDL_DestroyWindow(window);
}

void hagame::graphics::Window::clearGeometry() {
	m_geometryBuffer->bind();
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	resetGl();
}

void hagame::graphics::Window::clearLight()
{
	m_lightBuffer->bind();
	glClearColor(ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	resetGl();
}

void hagame::graphics::Window::resetGl()
{
	// m_geometryBuffer->bind();
	glStencilMask(0xFF);

	glEnable(GL_STENCIL_TEST);
	// glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendEquation(GL_FUNC_ADD);
	glDepthFunc(GL_LESS);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void hagame::graphics::Window::activateGeometryBuffer()
{
	m_geometryBuffer->bind();
}

void hagame::graphics::Window::activateLightBuffer()
{
	m_lightBuffer->bind();
}

void hagame::graphics::Window::renderGeometry()
{
	m_geometryBuffer->bind();
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	m_geometryBuffer->unbind();
}

void hagame::graphics::Window::renderLight()
{
	m_lightBuffer->bind();
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	m_lightBuffer->unbind();
}

void hagame::graphics::Window::render() {

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderShader->use();
	glDisable(GL_DEPTH_TEST);

	renderPasses.activateTextures();

	m_renderQuad->getMesh()->draw();

	SDL_GL_SwapWindow(window);
}

void hagame::graphics::Window::turnOnWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	renderMode = WindowRenderMode::Wireframe;
}

void hagame::graphics::Window::turnOnFilledMode() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderMode = WindowRenderMode::Filled;
}

void hagame::graphics::Window::toggleRenderMode() {
	if (renderMode == WindowRenderMode::Wireframe) {
		turnOnFilledMode();
	} else {
		turnOnWireframeMode();
	}
}

void hagame::graphics::Window::turnOnVSync()
{
	if (SDL_GL_SetSwapInterval(-1) < 0) {
		throw new std::exception("Failed to set VSync");
	}
}

void hagame::graphics::Window::turnOffVSync()
{
	if (SDL_GL_SetSwapInterval(0) < 0) {
		throw new std::exception("Failed to set VSync");
	}
}

void hagame::graphics::Window::setTitle(String title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

void hagame::graphics::Window::setViewport(Rect viewport)
{
	_viewport = viewport;
	glViewport(viewport.pos[0], size[1] - viewport.size[1] - viewport.pos[1], viewport.size[0], viewport.size[1]);
}

bool  hagame::graphics::Window::inViewport(Vec2 point)
{
	return _viewport.contains(point);
}

#endif