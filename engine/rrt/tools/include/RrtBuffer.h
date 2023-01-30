#ifndef RRT_BUFFER_H
#define RRT_BUFFER_H

#include <memory>
#include <string>

class Shader;

class RrtBuffer
{
public:
    RrtBuffer();
    virtual ~RrtBuffer();

    virtual void bind(const std::shared_ptr<Shader>& shader, int texUnit);

protected:
    void initBuf();

    std::string m_slot;
    uint32_t m_texId, m_bufId;
};

#endif  // RRT_BUFFER_H
