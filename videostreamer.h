#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QThread>


static cv::VideoCapture cap;


class VideoStreamer: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cannyThreshold1 READ cannyThreshold1 WRITE setCannyThreshold1 NOTIFY cannyThreshold1Changed FINAL)
    Q_PROPERTY(QString cannyThreshold2 READ cannyThreshold2 WRITE setCannyThreshold2 NOTIFY cannyThreshold2Changed FINAL)
    Q_PROPERTY(bool cannyEnabled READ cannyEnabled WRITE setCannyEnabled NOTIFY cannyEnabledChanged FINAL)
public:
    VideoStreamer();
    ~VideoStreamer();

public:
    QThread* threadStreamer = new QThread();
    void catchFrame(cv::Mat emittedFrame);

    QString cannyThreshold1() const;
    QString cannyThreshold2() const;

    bool cannyEnabled() const;


public slots:
    void openVideoCamera(QString path);

    void streamerThreadSlot();

    void setCannyThreshold1(const QString &newCannyThreshold1);
    void setCannyThreshold2(const QString &newCannyThreshold2);
    void setCannyEnabled(bool newCannyEnabled);

private:
    cv::Mat frame;
    QTimer tUpdate;

    QString m_cannyThreshold1 = "40";
    QString m_cannyThreshold2 = "100";

    bool m_cannyEnabled;

signals:
    void newImage(QImage &);

    void emitThreadImage(cv::Mat frameThread);

    void emitQImage(const QImage&);

    void cannyThreshold1Changed(QString cannyThreshold1);
    void cannyThreshold2Changed(QString cannyThreshold2);
    void cannyEnabledChanged(bool cannyEnabled);
};

#endif // VIDEOSTREAMER_H
