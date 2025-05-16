

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendCommand();
    void updateFrame();  // 新增：视频帧更新函数

private:
    QLabel *videoLabel;
    QLineEdit *cmdInput;
    QPushButton *sendButton;
    QTcpSocket *socket;
    cv::VideoCapture cap; // 使用 OpenCV 接收视频
};
