#ifndef GEOMPACKET_HPP
#define GEOMPACKET_HPP

#include "renderPacket.h"

//Jeg tror jeg overforbruker
namespace NS_EFF{

	class GeomPacket : public renderPacket {
	public:

		GeomPacket();

		virtual bool Init();

		void SetWVP(const M3DMatrix44f& WVP);
		void SetWorldMatrix(const M3DMatrix44f& W);

		void SetViewMatrix(const M3DMatrix44f& V);

		void SetProjectionMatrix(const M3DMatrix44f& P);

		
		//void SetDiffuseTextureUnit(unsigned int TextureUnit);
		void SetDiffuseTextureUnit(GLenum TextureUnit);
		//void SetDiffuseTextureUnit(GLint TextureUnit);
		//void SetBumpTextureUnit(unsigned int TextureUnit);

		void SetMaterialMapUnit(GLenum TextureUnit);
		//void SetMaterialMapUnit(GLint TextureUnit);
		void SetMaterialsCount(unsigned int MatCount);


	private:


		GLuint m_WVPLocation;
		GLuint m_ProjectionLocation;
		GLuint m_ViewLocation;
		GLuint m_WorldMatrixLocation;
		
		
		GLuint v4_FarNearLocation;
		GLuint m_diffuseTextureUnitLocation;
		GLuint m_bumpTextureUnitLocation;


		GLuint m_MaterialMapTextureUnitLocation;
		GLuint m_MaterialCountUnitLocation;


	};
}

#endif