#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <math.h>

#include "structs.h"
#include "config.h"

using namespace cv;

RGB two_points(Point xy,  std::vector<XYRGB> & points);
double distance_between(Point xy1, Point xy2);

int main()
{
    Mat bg_image(bg_dims.y,bg_dims.x, CV_8UC3, Scalar(0,0,0));
    RNG random(time(NULL));

#ifdef GRIDPOINTS
    num_points = grid_dims.x * grid_dims.y;
    std::vector<XYRGB> points(num_points);

    int i = 0;
    for(int x = 0; x < grid_dims.x; x++)
    {
        for(int y = 0; y < grid_dims.y; y++)
        {
            points[i].xy =
            {
                grid_margs.x + (static_cast<float>(x)/(grid_dims.x-1)) * (bg_image.cols - (2 * grid_margs.x)),
                grid_margs.y + (static_cast<float>(y)/(grid_dims.y-1)) * (bg_image.rows - (2 * grid_margs.y)),
            };
            i++;
        }
    }
#else
    std::vector<XYRGB> points(num_points);
    for(auto & i : points)
    {
        i.xy =
        {
            random.uniform(0, bg_image.cols),
            random.uniform(0, bg_image.rows)
        };
    }
#endif // GRIDPOINTS

    for(auto & i : points)
    {
        i.rgb =
        {
            random.uniform(200,255),
            random.uniform(0,90),
            random.uniform(0,90),
        };
    }
#ifdef UNPAR // I am leaving this here for debugging
    for(int x = 0; x < bg_image.cols; x++)
    {
        for(int y = 0; y < bg_image.rows; y++)
        {
            bg_image.at<RGB>({x,y}) = two_points({x,y}, points);
        }
        std::cout << "at row" << x << "\n";
    }
#else
    parallel_for_(Range(0,bg_image.cols * bg_image.rows), [&points, &bg_image](const Range & range)
    {
        for(int i = range.start; i < range.end; i++)
        {
            int x = i % bg_image.cols;
            int y = i / bg_image.cols;
            bg_image.at<RGB>({x,y}) = two_points({x,y}, points);
        }
    });
#endif // UNPAR
    imwrite(filename, bg_image);
}

double distance_between(Point xy1, Point xy2)
{
    return std::sqrt( std::pow(xy1.x - xy2.x, 2) + std::pow(xy1.y - xy2.y, 2) );
}

RGB two_points(Point xy,  std::vector<XYRGB> & points)
{
    struct ind_and_dist
    {
        int index;
        double distance;
    };

    std::vector<ind_and_dist> point_distances;
    point_distances.resize(num_points);

    for(int i = 0; i < num_points; i++)
    {
        point_distances[i].index = i;
        point_distances[i].distance = distance_between(xy, points[i].xy);
    }

// TODO (potato#1#): Replace this with something that isnt really slow.
    std::sort(point_distances.begin(), point_distances.end(), [](ind_and_dist thing1, ind_and_dist thing2)
    {
        return thing1.distance < thing2.distance;
    });

    RGB color = {0,0,0};

    double total_distance = 0;
    for(int i = 0; i < num_average; i++)
    {
        total_distance += point_distances[i].distance;
    }

    for(int i = 0; i < num_average; i ++)
    {
        double dist_adj = (point_distances[i].distance/total_distance);
        RGB point_color = points[point_distances[i].index].rgb;
        for(int j = 0; j < 3; j++)
        {
            color[j] += (point_color[j] * dist_adj);
        }
    }
    return color;

}
