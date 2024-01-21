#include "videostreamer.h"

VideoStreamer::VideoStreamer()
{

}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    tUpdate.stop();
    threadStreamer->requestInterruption();
}



void VideoStreamer::catchFrame(cv::Mat emittedFrame)
{
    pipeline.clear();

    QImage::Format imageFormat = QImage::Format_RGB888;

    if (m_cannyEnabled) {
        pipeline.push_back([this](const cv::Mat& frame) { return this->applyCanny(frame); });
        imageFormat = QImage::Format_Grayscale8;
    }
    if (m_flipEnabled) {
        pipeline.push_back([this](const cv::Mat& frame) { return this->applyFlip(frame); });
    }

    for (auto& func : pipeline) {
        emittedFrame = func(emittedFrame);
    }

    QImage imgIn = QImage((uchar*) emittedFrame.data, emittedFrame.cols, emittedFrame.rows, emittedFrame.step, imageFormat).rgbSwapped();
    emit emitQImage(imgIn);
}

void VideoStreamer::openVideoCamera(QString path)
{
    if(path.length() == 1)
        cap.open(path.toInt());
    else
        cap.open(path.toStdString());

    VideoStreamer* worker = new VideoStreamer();
    worker->moveToThread(threadStreamer);
    QObject::connect(threadStreamer,SIGNAL(started()),worker,SLOT(streamerThreadSlot()));
    QObject::connect(worker,&VideoStreamer::emitThreadImage,this,&VideoStreamer::catchFrame);
    threadStreamer->start();


}

void VideoStreamer::streamerThreadSlot()
{
    cv::Mat tempFrame;


    while (1) {
        cap>>tempFrame;

        if(tempFrame.data)
        {
            emit emitThreadImage(tempFrame);

        }

        if(QThread::currentThread()->isInterruptionRequested())
        {
            cap.release();
            return;
        }

    }
}

QString VideoStreamer::cannyThreshold1() const
{
    return m_cannyThreshold1;
}

void VideoStreamer::setCannyThreshold1(const QString &newCannyThreshold1)
{
    if (m_cannyThreshold1 == newCannyThreshold1)
        return;
    m_cannyThreshold1 = newCannyThreshold1;
    emit cannyThreshold1Changed(m_cannyThreshold1);
}

QString VideoStreamer::cannyThreshold2() const
{
    return m_cannyThreshold2;
}

void VideoStreamer::setCannyThreshold2(const QString &newCannyThreshold2)
{
    if (m_cannyThreshold2 == newCannyThreshold2)
        return;
    m_cannyThreshold2 = newCannyThreshold2;
    emit cannyThreshold2Changed(m_cannyThreshold2);
}

bool VideoStreamer::cannyEnabled() const
{
    return m_cannyEnabled;
}

void VideoStreamer::setCannyEnabled(bool newCannyEnabled)
{
    if (m_cannyEnabled == newCannyEnabled)
        return;
    m_cannyEnabled = newCannyEnabled;
    emit cannyEnabledChanged(m_cannyEnabled);
}

bool VideoStreamer::flipEnabled() const
{
    return m_flipEnabled;
}

void VideoStreamer::setFlipEnabled(bool newFlipEnabled)
{
    if (m_flipEnabled == newFlipEnabled)
        return;
    m_flipEnabled = newFlipEnabled;
    emit flipEnabledChanged(m_flipEnabled);
}

cv::Mat VideoStreamer::applyCanny(const cv::Mat& frame)
{
    cv::Mat cannyOut;
    cv::Canny(frame, cannyOut, m_cannyThreshold1.toDouble(), m_cannyThreshold2.toDouble());
    return cannyOut;
}

cv::Mat VideoStreamer::applyFlip(const cv::Mat& frame)
{
    cv::Mat flipOut;
    cv::flip(frame, flipOut, 1);
    return flipOut;
}
