#include "detection_responder.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "image_provider.h"
#include "bsp/esp-bsp.h"

#include "driver/temperature_sensor.h"
#include "wifi.h"

#define IMG_WD (96 * 2)
#define IMG_HT (96 * 2)

static lv_obj_t *camera_canvas = NULL;
static lv_obj_t *label2 = NULL;
static lv_obj_t *label = NULL;
#define DISPLAY_SUPPORT 0

static void create_gui(void)
{
  bsp_display_start();
  bsp_display_backlight_on(); // Set display brightness to 100%
  bsp_display_lock(0);
  camera_canvas = lv_canvas_create(lv_scr_act());
  assert(camera_canvas);
  lv_obj_align(camera_canvas, LV_ALIGN_TOP_MID, 0, 0);

  label = lv_label_create(lv_scr_act());
  assert(label);
  lv_obj_align(label, LV_ALIGN_BOTTOM_MID,-10, 0);
  /*
  label2 = lv_label_create(lv_scr_act());
  assert(label2);
  lv_obj_align_to(label2,label,LV_ALIGN_OUT_RIGHT_MID, 20, 0);
  */
  bsp_display_unlock();
}
//#endif // DISPLAY_SUPPORT

void RespondToDetection(float clear_sky_score_f,float patterened_cloud_score_f,float thick_dark_cloud_score_f, 
                      float thick_white_cloud_score_f,float thin_white_cloud_score_f,float veil_cloud_score_f) {
  char myString[50];
  int clear_sky_score_int = (clear_sky_score_f) * 100 + 0.5;
  int patterened_cloud_score_int = (patterened_cloud_score_f) * 100 + 0.5;
  int thick_dark_cloud_score_int = (thick_dark_cloud_score_f) * 100 + 0.5;
  int thick_white_cloud_score_int = (thick_white_cloud_score_f) * 100 + 0.5;
  int thin_white_cloud_score_int = (thin_white_cloud_score_f) * 100 + 0.5;
  int veil_cloud_score_int = (veil_cloud_score_f) * 100 + 0.5;
#if DISPLAY_SUPPORT
    if (!camera_canvas) {
       create_gui();
    }
#endif
    //finding the biggest score 
    int max_score = clear_sky_score_int;
    uint8_t return_value = 0;
    if (patterened_cloud_score_int > max_score) {
      max_score = patterened_cloud_score_int;
    }
    if (thick_dark_cloud_score_int > max_score) {
        max_score = thick_dark_cloud_score_int;
    }
    if (thick_white_cloud_score_int > max_score) {
        max_score = thick_white_cloud_score_int;
    }
    if (thin_white_cloud_score_int > max_score) {
        max_score = thin_white_cloud_score_int;
    }
    if (veil_cloud_score_int > max_score) {
        max_score = veil_cloud_score_int;
    }
 
    if(max_score == clear_sky_score_int){
      
      //lv_label_set_text(label, "clear_sky");
      tsd.Filed_Classification = 0;
      }
    if(max_score == patterened_cloud_score_int){
      //lv_label_set_text(label, "patterened_cloud");
      tsd.Filed_Classification = 1;
      }
    if(max_score == thick_dark_cloud_score_int){
      //lv_label_set_text(label, "thick_dark_cloud");
      tsd.Filed_Classification = 2;
      }
    if(max_score == thick_white_cloud_score_int){
      //lv_label_set_text(label, "thick_white_cloud");
      tsd.Filed_Classification = 3;
      }
    if(max_score == thin_white_cloud_score_int){
      //lv_label_set_text(label, "thin_white_cloud");
      tsd.Filed_Classification = 4;
      }
    if(max_score == veil_cloud_score_int){
      //lv_label_set_text(label, "veil_cloud");
      tsd.Filed_Classification = 5;
      }
    
    //BLOCK FOR TEMPERATURE MESURE
    /*
    temperature_sensor_handle_t temp_handle = NULL;
    temperature_sensor_enable(temp_handle);
    float tsens_out;
    temperature_sensor_get_celsius(temp_handle, &tsens_out);
    
    sprintf(myString, "temp: %f", tsens_out);
    lv_label_set_text(label2, myString);
    */
  #if DISPLAY_SUPPORT
   uint16_t *buf = (uint16_t *) image_provider_get_display_buf();
    bsp_display_lock(0);
    lv_canvas_set_buffer(camera_canvas, buf, IMG_WD, IMG_HT, LV_IMG_CF_TRUE_COLOR);
    bsp_display_unlock();
  #endif
    MicroPrintf("clear sky score:%d%%, patterened cloud score %d%%, thick dark cloud score:%d%% ,thick white cloud score:%d%%, thin white cloud score %d%%, veil cloud score:%d%%",
              clear_sky_score_int, patterened_cloud_score_int, thick_dark_cloud_score_int, thick_white_cloud_score_int, 
              thin_white_cloud_score_int, veil_cloud_score_int );
    
}