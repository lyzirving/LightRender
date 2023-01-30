#ifndef RRT_BVH_BUF_H
#define RRT_BVH_BUF_H

#include <vector>

#include "RrtBuffer.h"

class Shader;

struct RrtBVHNode;

class RrtBVHBuf : public RrtBuffer
{
public:
    RrtBVHBuf();
    virtual ~RrtBVHBuf();

    void addNodes(const std::vector<RrtBVHNode>& input);
    virtual void bind(const std::shared_ptr<Shader>& shader, int texUnit) override;

    inline int32_t nodesCnt() { return m_nodes.size(); }

protected:
    std::vector<RrtBVHNode> m_nodes;
};

#endif  // RRT_BVH_BUF_H
