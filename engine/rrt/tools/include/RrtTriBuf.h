#ifndef RRT_TRI_BUFFER_H
#define RRT_TRI_BUFFER_H

#include <vector>

#include "RrtBuffer.h"

class Shader;

struct RrtTriangle;

class RrtTriBuf : public RrtBuffer
{
public:
	RrtTriBuf();
	virtual ~RrtTriBuf();

	void addTriangles(const std::vector<RrtTriangle>& input);
	virtual void bind(const std::shared_ptr<Shader>& shader, int texUnit) override;

	inline int32_t triangleCnt() { return m_triangles.size(); }

protected:
	std::vector<RrtTriangle> m_triangles;
};

#endif // RRT_TRI_BUFFER_H
