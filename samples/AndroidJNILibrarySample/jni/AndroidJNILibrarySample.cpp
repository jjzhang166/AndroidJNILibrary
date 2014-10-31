#include <jni.h>


/**
 * 将assets中的文件拷贝出来。
 * 
 */
void copyFormAssets(JNIEnv *env, jobject thiz, jstring jfileName, jstring joutPath) {
	// 上下文。
	JContext context;
	context.InitJObject(thiz);

	// assets目录中的文件名。
	JString fileName;
	fileName.InitJObject(jfileName);

	// 输出目录。
	JString outPath;
	outPath.InitJObject(joutPath);

	JFile fileOutPath;
	fileOutPath.New(outPath);

	// AssetManager。
	lsp<JAssetManager> am = context.getAssets();
	lsp<JInputStream> is = am->open(fileName);

	JFile dir;
	dir.New(*(lsp<JString>(fileOutPath.getParent()).get()));
	// 如果目录不存在，则创建。
	if (false == dir.exists()) {
		dir.mkdirs();
	}

	JFileOutputStream os;
	os.New(outPath);

	JArray<jbyte> buffer;
	buffer.New(1024);

	// 写文件。
	jint length;
	while ((length = is->read(buffer)) > 0) {
		os.write(buffer, 0, 1024);
	}

	is->close();
	os.close();
}

const char* g_ClassName = "com/buwai/androidjnilibrarysample/MainActivity";

static const JNINativeMethod gMethods[] = { //定义批量注册的数组，是注册的关键部
	{ "JNITest", "(Ljava/lang/String;)I", (void*) JNITest }, 
	{ "copyFormAssets", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)copyFormAssets }, 
	{ "hookFileClass", "()V", (void*)hookFileClass }, 
	{ "template_test", "(I)V", (void*)template_test }
};

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
	jint result = -1;
	JNIEnv* env;
	result = vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if (result != JNI_OK) {
		return -1;
	}

	jclass clazz;
	//这里可以找到要注册的类，前提是这个类已经加载到java虚拟机中。 这里说明，动态库和有native方法的类之间，没有任何对应关系。
	clazz = env->FindClass(g_ClassName);
	if (clazz == NULL) {
		return -1;
	}

	if (env->RegisterNatives(clazz, gMethods,
		sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK) //这里就是关键了，把本地函数和一个java类方法关联起来。不管之前是否关联过，一律把之前的替换掉！
	{
		printf("register native method failed!\n");
		return -1;
	}
	result = JNI_VERSION_1_4;
	return result;
}
