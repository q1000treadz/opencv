#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    VideoCapture cap("C:/Users/IVAN/source/repos/Project5/vanya2.mp4");

    if (!cap.isOpened())
    {
        cout << "Error opening video file" << endl;
        return -1;
    }

    Mat frame, gray, gauss, edges, circles;
    std::vector<std::vector<cv::Point>> contours;

    while (true)
    {
        cap.read(frame);

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);


        cv::GaussianBlur(gray, gauss, cv::Size(7, 7), 0);
        cv::Canny(gauss, edges, 40, 250);
        cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);
        cv::Mat contours_draw = cv::Mat::zeros(edges.size(), CV_8UC3);

        for (size_t i = 0; i < contours.size(); i++) {
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);
            size_t vertices = approx.size();

            std::string label;
            if (vertices == 3)
                label = "Triangle";
            else if (vertices == 4)
                label = "Square";
            else
                label = "Circle";

            cv::Scalar color(0, 255, 0);
            cv::drawContours(frame, contours, int(i), color, 2);

            cv::Moments mu = cv::moments(contours[i]);
            cv::Point centroid(mu.m10 / mu.m00, mu.m01 / mu.m00);

            cv::putText(frame, label, centroid, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 2);
        }

        imshow("Video", frame);

        if (waitKey(25) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}