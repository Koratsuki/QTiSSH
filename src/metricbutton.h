#ifndef METRICBUTTON_H
#define METRICBUTTON_H

#include <QPushButton>
#include <QVector>
#include <QDateTime>
#include <QTimer>

struct MetricDataPoint {
    QDateTime timestamp;
    double value;
};

class MetricButton : public QPushButton
{
    Q_OBJECT
public:
    enum MetricType { CPU, Memory, Network };
    
    explicit MetricButton(MetricType type, QWidget *parent = nullptr);
    ~MetricButton();
    
    MetricType metricType() const { return m_type; }
    void setCurrentValue(double value);
    void setHistoryData(const QVector<MetricDataPoint> &data);
    const QVector<MetricDataPoint> &historyData() const { return m_historyData; }
    double currentValue() const { return m_currentValue; }
    void addHistoryPoint(const MetricDataPoint &point);
    void removeOldHistory(const QDateTime &cutoff);
    
signals:
    void historyRequested(MetricType type);
    void hoverChartRequested(MetricType type, const QPoint &globalPos);
    void hoverChartHidden(MetricType type);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    
private:
    MetricType m_type;
    double m_currentValue = 0.0;
    QVector<MetricDataPoint> m_historyData;
    QTimer m_hoverTimer;
    bool m_showingHoverChart = false;
    bool m_destroying = false;
};

#endif // METRICBUTTON_H