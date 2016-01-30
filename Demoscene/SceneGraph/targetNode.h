#pragma once

#include "transform.h"
#include "../Math/vec.h"

namespace NS_SG
{
	

	
	class targetTransform : public transform
	{
	public:
		targetTransform(std::string name) : transform(name), target(NULL) {}

		void getLocalTransform(M3DMatrix44f in)
		{
			if (NULL == target)
			{
				m3dLoadIdentity44(in);
			
			}

			M3DMatrix44f eyeAbs;
			m3dLoadIdentity44(eyeAbs);
			if (NULL != getParent())
			{
			  getParent()->getAbsoluteTransform(eyeAbs);
			}
			
			// get target position into the local coordinate system
			// note: this will obviously fail horribly if target is dependent on this node
			
			//lol faen for en stut jeg er
			M3DMatrix44f eyeInvTemp, eyeAbsInv;
			M3DVector4f invProdukt;
			m3dInvertMatrix44(eyeInvTemp, eyeAbs);
			m3dGetMatrixColumn44(invProdukt, eyeInvTemp, 3);

			m3dLoadIdentity44(eyeAbsInv);
			m3dSetMatrixColumn44(eyeAbsInv, invProdukt, 3);
			
			
			//NS_VEC::VEC3 targetPos;
			M3DVector4f targetPos;
			M3DVector4f targetTran;

			M3DMatrix44f targetTransform;
			target->getAbsoluteTransform(targetTransform);
			m3dGetMatrixColumn44(targetTran, targetTransform, 3);
			targetTran[3] = 0;

			//printf("target: %f %f %f\n",
			//	targetTran[0], targetTran[1], targetTran[2]
			//	);



			m3dTransformVector4(targetPos, eyeAbsInv, targetTran);
			
			//HELVETES DRITT BIBLIOTEK! ERSTATT DET!
			M3DVector3f at, from, up;
			at[0] = targetPos[0];
			at[1] = targetPos[1];
			at[2] = targetPos[2];
			from[0] = eyeAbs[12];
			from[1] = eyeAbs[13];
			from[2] = eyeAbs[14];
			up[0] = 0.0f;
			up[1] = 1.0f;
			up[2] = 0.0f;


			M3DMatrix44f lookat, lookat_inv;
			m3dLookAt(lookat, at, from, up, 0.0f);
			

			m3dCopyMatrix44(in, lookat);

		}

		void setTarget(node *target)
		{
			this->target = target;
		}

		node *getTarget() const
		{
			return target;
		}

	private:
		node *target;
	};
	
}
