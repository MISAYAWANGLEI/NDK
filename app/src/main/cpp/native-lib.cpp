#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"JNI",__VA_ARGS__);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_wanglei55_ndk_MainActivity_stringFromJNI(JNIEnv *env,jobject /* this */) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_wanglei55_ndk_MainActivity_arrayTest(JNIEnv *env,
        jobject instance,jint i,jintArray a1_,jobjectArray a2) {
    LOGE("i的值为：%d", i);
    // 第二个参数：
    // true：拷贝一个新数组
    // false: 就是使用的java的数组 (地址)
    jint *a1 = env->GetIntArrayElements(a1_, 0);//返回指针，指向数组地址
    jsize len = env ->GetArrayLength(a1_);//获取数组长度
    for (int i = 0; i < len; ++i) {
        LOGE("int数组的值为：%d", *(a1+i));
        //改变java中数组的值，如果下面参数3 mode设置为2则改变不了
        *(a1+i) = 666;
    }
    // 参数3：mode
    // 0:  刷新java数组 并 释放c/c++数组
    // 1 = JNI_COMMIT:
    //      只刷新java数组
    // 2 = JNI_ABORT
    //      只释放
    env->ReleaseIntArrayElements(a1_, a1, 0);
    //
    jsize  slen = env->GetArrayLength(a2);
    for (int i = 0; i < slen; ++i) {
        jstring str = static_cast<jstring>(env->GetObjectArrayElement(a2, i));
        const char* s = env->GetStringUTFChars(str,0);
        LOGE("jni获取java字符串数组：%s", s);
        env->ReleaseStringUTFChars(str, s);
    }
    return 3;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_wanglei55_ndk_MainActivity_objectTest(JNIEnv *env, jobject instance, jobject bean,
                                               jstring str_) {
    //
    const char *str = env->GetStringUTFChars(str_, 0);
    LOGE("objectTest: %s",str);
    env->ReleaseStringUTFChars(str_, str);

    //反射方式调用bean中的set/get方法
    jclass beanClass = env->GetObjectClass(bean);//获取class
    //修改属性值
    //jfieldID fieldID = env->GetFieldID(beanClass,"num","I");
    //env->SetIntField(bean,fieldID,444);

    //调用set方法设置
    jmethodID setMethodID = env->GetMethodID(beanClass,"setNum","(I)V");//获取方法信息
    env->CallVoidMethod(bean,setMethodID,999);
    //调用get方法获取
    jmethodID getMethodID = env->GetMethodID(beanClass,"getNum","()I");//获取方法信息
    jint result = env->CallIntMethod(bean,getMethodID);
    LOGE("调用Student中getNum返回值: %d",result);

    //调用静态方法：public static void printMsg(String str)
    jmethodID staticMID = env->GetStaticMethodID(beanClass,"printMsg","(Ljava/lang/String;)V");
    jstring jstring1 = env->NewStringUTF("JNI中的String");
    env->CallStaticVoidMethod(beanClass,staticMID,jstring1);
    env->DeleteLocalRef(jstring1);//释放

    //调用静态方法：public static void printMsg(Card card)
    jmethodID staticMID2 = env->GetStaticMethodID(beanClass,"printMsg","(Lcom/wanglei55/ndk/Card;)V");
    //创建参数Card
    jclass  cardclz = env->FindClass("com/wanglei55/ndk/Card");
    jmethodID constructorID = env->GetMethodID(cardclz,"<init>","(I)V");//<init>表示获取构造方法
    jobject cardObj = env->NewObject(cardclz,constructorID,333);
    env->CallStaticVoidMethod(beanClass,staticMID2,cardObj);
    env->DeleteLocalRef(cardObj);
}

jclass  globalclazz;
jobject globalObj;
jobject weakGlobalObj;

extern "C"
JNIEXPORT void JNICALL
Java_com_wanglei55_ndk_MainActivity_refTest(JNIEnv *env, jobject instance) {

    if (globalclazz == NULL){
        globalclazz = env->FindClass("com/wanglei55/ndk/Card");
    }
    jmethodID constructorID = env->GetMethodID(globalclazz,"<init>","(I)V");//<init>表示获取构造方法
    jobject cardObj = env->NewObject(globalclazz,constructorID,333);
    //与局部引用的创建不同，需要使用指定的JNI函数来创建全局引用。
    //通过NewGlobalRef方法来创建全局引用，通过DeleteGlobalRef来释放全局引用
    globalObj = env->NewGlobalRef(cardObj);

    weakGlobalObj = env->NewWeakGlobalRef(cardObj);
    // 释放弱全局引用
    //env->DeleteWeakGlobalRef(weakGlobalObj);
    //弱引用 ：不会阻止回收
    //问题： 当我们使用弱引用的时候  弱引用 引用的对象可能被回收了
    //使用的时候判断是否被回收了
    //对一个弱引用 与NULL相比较
    // true： 释放了
    // false: 还可以使用这个弱引用
    jboolean isEqual = env->IsSameObject(weakGlobalObj, NULL);

    env->DeleteLocalRef(cardObj);
}

//动态注册/静态注册/线程
void dynamicTest(){
    LOGE("JNI dynamicTest");
}

jint dynamicTest2(jint i){
    LOGE("JNI dynamicTest2:%d",i);
    return 9999;
}



static const JNINativeMethod methods[] = {
        {"dynamicJavaTest","()V",(void*)dynamicTest},
        {"dynamicJavaTest2","(I)I",(int*)dynamicTest2},
};
static const char *mClassName = "com/wanglei55/ndk/MainActivity";

JavaVM *_vm;

int JNI_OnLoad(JavaVM *vm, void *reserved){
    //
    LOGE("JNI_Onload");
    //
    _vm = vm;
    // 获得JNIEnv
    JNIEnv *env = 0;
    // 小于0 失败 ，等于0 成功
    int r = vm->GetEnv((void**)&env,JNI_VERSION_1_4);
    if (r != JNI_OK){
        return -1;
    }
    //获得 class对象
    jclass jcls = env->FindClass(mClassName);
    //动态注册方法
    env->RegisterNatives(jcls,methods, sizeof(methods)/ sizeof(JNINativeMethod));
    return JNI_VERSION_1_4;
}

jobject _instance;

void* threadTask(void* args){
    // native线程 附加 到 Java 虚拟机
    JNIEnv *env;//JNIEnv *是与线程有关的
    jint i = _vm->AttachCurrentThread(&env,0);
    if (i != JNI_OK){
        return nullptr;
    }
    //回调
    //获得MainActivity的class对象
    jclass cls = env->GetObjectClass(_instance);
    jmethodID  updateUI = env->GetMethodID(cls,"callBack","()V");
    env->CallVoidMethod(_instance,updateUI);
    //释放内存
    env->DeleteGlobalRef(_instance);
    //退出线程，释放线程资源
    _vm->DetachCurrentThread();
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_wanglei55_ndk_MainActivity_testThread(JNIEnv *env, jobject instance) {

    pthread_t pid;
    //启动线程
    _instance = env->NewGlobalRef(instance);
    pthread_create(&pid,0,threadTask,0);
}

