#include "Geom.h"

namespace geom {
	float rayPlaneIsec(Vector3 planeNorm, Vector3 pointOnPlane, Vector3 rayOrigin, Vector3 rayDir, Vector3 &isec) {
		float t = rayPlaneIsec(planeNorm, pointOnPlane, rayOrigin, rayDir);
		if (t != POSITIVE_INFINITY)
			isec = rayOrigin + (rayDir * t);
		return t;
	}

	float rayPlaneIsec(Vector3 planeNorm, Vector3 pointOnPlane, Vector3 rayOrigin, Vector3 rayDir) {
		float denom = planeNorm.dot(rayDir);
		if (fabs(denom) <= 0.000001f)
			return POSITIVE_INFINITY;
		float t = (pointOnPlane - rayOrigin).dot(planeNorm) / denom;
		return t;
	}
}
