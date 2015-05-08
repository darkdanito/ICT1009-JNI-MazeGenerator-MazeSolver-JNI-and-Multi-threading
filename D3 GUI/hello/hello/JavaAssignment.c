#include <jni.h>
#include "JavaAssignment.h"
#include "JavaAssignmentImpl.h"

JNIEXPORT void JNICALL Java_JavaAssignment_sayHello(JNIEnv *env, jobject thisObj)
{
	sayHello();  // invoke C++ function
	return;
}