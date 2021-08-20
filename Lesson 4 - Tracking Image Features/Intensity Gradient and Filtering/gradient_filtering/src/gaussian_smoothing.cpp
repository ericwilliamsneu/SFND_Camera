#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void gaussianSmoothing1()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");
    if (img.empty())
    {
        printf(" Error opening image\n");
        exit(EXIT_FAILURE);
    }

    // show result
    string windowName = "Input Image";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, img);
    cv::waitKey(0); // wait for keyboard input before continuing
    
    // create filter kernel
    float gauss_data[25] = {1, 4, 7, 4, 1,
                            4, 16, 26, 16, 4,
                            7, 26, 41, 26, 7,
                            4, 16, 26, 16, 4,
                            1, 4, 7, 4, 1};
    cv::Mat kernel = cv::Mat(5, 5, CV_32F, gauss_data);
    cv::Scalar kernelsum = cv::sum(kernel);
    double kernelfactor = kernelsum[0];
    kernel /= kernelfactor;

    // apply filter
    cv::Mat result;
    cv::filter2D(img, result, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // show result
    windowName = "Manual Gaussian Blurring";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, result);
    cv::waitKey(0); // wait for keyboard input before continuing
}

void gaussianSmoothing()
{
    // load image from file
     // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");
    if (img.empty())
    {
        printf(" Error opening image\n");
        exit(EXIT_FAILURE);
    }

    // show result
    string windowName = "Input Image";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, img);
    cv::waitKey(0); // wait for keyboard input before continuing

    cv::Mat dst;
    cv::GaussianBlur(img,dst,cv::Size(3,3),0,0);
    windowName = "Output Image";
    cv::namedWindow(windowName, 1);
    cv::imshow(windowName, dst);
    cv::waitKey(0);
}

int main()
{
    gaussianSmoothing1();
    gaussianSmoothing();
}