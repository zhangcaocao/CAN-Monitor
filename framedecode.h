#ifndef FRAMEDECODE_H
#define FRAMEDECODE_H

#include <QByteArray>
#include <QVector>

class FrameDecode
{
public:
    FrameDecode();
    ~FrameDecode();
    struct CANFrame{
        char ID;
        char status;
        char DLC;
        QString value;
    };




    bool FrameDecode_p(CANFrame *data, char byte);
    int GetData(CANFrame *data, char byte);
    QVector<CANFrame> frameDecode(const QByteArray &array);

private:
    int m_status;
    uint32_t m_data;
    int m_dataCount;
    int m_frameCount;
    int m_dataLength;
};

#endif // FRAMEDECODE_H
