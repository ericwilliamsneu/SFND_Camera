#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
#define MAX_KERNEL_LENGTH 50

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void printMatInfo(cv::Mat& matrix, string name)
{
    std::cout << name << ":" << type2str(matrix.type()) << " " << matrix.cols << "x" << matrix.rows << std::endl;
}

void magnitudeSobel()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    if (img.empty())
    {
        printf(" Error opening image\n");
        exit(EXIT_FAILURE);
    }
    printMatInfo(img,"Input");

    string windowName;
    cv::namedWindow(windowName="Input Image",1);
    cv::imshow(windowName="Input Image",img);
    cv::waitKey(0);

    // convert image to grayscale
    cv::Mat src;
    cv::cvtColor(img, src, cv::COLOR_BGR2GRAY);
    printMatInfo(src, "Greyscale");


    // create filter kernels using the cv::Mat datatype both for x and y
    // create filter kernel
    float sobel_x[9] = {-1, 0, +1,
                        -2, 0, +2, 
                        -1, 0, +1};
    cv::Mat kernel_x = cv::Mat(3,3,CV_32F,sobel_x);
    float sobel_y[9] = {-1, -2, -1,
                        0,   0,  0,
                        +1, +2, +1};
    cv::Mat kernel_y = cv::Mat(3,3,CV_32F,sobel_y);

    // apply smoothing using the GaussianBlur() function from the OpenCV
    // ToDo : Add your code here
    //Code adapted from OpenCV documentation
    //https://docs.opencv.org/4.5.3/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html
    //![gaussianblur]
    cv::namedWindow(windowName="Blurred Image",1);
    cv::namedWindow(windowName="Gradient Magnitude",1);
    cv::Mat dst(cv::Mat::zeros(src.size(),src.type()));
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        cv::GaussianBlur( src, dst, cv::Size( i, i ), 0, 0 );
        printMatInfo(dst,"Blurred");
        string caption = "Kernel Size: " + to_string(i);
        cv::putText(dst, caption,
            cv::Point(dst.cols/4, dst.rows/2),
            cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255) );
        cv::imshow(windowName="Blurred Image",dst);
        cv::waitKey(0);

        // apply filter using the OpenCv function filter2D()
        cv::Mat result_x;
        cv::filter2D(dst, result_x, -1, kernel_x, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
        cv::Mat result_y;
        cv::filter2D(dst, result_y, -1, kernel_y, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

        std::cout << "Result X size/type: " << result_x.size() << " / " << type2str(result_x.type()) << std::endl;
        std::cout << "Result Y size/type: " << result_y.size() << " / " << type2str(result_y.type()) << std::endl;  

        // compute magnitude image based on the equation presented in the lesson 
        cv::Mat magnitude(cv::Mat::zeros(dst.size(),dst.type()));
        for(int row = 0; row < result_x.rows; row++)
        {
            for(int col=0; col < result_x.cols; col++)
            {
                auto magnitudeval = std::sqrt(
                    std::pow(result_x.at<unsigned char>(row,col),2) + 
                    std::pow(result_y.at<unsigned char>(row,col),2));
                magnitude.at<unsigned char>(row,col) = magnitudeval;
            }
        }  

        // show result
        caption = "Kernel Size: " + to_string(i);
        cv::putText( magnitude, caption,
            cv::Point( magnitude.cols/4, magnitude.rows/2),
            cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255) );
        cv::imshow(windowName="BlurredImage",magnitude);
        cv::waitKey(0); // wait for keyboard input before continuing
    }
}

int main()
{
    magnitudeSobel();
}