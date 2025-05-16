#pragma once

#include <QThread>
#include <QLabel>

class MJPEGViewer : public QThread {
    Q_OBJECT
public:
    MJPEGViewer(const QString &url, QLabel *display, QObject *parent = nullptr);
    void run() override;

private:
    QString streamUrl;
    QLabel *displayLabel;
};
