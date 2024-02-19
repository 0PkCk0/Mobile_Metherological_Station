#include "main_functions.h"

#include "image_provider.h"
#include "model_settings.h"
#include "weather_detection_model.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_op_resolver.h"
#include "detection_responder.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_task_wdt.h"


#define REAL_TIME_DETECTION 1
#define COLLECT_CPU_STATS 1

namespace {
const tflite::Model* model = nullptr; //pointer to the model
tflite::MicroInterpreter* interpreter = nullptr; 
TfLiteTensor* input = nullptr;

// this memory is used to allocate the entire model, the input and output arrays
constexpr int kTensorArenaSize = 124 * 1024 ;
static uint8_t *tensor_arena;
esp_task_wdt_config_t struct_for_watchdog;
static tflite::MicroMutableOpResolver<6> micro_op_resolver; 
}  



void static_model_setup_configuration() {
  //to set REAL_TIME_DETECTION = 0 if WE want to change the watchdog timer
  #ifndef REAL_TIME_DETECTION
  struct_for_watchdog.timeout_ms = 10000;
  esp_task_wdt_reconfigure(&struct_for_watchdog);
  #endif

  model = tflite::GetModel(weather_detect_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf("Model provided is schema version %d not equal to supported ""version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  //This variable is important to be set correctly, each ai model has some layer/a structure
  //and we need to select the resolver that the model is based on

  if (micro_op_resolver.AddConv2D() != kTfLiteOk) {
    MicroPrintf("AddConv2D() failed");
  }
  if (micro_op_resolver.AddFullyConnected() != kTfLiteOk) {
    MicroPrintf("AddFullyConnceted() failed");
  }
  if (micro_op_resolver.AddSoftmax() != kTfLiteOk) {
    MicroPrintf("AddSoftmax() failed");
  }
  if ( micro_op_resolver.AddDepthwiseConv2D()!= kTfLiteOk) {
    MicroPrintf("AddDepthwiseConv2D() failed");
  } 
  if ( micro_op_resolver.AddPad()!= kTfLiteOk) {
    MicroPrintf("AddPad() failed");
  }
  if ( micro_op_resolver.AddMean()!= kTfLiteOk) {
    MicroPrintf("AddMean() failed");
  } 

}

void dinamic_model_configuration(){
  if (tensor_arena == NULL) {
    tensor_arena = (uint8_t *)heap_caps_malloc(kTensorArenaSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  }
  if (tensor_arena == NULL) {
    printf("Couldn't allocate memory of %d bytes\n", kTensorArenaSize);
    return;
  }
    // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    MicroPrintf("AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);

  // Initialize Camera
  TfLiteStatus init_status = InitCamera();
  if (init_status != kTfLiteOk) {
    MicroPrintf("InitCamera failed\n");
    return;
  }

}

//the function must return the label of the image
void inference() {
  // Get image from provider.
  if (kTfLiteOk != GetImage(kNumCols, kNumRows, kNumChannels, input->data.int8)) {
    MicroPrintf("Image capture failed.");
  }

  // Run the model on this input and make sure it succeeds.
  if (kTfLiteOk != interpreter->Invoke()) {
    MicroPrintf("Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);

  // Process the inference results.
  int8_t clear_sky = output->data.uint8[Clear_Sky];
  int8_t patterened_cloud = output->data.uint8[Patterened_Cloud];
  int8_t thick_dark_cloud = output->data.uint8[Thick_Dark_Cloud];
  int8_t thick_white_cloud = output->data.uint8[Thick_White_Cloud];
  int8_t thin_white_cloud = output->data.uint8[Thin_White_Cloud];
  int8_t veil_cloud = output->data.uint8[Veil_Cloud];

\
  float clear_sky_score_f =
      (clear_sky - output->params.zero_point) * output->params.scale;
  float patterened_cloud_score_f =
      (patterened_cloud - output->params.zero_point) * output->params.scale;
  float thick_dark_cloud_score_f =
      (thick_dark_cloud - output->params.zero_point) * output->params.scale;
  float thick_white_cloud_score_f =
      (thick_white_cloud - output->params.zero_point) * output->params.scale;
  float thin_white_cloud_score_f =
      (thin_white_cloud - output->params.zero_point) * output->params.scale;
  float veil_cloud_score_f =
      (veil_cloud - output->params.zero_point) * output->params.scale;

  // Respond to detection
  RespondToDetection(clear_sky_score_f, patterened_cloud_score_f, thick_dark_cloud_score_f, 
                      thick_white_cloud_score_f, thin_white_cloud_score_f, veil_cloud_score_f);

  //vTaskDelay(5); // to avoid watchdog trigger
}

void free_dma_buffer() {
    // Free memory
    heap_caps_free(tensor_arena);
    DeinitCamera();
}

#if defined(COLLECT_CPU_STATS)
  long long total_time = 0;
  long long start_time = 0;
  extern long long softmax_total_time;
  extern long long dc_total_time;
  extern long long conv_total_time;
  extern long long fc_total_time;
  extern long long pooling_total_time;
  extern long long add_total_time;
  extern long long mul_total_time;
#endif
