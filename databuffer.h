#ifndef DATABUFFER_H
#define DATABUFFER_H


#include <QVector>
#include <QList>
#include <QPointF>


namespace QtCharts {
class QLineSeries;
}


class databuffer
{
public:
    databuffer(const QVector<QtCharts::QLineSeries*> *series);
    void append(int channel, double value);
    int update();
    int maximumCount();
    void clear();

private:
    const QVector<QtCharts::QLineSeries*> *m_series;
    QVector<QVector<QPointF>> m_data;
    QVector<int> m_count;
};

#endif // DATABUFFER_H
