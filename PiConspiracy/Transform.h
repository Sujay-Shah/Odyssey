//
//  Transform.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class Transform : public Component
{
	COMPONENT_DECLARE(Transform);
public:
	~Transform(void);

	struct Axes
	{
		enum Enum
		{
			xAxis,
			yAxis,
			zAxis
		};
	}; typedef Axes::Enum EAxes;


	private:
		//PLS DONT change the order, Its tightly packed currently

		glm::quat m_rotation ; // current rotational orientation of the object
		vec3 m_position; // current postion of object
		vec3 m_scale; // scale of object
		vec3 m_axis; // axis about which the object has a rotation
	//	float m_angle; // angle of object with respect to  m_axis
		vec3 m_eulerAngles;//Euler angles for the rotation matrix


		glm::mat4 m_modelingMatrix; // the matrix the defines the transform


	public:
		void Initialize() override final;
		void Update(float a_dTime) override final; // the update function for transform
		void Serialize(const json&) override final;
		void HandleEvent(Event* a_pEvent) override final;

		
		// get the current position of object
		inline vec3 GetPosition(void) const
		{
			return m_position;
		}

		//Get the Euler Angles from the Json
		inline vec3 GetEulerAngles(void) const
		{
			return m_eulerAngles;
		}

		// Get the Modeling Matrix 
		inline mat4 GetModelingMatrix(void) const 
		{
			return m_modelingMatrix;
		}

		inline void SetEulerAngles(vec3 a_EulerAngles)
		{
			m_eulerAngles = a_EulerAngles;
		}

		inline void UpdateEulerAngles(vec3 a_EulerAngles)
		{
			m_eulerAngles = a_EulerAngles;
			//UpdateRigidBodyComponent();
		}


		// set the current position of object
		inline void SetPosition(vec3 a_pos)
		{
			m_position = a_pos;
		}

		// set the position to (a_x, a_y, a_z)
		inline void SetPosition(float a_x, float a_y, float a_z)
		{
			m_position = vec3(a_x, a_y, a_z);
		}

	

		inline void SetPositionFromLua(vec3 a_pos)
		{
			SetPosition(a_pos);
		}

		inline void UpdatePosition(vec3 a_pos)
		{
			m_position = a_pos;
			//UpdateRigidBodyComponent();
		}

		// get the rotation matrix of the object. Change to glm::quat
		inline glm::quat GetRotation(void) const
		{
			return m_rotation;
		}

		void SetRotation(glm::quat& a_q);
		void SetScale(vec3 a_scale);

		// get the angle of rotaion about a_axis
		inline float GetRotAngle(vec3 a_axis) const
		{
			//I have delcared a dot product that returns. In case theres an error, check function in Vector.h
			float dot = glm::dot(a_axis, this->m_position);

			return (dot / (a_axis.length() * this->m_position.length()));
		}

		// get the axis about which object is rotated
		inline vec3 GetRotAxis(void) const
		{
			return m_axis;
		}

		// get scale of the object
		inline vec3 GetScale(void) const
		{
			return m_scale;
		}

    inline vec3 GetScaleVec(void) const
    {
      return vec3(m_scale);
    }

		//Transformations

		/*Scale*/

		void Scale(vec3 a_scale); // set scale by the scaling vetor;

		/* Rotate */
		void Rotate(float a_angle, vec3 a_axis); // Rotate by angle a_angle about a_axis
		void Rotate(float a_angle, Axes::Enum a_axis); // Rotate by angle a_angle about x, y or z axis
		void Rotate(float a_angle); // rotate about m_axis;
		void EulerToQuat(glm::quat &q1, vec3 euler);
		/* Translate*/
		void Translate(vec3 a_translation); // translate with by the values in a_translation


		void Translate(float a_x, float a_y, float a_z); // translate by the values given by a_x, a_y and a_z

		void Translate(float a_xyz); // translate along x, y and z axis by a_xyz value


		/***for a plane***/
		Transform(vec3 a_position, glm::quat a_rotation, float a_scale, float a_angle, vec3 a_axis, vec3 a_eulerAngles);

		/* Rotate Plane */
	   //void Rotate(float a_angle, Vector4 a_axis); // Rotate by angle a_angle about a_axis


	   // Inherited via Component
		virtual void Deactivate();


		// Inherited via Component
		virtual void OverrideParams(const json&,float) override;

};

