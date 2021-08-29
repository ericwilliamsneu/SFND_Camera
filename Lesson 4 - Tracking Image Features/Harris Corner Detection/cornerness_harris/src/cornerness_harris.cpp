#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // visualize input
    string windowName = "Input Image";
    cv::namedWindow(windowName, 1);
    cv::imshow(windowName, img);
    cv::waitKey(0);
    cv::destroyWindow(windowName);

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    std::cout << "DST NORM SCALED - SIZE: " << dst_norm_scaled.size() << " TYPE: " << dst_norm_scaled.type() << std::endl;

    // visualize results
    windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 1);
    cv::imshow(windowName, dst_norm_scaled);
    cv::waitKey(0);
    cv::destroyWindow(windowName);

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.

    vector<cv::KeyPoint> keypoints;

    int window_size = 2*apertureSize;
    int window_dist = floor(window_size / 2);
    int mean = cv::mean(dst_norm).val[0];

    std::cout << "Mean (threshold) of dst_norm: " << mean << std::endl;

    for(int img_row = 0; img_row < dst_norm.rows; img_row++)
    {
        for(int img_col = 0; img_col < dst_norm.cols; img_col++)
        {
            int max_val = 0;
            int cur_val = (int) dst_norm.at<float>(img_row,img_col);

            //std::cout << "Pt(" << img_row << "," << img_col << "): " << cur_val << std::endl;
            
            if(cur_val < minResponse)
            {
                //std::cout << "Point does not meet minimum threshold, zero'd and continuing..." << std::endl; 
                continue;
            }
            
            for(int window_row = (img_row-window_dist); window_row <= (img_row+window_dist); window_row++)
            {
                for(int window_col = (img_col-window_dist); window_col <= (img_col+window_dist); window_col++)
                {
                    int new_val = (int) dst_norm.at<float>(window_row,window_col);
                    if(new_val >= minResponse && new_val > max_val)
                    {
                        max_val = new_val;
                        std::cout << "NewMax(" << window_row << "," << window_col << "): " << max_val << std::endl;
                    }
                }
            }

            if(cur_val == max_val)
            {
                cv::KeyPoint newKeyPoint;
                newKeyPoint.pt = cv::Point2f(img_col, img_row);
                newKeyPoint.size = 2 * apertureSize;
                newKeyPoint.response = cur_val;

                keypoints.push_back(newKeyPoint);

                //std::cout << " - Maximum Detected" << std::endl;
            }
        }
    }

    cout << "Number of keypoints detected: " << keypoints.size() << std::endl;
    cv::Mat dst_keypoints = dst_norm_scaled.clone();
    cv::drawKeypoints(dst_norm_scaled,keypoints,dst_keypoints);

    // visualize NMS Results
    windowName = "NMS Results";
    cv::namedWindow(windowName, 1);
    cv::imshow(windowName, dst_keypoints);
    cv::waitKey(0);
    cv::destroyWindow(windowName);
}

int main()
{
    cornernessHarris();
}