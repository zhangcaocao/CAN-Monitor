#include "framedecode.h"
#include <cctype>

enum {
    Frame_Head     = '!',           //帧头识别字
    Frame_ID = '!',                  //帧头起始
    Frame_status = 's',             //状态
    Frame_DATAB = 'd',               //数据开始
    Frame_DATAE = 'e',              //数据结束
    Frame_Headend  = 's',           //帧头结束字
};

enum {
    STA_None = 0, // 空闲状态
    STA_Headbegin =1,    //帧头起始
    STA_Headbegind=2,    //帧头起始
    STA_status=3,        //状态
    STA_statusd=4,        //状态结束
    STA_DATAB=5,          //数据开始
    STA_DATAE=6,          //数据结束
    };

enum Result {
    Ok = 0,
    Error = 1,
    Done = 2,
};

FrameDecode::FrameDecode()
{
    m_status = STA_None;
    m_data = 0;

    m_dataCount  = 0;
    m_frameCount = 0;
    m_dataLength = 8;  //数据帧默认为8个字节
//    m_frameLength = 0;
}

FrameDecode::~FrameDecode()
{
}

bool FrameDecode::FrameDecode_p(CANFrame *data, char byte)
{
//    int res;
    switch (m_status) {
    case STA_None: //空闲
        data->value.clear();
        m_status = byte == Frame_Head ? STA_Headbegin : STA_None;
        break;
    case STA_Headbegin:
        data->ID = byte;
        m_status = STA_Headbegind;
        break;
    case STA_Headbegind:
        m_status = byte == Frame_status ? STA_status : STA_None;
        break;
    case STA_status:  //开始读取status
        if(byte == '1' || byte=='0' || byte=='4' || byte=='5')
        {
            data->status = byte;
            m_status = STA_statusd;
            break;
        }else {
            m_status = STA_None;
            break;
        }
    case STA_statusd:
        m_status = byte == Frame_DATAB ? STA_DATAB : STA_None;
        break;
    case STA_DATAB:
        if (byte != 'e' && (isdigit(byte) || byte == '.'))
        {
            data->value.append(QString(byte));
            break;
        }else {
            if (data->value.size())
            {
                m_status = STA_None;
                return true;
            }
            m_status = STA_None;
            break;
}
//        res = GetData(data, byte);
//        // if res
//        switch (res) {
//        case Ok: // 还在接收数据
//            break;
//        case Error: // 错误则重新开始接收
//            m_status = STA_None;
//            break;
//        case Done: // 结束返回true
//            return true;
//        }
//        break;
    }
    return false;
}


int FrameDecode::GetData(CANFrame *data, char byte)
{
    switch (m_dataCount) {
    //  前三个数据是信息帧
    case 0:
        data->ID = byte;
        break;
    case 1:
        data->status = byte;
        break;
    case 2:
        data->DLC = byte;
        m_dataLength = data->DLC + 3;
        if (m_dataLength > 11) // 数据长度异常
        {
            m_dataCount = 0;
            return Error;
        }
        break;
    //后DLC个数据为数据帧
    default:
        //判断是否为第四个数据
        if (m_dataCount >= 3 && m_dataCount < m_dataLength)
        {
            data->value[m_dataCount-3] = byte;
            if ((m_dataCount-3) == m_dataLength-4)
            {
                m_dataCount = 0;
                return Done;
            }
        }
        else if (m_dataCount <3 || m_dataCount > m_dataLength) {  //数据长度异常
            m_dataCount = 0;
            return Error;
        }
        break;
    }
    ++ m_dataCount;
    return Ok;
}

QVector<FrameDecode::CANFrame> FrameDecode::frameDecode(const QByteArray &array)
{
    CANFrame Canframedata;
    QVector<CANFrame> vector;
    for(char byte:array)
    {

        if (FrameDecode_p(&Canframedata, byte) == true)
        {
            vector.append(Canframedata);
        }
    }
    return vector;
}
