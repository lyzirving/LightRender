#ifndef RRT_BVH_BUF_H
#define RRT_BVH_BUF_H

#include <vector>

#include "RrtBuffer.h"

class GfxShader;

struct EncodeBVH;

class RrtBVHBuf : public RrtBuffer
{
public:
    RrtBVHBuf();
    virtual ~RrtBVHBuf();

    void addNodes(const std::vector<EncodeBVH>& input);
    virtual void bind(const std::shared_ptr<GfxShader>& shader, int texUnit) override;

    inline int32_t nodesCnt() { return m_nodes.size(); }

protected:
    std::vector<EncodeBVH> m_nodes;
};

#endif  // RRT_BVH_BUF_H
