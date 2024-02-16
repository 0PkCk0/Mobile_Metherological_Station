#ifndef TENSORFLOW_LITE_MICRO_MODEL_SETTINGS_H_
#define TENSORFLOW_LITE_MICRO_MODEL_SETTINGS_H_

constexpr int kNumCols = 96;
constexpr int kNumRows = 96;
constexpr int kNumChannels = 3;

constexpr int kMaxImageSize = kNumCols * kNumRows * kNumChannels;

//Cloudy,Sunny,Rainy,Snowy,Foggy
constexpr int kCategoryCount = 6;
constexpr int Clear_Sky = 0;
constexpr int Patterened_Cloud = 1;
constexpr int Thick_Dark_Cloud = 2;
constexpr int Thick_White_Cloud = 3;
constexpr int Thin_White_Cloud = 4;
constexpr int Veil_Cloud = 5;
extern const char* kCategoryLabels[kCategoryCount];

#endif 
