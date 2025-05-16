#include "mjpegviewer.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>

MJPEGViewer::MJPEGViewer(const QString &url, QLabel *display, QObject *parent)
    : QThread(parent), streamUrl(url), displayLabel(display) {}

void MJPEGViewer::run() {
    cv::VideoCapture cap(streamUrl.toStdString());
    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (!frame.empty()) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            QPixmap pix = QPixmap::fromImage(img);
            displayLabel->setPixmap(pix.scaled(displayLabel->size(), Qt::KeepAspectRatio));
        }
        QThread::msleep(30);
    }
}
