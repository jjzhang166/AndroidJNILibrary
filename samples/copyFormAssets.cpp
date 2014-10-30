#include <utils/SmartPoint.h>
#include <JNIJava/JavaBase.h>
#include <JNIJava/AndroidContent.h>
#include <JNIJava/JavaIO.h>

/**
 * 将assets中的文件拷贝出来。
 * @param[in] jfileName assets中的文件名。
 * @param[in] joutPath 输出路径。
 */
void copyFormAssets(jobject obj, jstring jfileName, jstring joutPath) {
	// 上下文。
	JContext context;
	context.InitJObject(obj);

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