#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
using namespace std;
using namespace cv;

int main() {

    string imagePath = "C:/Users/IVAN/source/repos/Project5/Project5/";

    Mat testImage = imread(imagePath + string("test2.png"));

    vector<string> cardPaths = {
        "diamonds_king",
        "hearts_ace",
        "hearts_4",
        "hearts_2",
        "spades_3",
        "spades_5",
        "diamonds_7",
        "clubs_6",
        "clubs_8",
        "diamonds_9",
        "diamonds_10",
        "spades_king",
        "hearts_jack",
        "hearts_king"
    };

    Mat cardImage;

    vector<Mat> cardImages, cardDetector;
    vector<string> cardNames;
    vector<vector<KeyPoint>> cardKeypoints;
    for (auto& cardPath : cardPaths) {
        cardImage = imread(imagePath + cardPath + string(".png"));
        resize(cardImage, cardImage, Size(234, 333));
        cardImages.push_back(cardImage);
        cardNames.push_back(cardPath);
    }

    Ptr<ORB> ORBdetector = ORB::create();

    for (int i = 0; i < cardImages.size(); i++) {
        Mat detector;
        vector<KeyPoint> keypoints;
        ORBdetector->detectAndCompute(cardImages[i], noArray(), keypoints, detector);
        cardKeypoints.push_back(keypoints);
        cardDetector.push_back(detector);
    }

    vector<vector<Point>> contours;
    vector<Vec4i> detectedContours;

    Mat blurredImage, thresholdedImage, edgeImage;
    GaussianBlur(testImage, blurredImage, Size(15, 15), 3);
    Canny(blurredImage, edgeImage, 120, 120);
    findContours(edgeImage, contours, detectedContours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        vector<Point> contourPolyDP;

        approxPolyDP(contour, contourPolyDP, 1, true);
        RotatedRect cardRect = minAreaRect(contourPolyDP);

        if (cardRect.size.width < 100 || cardRect.size.height < 100) {
            continue;
        }

        Mat card, result;
        string cardName;
        Mat rotationMatrix, rotatedImage;
        rotationMatrix = getRotationMatrix2D(cardRect.center, cardRect.angle, 1.0);
        warpAffine(testImage, rotatedImage, rotationMatrix, testImage.size(), INTER_CUBIC);
        getRectSubPix(rotatedImage, cardRect.size, cardRect.center, card);

        rotate(card, result, ROTATE_180);

        if (result.size[0] < result.size[1]) {
            rotate(result, result, ROTATE_90_CLOCKWISE);
        }

        Mat cardDescriptor;
        vector<KeyPoint> cardKeypoints;
        ORBdetector->detectAndCompute(card, noArray(), cardKeypoints, cardDescriptor);

        if (cardDescriptor.empty()) {
            cardName = "";
            continue;
        }
        int mindex = -1, mcount = 0;

        Ptr<BFMatcher> matcher = BFMatcher::create();
        for (int i = 0; i < cardImages.size(); i++) {
            if (cardDetector[i].empty()) {
                continue;
            }
            vector<vector<DMatch>> knnMatches;
            matcher->knnMatch(cardDetector[i], cardDescriptor, knnMatches, 3);
            vector<DMatch> correctMatches;
            for (size_t i = 0; i < knnMatches.size(); i++) {
                if (knnMatches[i][0].distance < 0.5 * knnMatches[i][1].distance) {
                    correctMatches.push_back(knnMatches[i][0]);
                }
            }

            if (mcount < correctMatches.size()) {
                mcount = correctMatches.size();
                mindex = i;
            }
        }

        if (mindex != -1) {
            Point2f rectPoints[4];
            cardRect.points(rectPoints);

            for (int j = 0; j < 4; j++) {
                line(testImage, rectPoints[j], rectPoints[(j + 1) % 4], Scalar(0, 0, 255), 4, LINE_AA);
            }
            putText(testImage, cardNames[mindex], cv::Point(50, 50), 1, 2, Scalar(0, 0, 0), 2);
        }
    }

    imshow("card detector", testImage);
    waitKey(0);
}