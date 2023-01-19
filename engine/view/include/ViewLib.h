#ifndef VIEW_LIB_H
#define VIEW_LIB_H

#include <glm/glm.hpp>

class ViewLib
{
public:
	// spherical coordinate system to cartesian coordinate system
	static void SCStoCCS(float theta, float phi, float r, glm::vec3& out);
	// cartesian coordinate system to spherical coordinate system
	static void CCStoSCS(const glm::vec3& ccs, glm::vec3& out);

private:
	ViewLib() {};
	~ViewLib() = default;
	
};

#endif // !VIEW_LIB_H

