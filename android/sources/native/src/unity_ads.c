#include "com_unity3d_ads_android_ndk_AndroidNativeBridge.h"
#include "unity_ads.h"
#include <stdio.h>
#include <android/log.h>
#include <stdarg.h>
#include <stdlib.h>

#define APPNAME "UnityAdsJNI"

/*
 * Unity Ads Native SDK bridge
 *
 * (c) Unity Technologies 2013
 */

#ifdef __cplusplus
extern "C" {
#endif

void (*unityads_event_callback)(int, const char*);
static JavaVM *jvm;
unityads_reward_item *reward_items;

const char* __a_unityads_get_reward_image_url(const char* name) {
	JNIEnv *localEnv;
	jclass unityAdsNativeBridge;
	jmethodID methodID;
	jobject strReturn;

	(*jvm)->AttachCurrentThread(jvm, &localEnv, NULL);
	unityAdsNativeBridge = (*localEnv)->FindClass(localEnv, "com/unity3d/ads/android/ndk/AndroidNativeBridge");
	methodID = (*localEnv)->GetStaticMethodID(localEnv, unityAdsNativeBridge, "__getDefaultReward", "()Ljava/lang/String;");
	strReturn = (*localEnv)->CallStaticObjectMethod(localEnv, unityAdsNativeBridge, methodID, (*localEnv)->NewStringUTF(localEnv,name));

	return (*localEnv)->GetStringUTFChars(localEnv, strReturn, 0);
}

void __a_unityads_call_static_method(const char* name, const char* sig, ...) {
	JNIEnv *localEnv;
	jclass unityAdsNativeBridge;
	jmethodID methodID;
	va_list args;
	char buf[50];

	(*jvm)->AttachCurrentThread(jvm, &localEnv, NULL);
	unityAdsNativeBridge = (*localEnv)->FindClass(localEnv, "com/unity3d/ads/android/ndk/AndroidNativeBridge");
	methodID = (*localEnv)->GetStaticMethodID(localEnv, unityAdsNativeBridge, name, sig);
	va_start(args, sig);
	(*localEnv)->CallStaticVoidMethodV(localEnv, unityAdsNativeBridge, methodID, args);
	va_end(args);
}

/**
 * Get the reward items configured
 */
unityads_reward_item* unity_ads_get_reward_items() {
	return reward_items;
}

/**
 * Set the reward item
 */
void unity_ads_set_reward_item(const char* key) {
	JNIEnv *localEnv;
	(*jvm)->AttachCurrentThread(jvm, &localEnv, NULL);

	__a_unityads_call_static_method("__setReward", "(Ljava/lang/String;)V", (*localEnv)->NewStringUTF(localEnv, key));
}

/*
 * Show Unity Ads
 */
void unity_ads_show(int show_offer_screen, int show_animated) {
	__a_unityads_call_static_method("__show", "(ZZ)V", show_offer_screen, show_animated);
}

/* 
 * Init Unity Ads
 */
void unity_ads_init(int game_id, void (*iec)(int, const char*)) {
	unityads_event_callback = iec;
	__a_unityads_call_static_method("__init", "(I)V", game_id);
}

/*
 * Class:     com_unity3d_ads_android_ndk_AndroidNativeBridge
 * Method:    bridgeReady
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_unity3d_ads_android_ndk_AndroidNativeBridge_bridgeReady
  (JNIEnv *env, jobject obj) {
  	unity_ads_debug("Bridge ready");

    int status = (*env)->GetJavaVM(env, &jvm);
    if(status != 0) {
    	// Something went really wrong
    }
 }

/*
 * Dispatch an event from the IUnityAdsListener to the native listener
 *
 * Class:     com_unity3d_ads_android_ndk_AndroidNativeBridge
 * Method:    dispatchEvent
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_unity3d_ads_android_ndk_AndroidNativeBridge_dispatchEvent
  (JNIEnv *env, jobject obj, jint event_id, jstring data) {
  	const char* event_data;

  	if(data != NULL) {
		event_data = (*env)->GetStringUTFChars(env, data, 0);
  	}

  	(*unityads_event_callback)(event_id, event_data);
}

/*
 * Dispatch information about reward items
 * 
 * Class:     com_unity3d_ads_android_ndk_AndroidNativeBridge
 * Method:    setRewardItems
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_unity3d_ads_android_ndk_AndroidNativeBridge_setRewardItems
  (JNIEnv * env, jobject obj, jobjectArray items) {
	JNIEnv *localEnv;
	jclass unityAdsNativeBridge;
	jmethodID methodID;

	int i;

	(*jvm)->AttachCurrentThread(jvm, &localEnv, NULL);
	unityAdsNativeBridge = (*localEnv)->FindClass(localEnv, "com/unity3d/ads/android/ndk/AndroidNativeBridge");

	reward_items = (unityads_reward_item*)malloc(sizeof(unityads_reward_item) * (*env)->GetArrayLength(env, items));

	for(i = 0; i < (*env)->GetArrayLength(env, items); i++) {
		unityads_reward_item r;
		r.reward_name = (*env)->GetStringUTFChars(env, (*env)->GetObjectArrayElement(env, items, i), 0);
		r.reward_image_url = __a_unityads_get_reward_image_url(r.reward_name);
		reward_items[i] = r;
	}
}

static void unity_ads_debug(const char* msg) {
	__android_log_print(ANDROID_LOG_INFO, APPNAME, msg);
}

#ifdef __cplusplus
}
#endif	