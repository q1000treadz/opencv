#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <iostream>



int main(int argc, char* argv[]) {
    cv::Mat image,gray, blur, edges;
    std::vector<std::vector<cv::Point>> contours;
    image = cv::imread("C:/Users/IVAN/Documents/data.png", cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Error file" << std::endl;
        return -1;
    }

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);


    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);

    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Gray", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);

    cv::imshow("Image", image);
    cv::imshow("Gray", gray);
    
    cv::Canny(blur, edges, 0, 0, 3);
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
        cv::drawContours(contours_draw, contours, int(i), color, 2);

        cv::Moments mu = cv::moments(contours[i]);
        cv::Point centroid(mu.m10 / mu.m00, mu.m01 / mu.m00);

        cv::putText(contours_draw, label, centroid, cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 2);
    }

    cv::imshow("Canny", edges);
    cv::imshow("Contours", contours_draw);

    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}