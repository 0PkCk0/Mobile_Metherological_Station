#ifndef TENSORFLOW_LITE_MICRO_MAIN_FUNCTIONS_H_
#define TENSORFLOW_LITE_MICRO_MAIN_FUNCTIONS_H_

// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

// Initialization all the data, ai model
void model_setup_configuration();
void loop();

#ifdef __cplusplus
}
#endif

#endif
