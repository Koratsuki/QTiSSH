#include "metricbutton.h"
#include <QEnterEvent>
#include <QMouseEvent>
#include <QTimer>

MetricButton::MetricButton(MetricType type, QWidget *parent)
    : QPushButton(parent), m_type(type)
{
    setFixedSize(32, 32);
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
    
    m_hoverTimer.setSingleShot(true);
    m_hoverTimer.setInterval(500);
    connect(&m_hoverTimer, &QTimer::timeout, this, [this]() {
        if (!m_destroying) {
            emit hoverChartRequested(m_type, mapToGlobal(rect().bottomLeft()));
        }
    });
}

void MetricButton::setCurrentValue(double value)
{
    m_currentValue = value;
    QString suffix;
    switch (m_type) {
        case CPU:
        case Memory:
            suffix = "%";
            break;
        case Network:
            suffix = " MB/s";
            break;
    }
    setToolTip(QString("%1: %2%3").arg(
        m_type == CPU ? tr("CPU") : (m_type == Memory ? tr("Memory") : tr("Network")),
        QString::number(value, 'f', 1), suffix));
}

void MetricButton::setHistoryData(const QVector<MetricDataPoint> &data)
{
    m_historyData = data;
}

void MetricButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    m_hoverTimer.start();
}

void MetricButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    m_hoverTimer.stop();
    emit hoverChartHidden(m_type);
}

void MetricButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit historyRequested(m_type);
    }
    QPushButton::mousePressEvent(event);
}

void MetricButton::addHistoryPoint(const MetricDataPoint &point)
{
    m_historyData.append(point);
    
    // Limit history size
    static constexpr int MAX_HISTORY_POINTS = 288; // 24 hours at 5 min intervals
    while (m_historyData.size() > MAX_HISTORY_POINTS) {
        m_historyData.removeFirst();
    }
}

void MetricButton::removeOldHistory(const QDateTime &cutoff)
{
    int removeCount = 0;
    for (const auto &pt : m_historyData) {
        if (pt.timestamp < cutoff) {
            removeCount++;
        } else {
            break;
        }
    }
    if (removeCount > 0) {
        m_historyData.remove(0, removeCount);
    }
}

MetricButton::~MetricButton()
{
    m_destroying = true;
    m_hoverTimer.stop();
}