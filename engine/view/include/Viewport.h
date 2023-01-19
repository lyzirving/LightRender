#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <atomic>
#include <glm/glm.hpp>

class Viewport
{
public:
    Viewport();
    Viewport(float startX, float startY, float width, float height, float dist = 1.f);

    virtual ~Viewport();

    inline float x() { return m_start.x; }
    inline float y() { return m_start.y; }
    inline float width() { return m_size.x; }
    inline float height() { return m_size.y; }
    inline bool isChanged() { return m_change.load(); }

    const glm::mat4& getMat();
    void setStart(float x, float y);
    void setStart(const glm::vec2& start);
    void setSize(float width, float height);
    void setSize(const glm::vec2& size);
    void setDist(float dist);
private:
    void calcMat();

    glm::vec2 m_start, m_size;
    float m_dist;
    glm::mat4 m_viewportMat;
    std::atomic_bool m_change;
};

#endif // !VIEWPORT_H

