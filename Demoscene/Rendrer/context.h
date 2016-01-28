#pragma once


#include "../util.h"

#include "gbuffer.h"
#include "callbacks.h"
//namespace NS_REND
//{
//	class context
//	{
//	public:
		//static GLFWwindow* s_pWindow = NULL;
		//static GBuffer * mGBuffer = 0;
		static unsigned int pHeight;
		static unsigned int pWidth;
		static bool m_created;
		
		
		static bool ResizeBuffer;
		
		static int glu_Window;
		//static HGLRC RendContext;
		//static HDC DeviceContext;
		static int Glu_Window;

		//static HGLRC SharedContex;
		static GLuint Program;

		//static GBuffer* mgBuffer;


		//context();
		//~context();
		void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods);

		void ErrorCallback(int error, const char* description);
		//void Init(int argc, char** arg, bool aDepth, bool aStencil);
		//static HGLRC InitWindow(unsigned int Width, unsigned int Height, bool fs, const char* aTitle);

		bool Init(int argc, char** arg, bool aDepth, bool aStencil, unsigned int Width, unsigned int Height, bool fs, const char* aTitle);


		void ChangeSize(unsigned int w, unsigned int h);
		
		
		void ContextRun(ICallbacks* pCallbacks);

		void Swap();

		void LeaveMainLoop();

		unsigned int GetPixelWidth() ;

		unsigned int GetPixelHeight() ;

		//bool GetGBStatus() ;

		//void SetGBStatus(bool) ;


	//private:


	//};
		

	//static GBuffer * mGBuffer;



	

//}