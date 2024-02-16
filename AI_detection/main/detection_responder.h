#ifndef TENSORFLOW_LITE_MICRO_DETECTION_RESPONDER_H_
#define TENSORFLOW_LITE_MICRO_DETECTION_RESPONDER_H_

#include "tensorflow/lite/c/common.h"

void RespondToDetection(float clear_sky_score_f,float patterened_cloud_score_f,float thick_dark_cloud_score_f, 
                      float thick_white_cloud_score_f,float thin_white_cloud_score_f,float veil_cloud_score_f);

#endif  
