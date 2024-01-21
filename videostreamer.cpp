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

    if(m_cannyEnabled)
    {
        cv::Mat cannyOut;
        cv::Canny(emittedFrame, cannyOut, m_cannyThreshold1.toDouble(), m_cannyThreshold2.toDouble());
        QImage imgIn= QImage((uchar*) cannyOut.data, cannyOut.cols, cannyOut.rows, cannyOut.step, QImage::Format_Grayscale8).rgbSwapped();
        emit emitQImage(imgIn);
    }
    else
    {
        QImage imgIn= QImage((uchar*) emittedFrame.data, emittedFrame.cols, emittedFrame.rows, emittedFrame.step, QImage::Format_RGB888).rgbSwapped();
        emit emitQImage(imgIn);
    }

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
