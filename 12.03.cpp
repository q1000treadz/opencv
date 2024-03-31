#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>



void showColoredImage(const cv::Mat& image, const std::string& windowName, int color) {
    cv::Mat convertedImage;
    cv::cvtColor(image, convertedImage, color);
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);
    cv::imshow(windowName, convertedImage);
}

int main() {
    cv::Mat image = cv::imread("C:/Users/IVAN/Documents/photo3.jpg");
    if (image.empty()) {
        std::cout << "Error image" << std::endl;
        return -1;
    }
    cv::namedWindow("RGB", cv::WINDOW_NORMAL);
    imshow("RGB", image);

    showColoredImage(image, "HSV", cv::COLOR_BGR2HSV);
    showColoredImage(image, "LAB", cv::COLOR_BGR2Lab);
    showColoredImage(image, "YUV", cv::COLOR_BGR2YUV);
    showColoredImage(image, "XYZ", cv::COLOR_BGR2XYZ);
    showColoredImage(image, "GRAY", cv::COLOR_BGR2GRAY);

    cv::Mat GRAYImage;
    cvtColor(image, GRAYImage, cv::COLOR_BGR2GRAY);
    cv::Mat edges;
    Canny(GRAYImage, edges, 50, 200);
    cv::imshow("Canny Image", edges);
    std::vector<cv::Vec4i> lines;
    HoughLinesP(edges, lines, 1, CV_PI / 180, 150, 10, 250);
    for (size_t i = 0; i < lines.size(); i++) {
        cv::Vec4i l = lines[i];
        line(image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
    }

    std::vector<cv::Vec3f>  circles;
    HoughCircles(edges, circles, cv::HOUGH_GRADIENT, 1, 750, 200, 10, 10, 100);
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(image, center, radius, cv::Scalar(0, 255, 0), 2, 8, 0);
    }
    cv::imshow("Final image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}