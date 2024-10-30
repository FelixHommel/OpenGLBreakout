#include "PostProcessor.hpp"

PostProcessor::PostProcessor(const Shader& shader, unsigned int width, unsigned int height)
    : m_msfbo{}
    , m_fbo{}
    , m_rbo{}
    , m_vao{}
    , m_ppShader{ shader }
    , m_width{ width }
    , m_height{ height }
    , m_confuse{ false }
    , m_chaos{ false }
    , m_shake{ false }
{
    glGenFramebuffers(1, &m_msfbo);
    glGenFramebuffers(1, &m_fbo);
    glGenRenderbuffers(1, &m_rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    m_texture.generate(width, height, nullptr);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.ID(), 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initRenderData();
    m_ppShader.setInt("scene", 0, true);

    glUniform2fv(glGetUniformLocation(m_ppShader.ID(), "offsets"), 9, (float*)offsets);
    glUniform1iv(glGetUniformLocation(m_ppShader.ID(), "edge_kernel"), edgeKernel.size(), edgeKernel.data());
    glUniform1fv(glGetUniformLocation(m_ppShader.ID(), "blur_kernel"), blurKernel.size(), blurKernel.data());
}

PostProcessor::~PostProcessor()
{
    glDeleteVertexArrays(1, &m_vao);
}

void PostProcessor::beginRender() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msfbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(float time)
{
    m_ppShader.use();
    m_ppShader.setFloat("uTime", time);
    m_ppShader.setInt("uConfuse", m_confuse);
    m_ppShader.setInt("uChaos", m_chaos);
    m_ppShader.setInt("uShake", m_shake);

    glActiveTexture(GL_TEXTURE0);
    m_texture.bind();
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
    unsigned int vbo{};
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesPP.size(), verticesPP.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
