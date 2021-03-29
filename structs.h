#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
#include <opencv2/core/types.hpp>

typedef std::array<uchar,3> RGB;

struct XYRGB {
    cv::Point xy;
    RGB rgb;
};

#endif // STRUCTS_H_INCLUDED
