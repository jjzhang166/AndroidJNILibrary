#include <jni.h>
#include <JNIJava/JavaBase.h>
#include <JNIJava/AndroidContent.h>
#include <utils/SmartPoint.h>


/**
 * ��assets�е��ļ�����������
 * 
 */
void copyFormAssets(JNIEnv *env, jobject thiz, jstring jfileName, jstring joutPath) {
	// �����ġ�
	JContext context;
	context.InitJObject(thiz);

	// assetsĿ¼�е��ļ�����
	JString fileName;
	fileName.InitJObject(jfileName);

	// ���Ŀ¼��
	JString outPath;
	outPath.InitJObject(joutPath);

	JFile fileOutPath;
	fileOutPath.New(outPath);

	// AssetManager��
	lsp<JAssetManager> am = context.getAssets();
	lsp<JInputStream> is = am->open(fileName);

	JFile dir;
	dir.New(*(lsp<JString>(fileOutPath.getParent()).get()));
	// ���Ŀ¼�����ڣ��򴴽���
	if (false == dir.exists()) {
		dir.mkdirs();
	}

	JFileOutputStream os;
	os.New(outPath);

	JArray<jbyte> buffer;
	buffer.New(1024);

	// д�ļ���
	jint length;
	while ((length = is->read(buffer)) > 0) {
		os.write(buffer, 0, length);
	}

	is->close();
	os.close();
}

const char* g_ClassName = "com/buwai/androidjnilibrarysample/MainActivity";

static const JNINativeMethod gMethods[] = {
	{ "copyFormAssets", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)copyFormAssets }
};

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
	jint result = -1;
	JNIEnv* env;
	result = vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if (result != JNI_OK) {
		return -1;
	}

	jclass clazz;
	clazz = env->FindClass(g_ClassName);
	if (clazz == NULL) {
		return -1;
	}

	if (env->RegisterNatives(clazz, gMethods,
		sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	return result;
}
