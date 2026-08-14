#ifndef SERVERMONITORINGBAR_H
#define SERVERMONITORINGBAR_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QDateTime>
#include <QPointer>
#include "serverconfig.h"
#include "metricbutton.h"

class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QChartView;
class QChart;
class QLineSeries;
class QValueAxis;
class QDateTimeAxis;

struct ServerMetrics {
    double cpuUsage = 0.0;
    double memoryUsage = 0.0;
    double memoryTotal = 0.0;
    double memoryUsed = 0.0;
    double networkRx = 0.0;
    double networkTx = 0.0;
    QString uptime;
    int userCount = 0;
    QDateTime lastUpdate;
};

class ServerMonitoringBar : public QWidget
{
    Q_OBJECT
public:
    explicit ServerMonitoringBar(const ServerConfig &config, QWidget *parent = nullptr);
    ~ServerMonitoringBar();
    
    void startMonitoring(int intervalMs = 5000);
    void stopMonitoring();
    void updateMetrics(const ServerMetrics &metrics);
    void addHistoryPoint(MetricButton::MetricType type, double value);
    
signals:
    void metricsRequested();
    void errorOccurred(const QString &error);

private slots:
    void onRefreshTimeout();
    void onMetricButtonHoverChart(MetricButton::MetricType type, const QPoint &globalPos);
    void onMetricButtonHistoryRequested(MetricButton::MetricType type);
    void onMetricButtonHoverChartHidden(MetricButton::MetricType type);
    void cleanupOldHistory();
    void updateHoverChart();
    void updateHistoryDialog();
    
private:
    void setupUI();
    void createMetricButton(MetricButton::MetricType type, const QString &icon, const QString &label);
    QWidget* createSeparator();
    void updateLabels();
    void showHoverChart(MetricButton::MetricType type, const QPoint &globalPos);
    void hideHoverChart(MetricButton::MetricType type);
    void showHistoryDialog(MetricButton::MetricType type);
    void updateButtonHistory(MetricButton::MetricType type);
    
    ServerConfig m_config;
    QTimer m_refreshTimer;
    QTimer m_historyCleanupTimer;
    QTimer m_hoverChartUpdateTimer;
    QTimer m_historyDialogUpdateTimer;
    ServerMetrics m_currentMetrics;
    
    QLabel *m_cpuLabel;
    QLabel *m_memoryLabel;
    QLabel *m_networkLabel;
    QLabel *m_uptimeLabel;
    QLabel *m_usersLabel;
    
    MetricButton *m_cpuButton;
    MetricButton *m_memoryButton;
    MetricButton *m_networkButton;
    
    QWidget *m_hoverChartWidget = nullptr;
    QChartView *m_hoverChartView = nullptr;
    QLineSeries *m_hoverChartSeries = nullptr;
    MetricButton::MetricType m_hoverChartType = MetricButton::CPU;
    
    QDialog *m_historyDialog = nullptr;
    QChartView *m_historyDialogChartView = nullptr;
    QLineSeries *m_historyDialogSeries = nullptr;
    MetricButton::MetricType m_historyDialogType = MetricButton::CPU;
    
    bool m_destroying = false;
    
    static constexpr int MAX_HISTORY_POINTS = 288; // 24 hours at 5 min intervals
    static constexpr int HISTORY_RETENTION_MS = 24 * 60 * 60 * 1000; // 24 hours
};

#endif // SERVERMONITORINGBAR_H