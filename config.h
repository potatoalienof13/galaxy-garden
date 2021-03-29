#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <opencv2/core/types.hpp>

cv::Point bg_dims {1920,1080};

int num_points = 10;


//The number of points used for the weighted average taken
constexpr int num_average = 2;

std::string filename = "default_file_name.png";

//Instead of arranging the points randomly, organize into a grid that is grid_dims.x wide, and grid_dims.y tall,
//and with margins around
//#define GRIDPOINTS
#ifdef GRIDPOINTS
cv::Point grid_dims {4,4};
cv::Point grid_margs {0,50};
#endif // GRIDPOINTS

//This unparrallelizes the drawing onto the image.
//I am leaving it here because its annoying to debug without it.
//#define UNPAR


#endif // CONFIG_H_INCLUDED
