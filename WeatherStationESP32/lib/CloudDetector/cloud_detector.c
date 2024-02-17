#include "cloud_detector.h"

SkySplitter sspl;

//Convert a pixel from RGB888 to HSV space color
HSV rgb2hsv(RGB rgb888)
{
    HSV hsv;
    float r = rgb888.r / 255.0f;
    float g = rgb888.g / 255.0f;
    float b = rgb888.b / 255.0f;

    float max = (r > g) ? (r > b ? r : b) : (g > b ? g : b);
    float min = (r < g) ? (r < b ? r : b) : (g < b ? g : b);
    float delta = max - min;

    if (delta == 0) {
        hsv.h = 0;
    } else if (max == r) {
        hsv.h = 60 * ((int)((g - b) / delta) % 6);
    } else if (max == g) {
        hsv.h = 60 * (((b - r) / delta) + 2);
    } else {
        hsv.h = 60 * (((r - g) / delta) + 4);
    }
    hsv.h = hsv.h/2;

    hsv.s = max == 0 ? 0 : (delta / max) * 255;

    hsv.v = max * 255;

    return hsv;
}

//Check if a pixel is into the HSV color range
bool pixel_analysis(HSV hsv, int lowH, int highH, int lowS, int highS, int lowV, int highV){
    //printf("h=%.2f s=%.2f v=%.2f", hsv.h, hsv.s, hsv.v);
    if(hsv.h>=lowH && hsv.h<=highH && hsv.s>=lowS && hsv.s<=highS && hsv.v>=lowV && hsv.v<=highV){
        return true;
    }else{
        return false;
    }
}

//Convert each pixel of the image from RGB565->RGB888->HSV and classifie it as cloud/sun/sky/error
void pic_analysis(camera_fb_t *pic){
    RGB rgb;
    sspl.cloud=0;
    sspl.sun=0;
    sspl.sky=0;
    sspl.error=0;
    int count=0;
    for (int i = 0; i < pic->len; i += 2) {
        if(i%15000==0){
            vTaskDelay(100/portTICK_RATE_MS);
        }
        count++;
        // Prendi un pixel dall'immagine
        uint16_t rgb565 = ((uint16_t)pic->buf[i] << 8) | pic->buf[i + 1];
        //printf("rgb565=%d", rgb565);
        uint8_t r = (rgb565 >> 11) & 0x1F;
        uint8_t g = (rgb565 >> 5) & 0x3F;
        uint8_t b = rgb565 & 0x1F;
        //printf("r5=%d g5=%d b5=%d\n", r, g, b);
        rgb.r = (r * 527 + 15) >> 6;
        rgb.g = (g * 259 + 33) >> 6;
        rgb.b = (b * 527 + 15) >> 6;
        //printf("r=%d g=%d b=%d \n", rgb.r, rgb.g, rgb.b);
        // Converte il pixel da RGB a HSV
        HSV hsv = rgb2hsv(rgb);
        //printf("h=%d s=%d v=%d \n", hsv.h, hsv.s, hsv.v);
        if(pixel_analysis(hsv, 0, 179, 0, 0, 0, 255)){
            sspl.sun++;
        }else{
            if(pixel_analysis(hsv, 0, 179, 0, 50, 230, 255)){
                sspl.cloud++;
            }else{
                if(pixel_analysis(hsv, 70, 110, 0, 110, 60, 160)){
                    sspl.sky++;
                }else{
                    if(pixel_analysis(hsv, 76, 110, 0, 110, 160, 255)){
                        sspl.sky++;
                    }else{
                        if(pixel_analysis(hsv, 45, 60, 0, 50, 120, 240)){
                            sspl.cloud++;
                        }else{
                            if(pixel_analysis(hsv, 60, 75, 0, 80, 90, 240)){
                                sspl.cloud++;
                            }else{
                                if(pixel_analysis(hsv, 0, 179, 0, 5, 0, 255)){
                                    sspl.sun++;
                                }else{
                                    if(pixel_analysis(hsv, 0, 255, 0, 255, 0, 255)){
                                        sspl.error++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    printf("count: %d\n", count);
    sspl.p_sun = (float)(sspl.sun*100)/(float)(pic->width*pic->height);
    printf("sun = %ldpx p = %.2f \n", sspl.sun, sspl.p_sun);
    sspl.p_sky = (float)(sspl.sky*100)/(float)(pic->width*pic->height);
    printf("sky = %ld px p = %.2f \n", sspl.sky, sspl.p_sky);
    sspl.p_cloud = (float)(sspl.cloud*100)/(float)(pic->width*pic->height);
    printf("cloud = %ld px p = %.2f \n", sspl.cloud, sspl.p_cloud);
    sspl.p_error = (float)(sspl.error*100)/(float)(pic->width*pic->height);
    printf("error = %ld px p = %.2f \n", sspl.error, sspl.p_error);
}