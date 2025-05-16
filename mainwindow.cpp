#include "mainwindow.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this); //创建一个中心部件central
    QVBoxLayout *layout = new QVBoxLayout(); //用垂直布局layout管理界面元素

    videoLabel = new QLabel("等待视频流..."); //videoLabel 用于显示视频帧
    videoLabel->setFixedSize(640, 480);//设置界面为固定大小

    cmdInput = new QLineEdit();//文本框，用于输入控制命令
    sendButton = new QPushButton("发送命令");//按钮

    //把三个控件添加到布局中，再将该布局设置为窗口的中央内容区
    layout->addWidget(videoLabel);
    layout->addWidget(cmdInput);
    layout->addWidget(sendButton);
    central->setLayout(layout);
    setCentralWidget(central);

    // 建立 TCP 控制连接
    socket = new QTcpSocket(this);//初始化一个 TCP 客户端 socket
    socket->connectToHost("192.168.74.184", 9000);  // 连接到 Jetson 端 TCP 服务器（IP: 192.168.74.184，端口: 9000）

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendCommand);//建立信号槽连接：点击按钮时调用 sendCommand() 函数

    cap.open("rtsp://192.168.74.184:8554/test", cv::CAP_FFMPEG);//打开 Jetson 上的 RTSP 视频流，使用 FFMPEG 解码器

    if (!cap.isOpened()) {
        qDebug("视频流打开失败！");
    }

    // 定时读取视频帧
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(33); // 每 33ms 一帧 ≈ 30fps
}

//析构函数
MainWindow::~MainWindow() {
    socket->disconnectFromHost();// 断开 TCP
    cap.release(); // 释放视频流
}

void MainWindow::sendCommand() {
    QString cmd = cmdInput->text(); // 读取命令
    socket->write(cmd.toUtf8()); // 转为 UTF-8 后通过 TCP 发送
}

void MainWindow::updateFrame() {
    cv::Mat frame;
    if (cap.read(frame) && !frame.empty()) {//从 VideoCapture 读取一帧图像
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);//转换色彩空间为 RGB（OpenCV 默认是 BGR）
        QImage image((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);//将 cv::Mat 封装为 QImage
        videoLabel->setPixmap(QPixmap::fromImage(image).scaled(videoLabel->size(), Qt::KeepAspectRatio));//缩放并显示在 videoLabel 上
    }
}
