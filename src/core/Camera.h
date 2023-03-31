#pragma once



namespace ag
{
    struct Camera
	{
		float fieldOfView;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		Camera(float fov, float asp, float near, float far)
		{
			fieldOfView = fov;
			aspectRatio = asp;
			nearPlane = near;
			farPlane = far;
		}
	};
}
