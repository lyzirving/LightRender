#ifndef RRT_BUFFER_H
#define RRT_BUFFER_H

#include <memory>
#include <string>

class GfxShader;

class RrtBuffer
{
public:
    RrtBuffer();
    virtual ~RrtBuffer();

    virtual void bind(const std::shared_ptr<GfxShader>& shader, int texUnit);

protected:
    void initBuf();

    std::string m_slot;
    uint32_t m_texId, m_bufId;
};

#endif  // RRT_BUFFER_H
