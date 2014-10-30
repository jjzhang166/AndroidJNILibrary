#include "../stdafx.h"
#include "JNIReflect.h"

// ��÷���ID��
jmethodID GetMethodID_(IN JNIEnv* env, IN jobject obj, IN const char* methodName, IN const char* sig) {
	jclass clazz = env->GetObjectClass(obj);
	if (NULL == clazz) {
		return NULL;
	}
	jmethodID id = env->GetMethodID(clazz, methodName, sig);
	env->DeleteLocalRef(clazz);
	return id;
}

// ��÷���ID��
jmethodID GetMethodID_(IN JNIEnv* env, IN const char* className, IN const char* methodName, IN const char* sig) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return NULL;
	}
	jmethodID id = env->GetMethodID(clazz, methodName, sig);
	env->DeleteLocalRef(clazz);
	return id;
}

// ��þ�̬����ID��
// jmethodID GetStaticMethodID_(IN JNIEnv* env, IN const char* className, IN const char* methodName, IN const char* sig) {
// 	jclass clazz = env->FindClass(className);
// 	if (NULL == clazz) {
// 		return NULL;
// 	}
// 	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
// 	env->DeleteLocalRef(clazz);
// 	return id;
// }

//////////////////////////////////////////////////////////////////////////

// ����Java������
jobject CallObjectMethod_(IN JNIEnv* env, IN jobject obj, IN const char* methodName, IN const char* sig, IN ...) {
	jmethodID id = GetMethodID_(env, obj, methodName, sig);
	if (NULL == id) return NULL;

	va_list ap;
	va_start(ap, sig);
	jobject result = env->CallObjectMethodV(obj, id, ap);
	va_end(ap);

	return result;
}

jint CallStaticIntMethod_(IN JNIEnv* env, IN const char* className, IN const char* methodName, IN const char* sig, IN ...) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return 0;
	}

	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
	if (NULL == id) return 0;

	va_list ap;
	va_start(ap, sig);
	jint result = env->CallStaticIntMethodV(clazz, id, ap);
	va_end(ap);

	env->DeleteLocalRef(clazz);
	return result;
}

// ����Java������
jobject CallStaticObjectMethod_(IN JNIEnv* env, IN const char* className, IN const char* methodName, IN const char* sig, IN ...) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return NULL;
	}

	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
	if (NULL == id) return NULL;

	va_list ap;
	va_start(ap, sig);
	jobject result = env->CallStaticObjectMethodV(clazz, id, ap);
	va_end(ap);

	return result;
}
