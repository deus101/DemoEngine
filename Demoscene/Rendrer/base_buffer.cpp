#include "base_buffer.h"

GLuint base_buffer::DiffArrayMapTexure = 0;

#define Derive_Buffer_CRTP(Type) class Type: public Buffer_CRTP<Type>
/*
base_buffer::base_buffer()
{
}
*/
/*
base_buffer::~base_buffer()
{
}
*/
/*
//int Init(unsigned int WindowWidth, unsigned int WindowHeight);
//int EnablePass(int PassId)
*/