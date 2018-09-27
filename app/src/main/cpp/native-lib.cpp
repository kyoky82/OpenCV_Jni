#include <jni.h>
#include <string>

#include "opencv2/opencv.hpp"

using namespace cv;

extern "C" JNIEXPORT void

JNICALL
Java_com_park_test_CppImageProcessUtils_cppImageProcess(
        JNIEnv *env,
        jobject jobj, jint jw, jint jh, jintArray jPixArr, jint jld) {
    // 将Java数组转换成c/c++数组
    jint *cPixArr = env->GetIntArrayElements(jPixArr, JNI_FALSE);
    if(NULL == cPixArr){
        return;
    }

    // 将c/c++图片转成OpenCV图片
    Mat mat_image_src(jh, jw, CV_8UC4, (unsigned char*)cPixArr);
    // 将4通道Mat转换成3通道Mat，才能进行图像处理
    Mat mat_image_dst;
    cvtColor(mat_image_src, mat_image_dst, CV_BGRA2BGR, 3);

    //进行图像处理，先克隆一张图片
    Mat mat_image_clone = mat_image_dst.clone();
    // 逐个像素分别增加其B/R/G通道值
    for(int i = 0; i < jh; i++){
        for(int j = 0; j < jw; j++){
            // B通道增强
            mat_image_clone.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(mat_image_dst.at<Vec3b>(i, j)[0] + jld);
            // R通道增强
            mat_image_clone.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(mat_image_dst.at<Vec3b>(i, j)[1] + jld);
            // G通道增强
            mat_image_clone.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(mat_image_dst.at<Vec3b>(i, j)[2] + jld);
        }
    }

    // 将修改后的图像赋值给原始Mat-- mat_image_src
    cvtColor(mat_image_clone, mat_image_src, CV_BGR2BGRA, 4);

    // 更新Java数组
    env->ReleaseIntArrayElements(jPixArr, cPixArr, 0);
}
