#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
cv::CascadeClassifier faceCascade, eyesCascade, smileCascade;

auto editFrame(cv::Mat frame) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces, eyes, smiles;
    faceCascade.detectMultiScale(gray, faces, 2, 3, 0, cv::Size(20, 20));

    for (const auto& face : faces) {
        cv::rectangle(frame, face, cv::Scalar(255, 180, 0), 2);

        eyesCascade.detectMultiScale(gray(face), eyes, 3, 2, 0, cv::Size(5, 5));
        for (const auto& eye : eyes) {
            cv::Point center(eye.x + eye.width / 2, eye.y + eye.height / 2);
            int radius = cvRound((eye.width + eye.height) * 0.25);
            cv::circle(frame(face), center, radius, cv::Scalar(200, 200, 0), 2);
        }

        smileCascade.detectMultiScale(gray(face), smiles, 3, 35, 0, cv::Size(5, 5));
        for (const auto& smile : smiles) {
            cv::rectangle(frame(face), smile, cv::Scalar(255, 0, 0), 2);
        }

        cv::blur(frame(face), frame(face), cv::Size(3, 3));
    }

    return frame;
}
int main() {

   
    faceCascade.load("C:/Users/IVAN/clibraries/opencv/build/etc/haarcascades/haarcascade_frontalface_alt.xml");
    eyesCascade.load("C:/Users/IVAN/clibraries/opencv/build/etc/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
    smileCascade.load("C:/Users/IVAN/clibraries/opencv/build/etc/haarcascades/haarcascade_smile.xml");

    cv::VideoCapture cap("C:/Users/IVAN/source/repos/Project5/ZUA.mp4");

    if (!cap.isOpened()) {
        std::cout << "Error" << std::endl;
        return -1;
    }

    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    std::vector<cv::Mat> processedFrames;
    auto start = std::chrono::steady_clock::now();
    cv::Mat frame, gray;

    while (cap.read(frame)) {
        
        processedFrames.push_back(editFrame(frame).clone());
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time: " << elapsed_seconds.count() << "s" << std::endl;

    cap.release();

    cv::VideoWriter video("result.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(frame_width, frame_height));
    for (const auto& frame : processedFrames) {
        video.write(frame);
        cv::imshow("Face", frame);

        if (cv::waitKey(10) == 'q') {
            break;
        }
    }
    video.release();
    cv::destroyAllWindows();

    return 0;
}