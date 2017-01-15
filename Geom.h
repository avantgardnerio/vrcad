#pragma once

#include <limits>

#include "Vectors.h"

namespace geom {

	static float POSITIVE_INFINITY = std::numeric_limits<float>::infinity();

	float rayPlaneIsec(Vector3 planeNorm, Vector3 pointOnPlane, Vector3 rayOrigin, Vector3 rayDir);

	float rayPlaneIsec(Vector3 planeNorm, Vector3 pointOnPlane, Vector3 rayOrigin, Vector3 rayDir, Vector3 &isec);
}
