#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    const int height = 750;
    const int width = 750;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    putText(image, "width: " + to_string(image.cols), Point(330, 520), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
    putText(image, "height: " + to_string(image.rows), Point(330, 550), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);

    line(image, Point(400, 90), Point(570, 120), Scalar(255, 255, 0), 3);
    circle(image, Point(image.cols / 2, image.rows / 2), 50, Scalar(0, 204, 255), -1);
    circle(image, Point(530, 350), 50, Scalar(0, 0, 255), 2);
    rectangle(image, Rect(10, image.rows - 150, 150, 150), Scalar(255, 0, 0), -1);
    rectangle(image, Rect(50, 50, 100, 100), Scalar(0, 0, 255), 2);

    imshow("Figures", image);

    Mat segment1 = image(Rect(0, 0, width / 2, width / 2));
    Mat segment2 = image(Rect(width / 2, 0, width / 2, width / 2));
    Mat segment3 = image(Rect(0, height / 2, width / 2, width / 2));
    Mat segment4 = image(Rect(width / 2, height / 2, width / 2, width / 2));

    namedWindow("Segment 1", WINDOW_NORMAL);
    imshow("Segment 1", segment1);

    namedWindow("Segment 2", WINDOW_NORMAL);
    imshow("Segment 2", segment2);

    namedWindow("Segment 3", WINDOW_NORMAL);
    imshow("Segment 3", segment3);

    namedWindow("Segment 4", WINDOW_NORMAL);
    imshow("Segment 4", segment4);

    Mat mask(image.size(), CV_8UC1, Scalar(0));
    circle(mask, Point(350, 380), 130, Scalar(255), -1);

    Mat segmented;
    image.copyTo(segmented, mask);

    namedWindow("Mask", WINDOW_NORMAL);
    imshow("Mask", segmented);

    waitKey(0);

    return 0;
}