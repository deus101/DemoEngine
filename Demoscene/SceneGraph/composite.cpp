#include "composite.h"
//#include "sync.h"
#include <typeinfo>
#include <iostream>

using namespace NS_SG;

	
		float gravity = 9.8f;

		NS_VEC::QUAT qStart = NS_VEC::QUAT(-30, 0, 1, 0) * NS_VEC::QUAT(-45, 1, 0, 0);
		NS_VEC::QUAT qEnd = NS_VEC::QUAT(-40, 0, 0, 1) * NS_VEC::QUAT(45, 1, 0, 0);

        //typedef boost::shared_ptr< actors > actorsPtr;

       // Nodes herd;
		float counter;
		double second;

		const struct sync_track * Peek_C_L_X, *Peek_C_L_Y, *Peek_C_L_Z;
		const struct sync_track * From_C_P_X, *From_C_P_Y, *From_C_P_Z;

		static inline NS_VEC::VEC3 lerp(std::map<float, NS_VEC::VEC3>::const_iterator lower, std::map<float, NS_VEC::VEC3>::const_iterator upper, float time)
		{
			// find local time
			float t = (time - lower->first) / (upper->first - lower->first);

			// lerp
			NS_VEC::VEC3 delta = upper->second - lower->second;
			return lower->second + delta * t;
		}

		static inline NS_VEC::QUAT lerp(std::map<float, NS_VEC::QUAT>::const_iterator lower, std::map<float, NS_VEC::QUAT>::const_iterator upper, float time)
		{
			// find local time
			float t = (time - lower->first) / (upper->first - lower->first);

			// lerp
			return lower->second.Slerp(upper->second, t);
		}
	

		NS_VEC::VEC3  objectAnim::getPos(float time)
		{
			// find bounds
			std::map<float, NS_VEC::VEC3 >::const_iterator upper = posTrack.upper_bound(time);
			std::map<float, NS_VEC::VEC3 >::const_iterator lower = upper;
			lower--;

			// bounds check
			if (lower == posTrack.end()) return upper->second;
			if (upper == posTrack.end()) return lower->second;

			// interpolate
			return lerp(lower, upper, time);
		}

		NS_VEC::QUAT  objectAnim::getRot(float time)
		{
			// find bounds
			std::map<float, NS_VEC::QUAT >::const_iterator upper = rotTrack.upper_bound(time);
			std::map<float, NS_VEC::QUAT >::const_iterator lower = upper;
			lower--;

			// bounds check
			if (lower == rotTrack.end()) return upper->second;
			if (upper == rotTrack.end()) return lower->second;

			// interpolate
			return lerp(lower, upper, time);
		}

		NS_VEC::VEC3 objectAnim::getScale(float time)
		{
			// find bounds
			std::map<float, NS_VEC::VEC3 >::const_iterator upper = scaleTrack.upper_bound(time);
			std::map<float, NS_VEC::VEC3 >::const_iterator lower = upper;
			lower--;

			// bounds check
			if (lower == scaleTrack.end()) return upper->second;
			if (upper == scaleTrack.end()) return lower->second;

			// interpolate
			return lerp(lower, upper, time);
		}

		void composite::anim(float time)
		{
			std::map<objTransform*, objectAnim>::iterator i;

			for (i = animTracks.begin(); i != animTracks.end(); ++i)
			{
				objTransform *target = i->first;
				target->setPosition(i->second.getPos(time));
				target->setRotation(i->second.getRot(time));
				target->setScale(i->second.getScale(time));
			}
		}



		//denne kan gjenbrukes til noe lurt

		void composite::AddActor(int typeID, GLfloat x, GLfloat y, GLfloat z)
		{
			switch (typeID)
			{
			case 001:
				//herd.push_back(actorsPtr(new camera(x, y, z, false)));
                break;
			case 002:
				//herd.push_back(actorsPtr(new cube(x, y, z, false)));
				break;
			case 003:
				//herd.push_back(actorsPtr(new composite()));
			default:
				std::cout << "Error no such object: " << typeID << std::endl;
			}
		}
		void composite::AddActor(NS_VEC::VEC3 co, std::string filename1, std::string filename2, GLfloat x, GLfloat y, GLfloat z)
		{
			//herd.push_back(actorsPtr(new model(x, y, z, false, filename1, filename2, co)));


		}



		
	//	void composite::Magic(bool draw)
	//	{
	//		
	//		//found out the timer started way to early.
	//		if (counter == -1.0f)
	//		{
	//			counter = 0.0f;
	//			clock.restart();
	//		}
	//		//counter = counter + 0.01f;
	//		
	//		for (unsigned int i = 0; i < herd.size(); i++)
	//		{
	//			//QUAT qStart(90, 0, 1, 0);
	//			//QUAT qEnd = QUAT(90, 0, 0, 1) * QUAT(90, 0, 0, 1);
	//			//Animation handler, but how to handle transofrmation data and calls?
	//			//should scripting functions take place here or in another class?
	//			/*
	//			if(  typeid( * herd[i].get())==typeid( camera ))
	//			cout << "actor nr: " << i << " is camera" << endl;
	//			if(  typeid( * herd[i].get())==typeid( model ))
	//			cout << "actor nr: " << i << " is model" << endl;
	//			*/
	//			if (typeid(*herd[i].get()) == typeid(camera))
	//			{
	//				//herd[i]->TranslateLocal(1.0f * second, 5.0f, 15.0f + (-0.05f * 10 * second));
	//				//herd[i]->f
	//				herd[i]->RotateLocal(30.0f  * second , 1.0f, 0.0f, 0.0f);
	//				//herd[i]->SetModel();
	//				//for lookat m� man inverse
	//			}
	//			if (typeid(*herd[i].get()) == typeid(model))
	//			{ 
	//				
	//				//herd[i]->RotateLocal(float(sync_get_val(Peek_C_L_Y,second )), 0, 1, 0 );
	//				//herd[i]->RotateLocal(qStart.Slerp(qEnd, second));
	//				herd[i]->RotateLocal(90 * second, 0.0f, 1.0f, 0.0f);
	//				//herd[i]->TranslateLocal(0.0f, 0.0f, -second );
	//				//er ikke dette alt jeg trenger til gnu rocket?
	//				//if (counter < 1.0f)
	//				//{
	//				//	VEC3 axRot;sy
	//				//	float axDeg;
	//				//	qStart.Slerp(qEnd, counter).TAA(axRot, axDeg);
	//				//	//herd[i]->RotateLocal(qSl.W,qStart.X, qStart.Y, qStart.Z);
	//				//	herd[i]->RotateLocal(axDeg, axRot.X, axRot.Y, axRot.Z);
	//				//}
	//			//herd[i]->RotateLocal(1.0f, 1.0f, 0.0f, 0.0f);
	//				//herd[i]->TranslateLocal(0.0f, 0.0f, 2.0f * clock.elapsed());
	//			}
	//			//herd[i]->Translate(0.0f, 0.0f, 0.05f);
	//			//if(draw)
	//			herd[i]->draw();
	//		}
	//		

	//		second = second + (clock.elapsed());

	//		//cout << "FPS: " << counter << endl;

	//		if (second >= 10.0)
	//		{

	//			//cout << "FPS: " << counter << endl;
	//			counter = 0;
	//			second = 0;

	//		}

	//		//clock.elapsed();
	//		clock.restart();
	//	}

	///*	void composite::DrawAll(){
	//		for (unsigned int i = 0; i < herd.size(); i++)
	//		{
	//			herd[i]->draw();
	//		}
	//	}*/

		void composite::StartTimer()
		{
			//clock.
			//clock = Timer();
			//counter = -1.0f;
			//second = 0;

		}

		

		void composite::SortTracks()
		{

			//rocket = sync_create_device("sync");


		/*	Peek_C_L_X = sync_get_track( rocket, "Look.X");
			Peek_C_L_Y = sync_get_track( rocket, "Look.Y");
			Peek_C_L_Z = sync_get_track( rocket, "Look.Z");

			From_C_P_X = sync_get_track(rocket, "Pos.X");
			From_C_P_Y = sync_get_track(rocket, "Pos.Y");
			From_C_P_Z = sync_get_track(rocket, "Pos.Z");
			*/


		}
	

