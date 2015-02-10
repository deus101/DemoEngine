#pragma once
#include "math3d.h"
#include "transform.h"
#include "vec.h"

namespace sg
{
	namespace noder
	{
		class objTransform : public transform
		{
		public:
			objTransform(std::string name) :
				transform(name),
				position(0, 0, 0),
				rotation(0, 0, 0),
				scale(0, 0, 0),
				matrix_dirty(true)
			{}
			//should maybe have a failure return here
			void getLocalTransform(M3DMatrix44f in)
			{
				if (matrix_dirty)
				{
#if 0
					M3DMatrix44f m_tran;
					m3dLoadIdentity44(m_tran);
					m3dTranslationMatrix44(m_tran, position.X, position.Y, position.Z);
					
					M3DMatrix44f m_rot;
					m3dRotationMatrix44(m_rot, rotation);

					m3dMatrixMultiply44(m_rot, m_tran, m_rot);

					M3DMatrix44f m_scale;
					m3dLoadIdentity44(m_scale);
					m3dScaleMatrix44(m_scale, scale.X, scale.Y, scale.Z);

					m3dMatrixMultiply44(m_scale, m_rot, m_scale);
					m3dCopyMatrix44(matrix, m_scale);
						
#else
					M3DMatrix44f m_scale;
					m3dLoadIdentity44(m_scale);
					m3dScaleMatrix44(m_scale, scale.X, scale.Y, scale.Z);

					M3DMatrix44f m_rot;
					m3dRotationMatrix44(m_rot, rotation);

					m3dMatrixMultiply44(m_rot, m_scale, m_rot);

					M3DMatrix44f m_tran;
					m3dLoadIdentity44(m_tran);
					m3dTranslationMatrix44(m_tran, position.X, position.Y, position.Z);


					m3dMatrixMultiply44(m_tran, m_rot, m_tran);
					m3dCopyMatrix44(matrix, m_tran);
#endif				
					matrix_dirty = false;
				}
				m3dCopyMatrix44(in, matrix);
			}

			void setPosition(NS_VEC::VEC3 &pos)
			{
				this->position = pos;
				this->matrix_dirty = true;
			}

			void setRotation(NS_VEC::QUAT &rot)
			{
				this->rotation = rot;
				this->matrix_dirty = true;
			}

			void setScale(NS_VEC::VEC3 &scale)
			{
				this->scale = scale;
				this->matrix_dirty = true;
			}

		private:
			NS_VEC::VEC3 position;
			NS_VEC::QUAT rotation;
			NS_VEC::VEC3 scale;

			M3DMatrix44f matrix;
			bool            matrix_dirty;
		};
	}
}