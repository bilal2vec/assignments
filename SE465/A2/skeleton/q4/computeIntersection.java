/* Excerpt from SweetHome3D, 
 * class com.eteks.sweethome3d.viewcontroller.PlanController. */

/* Author info:
 * Sweet Home 3D, Copyright (c) 2006 Emmanuel PUYBARET / eTeks <info@eteks.com>
 * Licensed under GPL v2+.
 */

/** 
 * Returns the intersection point between the lines defined by the points
 * (<code>point1</code>, <code>point2</code>) and (<code>point3</code>, <code>point4</code>).
 */
private float [] computeIntersection(float [] point1, float [] point2, float [] point3, float [] point4) {
	float x = point2 [0];
	float y = point2 [1];
	float alpha1 = (point2 [1] - point1 [1]) / (point2 [0] - point1 [0]);
	float alpha2 = (point4 [1] - point3 [1]) / (point4 [0] - point3 [0]);
	if (alpha1 != alpha2) {
		// If first line is vertical
		if (Math.abs(alpha1) > 4000) {
			if (Math.abs(alpha2) < 4000) {
				x = point1 [0];
				float beta2 = point4 [1] - alpha2 * point4 [0];
				y = alpha2 * x + beta2;
			}
		// If second line is vertical
		} else if (Math.abs(alpha2) > 4000) {
			if (Math.abs(alpha1) < 4000) {
				x = point3 [0];
				float beta1 = point2 [1] - alpha1 * point2 [0];
				y = alpha1 * x + beta1;
			}
		} else {
			boolean sameSignum = Math.signum(alpha1) == Math.signum(alpha2);
			if ((sameSignum && (Math.abs(alpha1) > Math.abs(alpha2) ? alpha1 / alpha2 : alpha2 / alpha1) > 1.0001) 
                || (!sameSignum && Math.abs(alpha1 - alpha2) > 1E-5)) {
				float beta1 = point2 [1] - alpha1 * point2 [0];
				float beta2 = point4 [1] - alpha2 * point4 [0];
				x = (beta2 - beta1) / (alpha1 - alpha2);
				y = alpha1 * x + beta1;
			}
		}
	}
	return new float [] {x, y};
}

  
