#ifndef RRT_TRI_BUFFER_H
#define RRT_TRI_BUFFER_H

#include <vector>

#include "RrtBuffer.h"

class GfxShader;

struct EncodeTriangle;

class RrtTriBuf : public RrtBuffer
{
public:
	RrtTriBuf();
	virtual ~RrtTriBuf();

	void addTriangles(const std::vector<EncodeTriangle>& input);
	virtual void bind(const std::shared_ptr<GfxShader>& shader, int texUnit) override;

	inline int32_t triangleCnt() { return m_triangles.size(); }

protected:
	std::vector<EncodeTriangle> m_triangles;
};

#endif // RRT_TRI_BUFFER_H
