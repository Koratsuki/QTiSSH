#include "servermonitoringbar.h"
#include "metricbutton.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QApplication>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDialog>
#include <QDebug>
#include <QtCharts>

ServerMonitoringBar::ServerMonitoringBar(const ServerConfig &config, QWidget *parent)
    : QWidget(parent), m_config(config)
{
    setFixedHeight(36);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setupUI();
    
    m_refreshTimer.setSingleShot(false);
    connect(&m_refreshTimer, &QTimer::timeout, this, &ServerMonitoringBar::onRefreshTimeout);
    
    m_historyCleanupTimer.setSingleShot(false);
    m_historyCleanupTimer.setInterval(60000);
    connect(&m_historyCleanupTimer, &QTimer::timeout, this, &ServerMonitoringBar::cleanupOldHistory);
    m_historyCleanupTimer.start();
    
    m_hoverChartUpdateTimer.setSingleShot(false);
    m_hoverChartUpdateTimer.setInterval(2000);
    connect(&m_hoverChartUpdateTimer, &QTimer::timeout, this, &ServerMonitoringBar::updateHoverChart);
    
    m_historyDialogUpdateTimer.setSingleShot(false);
    m_historyDialogUpdateTimer.setInterval(2000);
    connect(&m_historyDialogUpdateTimer, &QTimer::timeout, this, &ServerMonitoringBar::updateHistoryDialog);
}

ServerMonitoringBar::~ServerMonitoringBar()
{
    m_destroying = true;
    m_hoverChartUpdateTimer.stop();
    m_historyDialogUpdateTimer.stop();
    stopMonitoring();
    
    if (m_hoverChartWidget) {
        m_hoverChartWidget->close();
        m_hoverChartWidget->deleteLater();
        m_hoverChartWidget = nullptr;
        m_hoverChartView = nullptr;
        m_hoverChartSeries = nullptr;
    }
    
    if (m_historyDialog) {
        m_historyDialog->close();
        m_historyDialog->deleteLater();
        m_historyDialog = nullptr;
        m_historyDialogChartView = nullptr;
        m_historyDialogSeries = nullptr;
    }
}

void ServerMonitoringBar::setupUI()
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 2, 8, 2);
    mainLayout->setSpacing(12);
    
    // CPU
    m_cpuButton = new MetricButton(MetricButton::CPU, this);
    m_cpuButton->setIcon(QIcon::fromTheme("cpu"));
    m_cpuButton->setIconSize(QSize(20, 20));
    connect(m_cpuButton, &MetricButton::hoverChartRequested, this, &ServerMonitoringBar::onMetricButtonHoverChart);
    connect(m_cpuButton, &MetricButton::historyRequested, this, &ServerMonitoringBar::onMetricButtonHistoryRequested);
    connect(m_cpuButton, &MetricButton::hoverChartHidden, this, &ServerMonitoringBar::onMetricButtonHoverChartHidden);
    
    m_cpuLabel = new QLabel("CPU: --%", this);
    m_cpuLabel->setMinimumWidth(80);
    m_cpuLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    auto *cpuLayout = new QHBoxLayout();
    cpuLayout->setSpacing(4);
    cpuLayout->addWidget(m_cpuButton);
    cpuLayout->addWidget(m_cpuLabel);
    
    // Memory
    m_memoryButton = new MetricButton(MetricButton::Memory, this);
    m_memoryButton->setIcon(QIcon::fromTheme("memory"));
    m_memoryButton->setIconSize(QSize(20, 20));
    connect(m_memoryButton, &MetricButton::hoverChartRequested, this, &ServerMonitoringBar::onMetricButtonHoverChart);
    connect(m_memoryButton, &MetricButton::historyRequested, this, &ServerMonitoringBar::onMetricButtonHistoryRequested);
    connect(m_memoryButton, &MetricButton::hoverChartHidden, this, &ServerMonitoringBar::onMetricButtonHoverChartHidden);
    
    m_memoryLabel = new QLabel("RAM: --%", this);
    m_memoryLabel->setMinimumWidth(80);
    m_memoryLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    auto *memLayout = new QHBoxLayout();
    memLayout->setSpacing(4);
    memLayout->addWidget(m_memoryButton);
    memLayout->addWidget(m_memoryLabel);
    
    // Network
    m_networkButton = new MetricButton(MetricButton::Network, this);
    m_networkButton->setIcon(QIcon::fromTheme("network-transmit-receive"));
    m_networkButton->setIconSize(QSize(20, 20));
    connect(m_networkButton, &MetricButton::hoverChartRequested, this, &ServerMonitoringBar::onMetricButtonHoverChart);
    connect(m_networkButton, &MetricButton::historyRequested, this, &ServerMonitoringBar::onMetricButtonHistoryRequested);
    connect(m_networkButton, &MetricButton::hoverChartHidden, this, &ServerMonitoringBar::onMetricButtonHoverChartHidden);
    
    m_networkLabel = new QLabel("Net: -- MB/s", this);
    m_networkLabel->setMinimumWidth(100);
    m_networkLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    auto *netLayout = new QHBoxLayout();
    netLayout->setSpacing(4);
    netLayout->addWidget(m_networkButton);
    netLayout->addWidget(m_networkLabel);
    
    // Uptime
    m_uptimeLabel = new QLabel("Up: --", this);
    m_uptimeLabel->setMinimumWidth(100);
    m_uptimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    // Users
    m_usersLabel = new QLabel("Users: --", this);
    m_usersLabel->setMinimumWidth(80);
    m_usersLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    // Add all to main layout
    mainLayout->addLayout(cpuLayout);
    mainLayout->addWidget(createSeparator());
    mainLayout->addLayout(memLayout);
    mainLayout->addWidget(createSeparator());
    mainLayout->addLayout(netLayout);
    mainLayout->addWidget(createSeparator());
    mainLayout->addWidget(m_uptimeLabel);
    mainLayout->addWidget(createSeparator());
    mainLayout->addWidget(m_usersLabel);
    mainLayout->addStretch();
    
    // Style
    setStyleSheet(R"(
        ServerMonitoringBar {
            background-color: #2b2b2b;
            border-top: 1px solid #3c3c3c;
            color: #e0e0e0;
        }
        QLabel {
            color: #e0e0e0;
            font-size: 11px;
            font-family: Monospace;
        }
        QPushButton {
            border: none;
            border-radius: 4px;
            background-color: transparent;
        }
        QPushButton:hover {
            background-color: #3c3c3c;
        }
        QPushButton:pressed {
            background-color: #4a4a4a;
        }
    )");
}

QWidget* ServerMonitoringBar::createSeparator()
{
    QWidget *sep = new QWidget(this);
    sep->setFixedWidth(1);
    sep->setStyleSheet("background-color: #3c3c3c;");
    sep->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    return sep;
}

void ServerMonitoringBar::startMonitoring(int intervalMs)
{
    m_refreshTimer.start(intervalMs);
    emit metricsRequested();
}

void ServerMonitoringBar::stopMonitoring()
{
    m_refreshTimer.stop();
}

void ServerMonitoringBar::updateMetrics(const ServerMetrics &metrics)
{
    m_currentMetrics = metrics;
    updateLabels();
    
    m_cpuButton->setCurrentValue(metrics.cpuUsage);
    m_memoryButton->setCurrentValue(metrics.memoryUsage);
    m_networkButton->setCurrentValue((metrics.networkRx + metrics.networkTx) / 1024.0 / 1024.0);
    
    // Add to history
    addHistoryPoint(MetricButton::CPU, metrics.cpuUsage);
    addHistoryPoint(MetricButton::Memory, metrics.memoryUsage);
    addHistoryPoint(MetricButton::Network, (metrics.networkRx + metrics.networkTx) / 1024.0 / 1024.0);
}

void ServerMonitoringBar::addHistoryPoint(MetricButton::MetricType type, double value)
{
    MetricButton *button = nullptr;
    switch (type) {
        case MetricButton::CPU: button = m_cpuButton; break;
        case MetricButton::Memory: button = m_memoryButton; break;
        case MetricButton::Network: button = m_networkButton; break;
    }
    
    if (button) {
        button->addHistoryPoint({QDateTime::currentDateTime(), value});
        updateButtonHistory(type);
    }
}

void ServerMonitoringBar::updateButtonHistory(MetricButton::MetricType type)
{
    // This could trigger UI updates for hover charts if they're visible
    // For now, the hover chart reads directly from the button's history data
}

void ServerMonitoringBar::cleanupOldHistory()
{
    QDateTime cutoff = QDateTime::currentDateTime().addMSecs(-HISTORY_RETENTION_MS);
    
    auto cleanButton = [cutoff](MetricButton *button) {
        if (!button) return;
        button->removeOldHistory(cutoff);
    };
    
    cleanButton(m_cpuButton);
    cleanButton(m_memoryButton);
    cleanButton(m_networkButton);
}

void ServerMonitoringBar::updateLabels()
{
    m_cpuLabel->setText(QString("CPU: %1%").arg(m_currentMetrics.cpuUsage, 0, 'f', 1));
    m_memoryLabel->setText(QString("RAM: %1% (%2/%3 MB)")
        .arg(m_currentMetrics.memoryUsage, 0, 'f', 1)
        .arg(m_currentMetrics.memoryUsed, 0, 'f', 0)
        .arg(m_currentMetrics.memoryTotal, 0, 'f', 0));
    m_networkLabel->setText(QString("Net: ↓%1 ↑%2 MB/s")
        .arg(m_currentMetrics.networkRx / 1024.0 / 1024.0, 0, 'f', 2)
        .arg(m_currentMetrics.networkTx / 1024.0 / 1024.0, 0, 'f', 2));
    m_uptimeLabel->setText(QString("Up: %1").arg(m_currentMetrics.uptime));
    m_usersLabel->setText(QString("Users: %1").arg(m_currentMetrics.userCount));
}

void ServerMonitoringBar::onRefreshTimeout()
{
    emit metricsRequested();
}

void ServerMonitoringBar::onMetricButtonHoverChart(MetricButton::MetricType type, const QPoint &globalPos)
{
    if (m_destroying) return;
    if (m_hoverChartWidget) {
        hideHoverChart(m_hoverChartType);
    }
    showHoverChart(type, globalPos);
}

void ServerMonitoringBar::onMetricButtonHoverChartHidden(MetricButton::MetricType type)
{
    if (m_destroying) return;
    if (m_hoverChartWidget && m_hoverChartType == type) {
        // Delay hiding slightly to allow moving mouse to chart
        QTimer::singleShot(200, this, [this, type]() {
            if (m_destroying) return;
            if (m_hoverChartWidget && m_hoverChartType == type) {
                // Check if mouse is still over the chart or button
                QWidget *widgetUnderMouse = QApplication::widgetAt(QCursor::pos());
                bool overChart = false;
                QWidget *w = widgetUnderMouse;
                while (w) {
                    if (w == m_hoverChartWidget || w == static_cast<QWidget*>(m_cpuButton) || w == static_cast<QWidget*>(m_memoryButton) || w == static_cast<QWidget*>(m_networkButton)) {
                        overChart = true;
                        break;
                    }
                    w = w->parentWidget();
                }
                if (!overChart) {
                    hideHoverChart(type);
                }
            }
        });
    }
}

void ServerMonitoringBar::onMetricButtonHistoryRequested(MetricButton::MetricType type)
{
    if (m_destroying) return;
    showHistoryDialog(type);
}

void ServerMonitoringBar::showHoverChart(MetricButton::MetricType type, const QPoint &globalPos)
{
    if (m_destroying) return;
    
    // Hide any existing chart first
    if (m_hoverChartWidget) {
        hideHoverChart(m_hoverChartType);
    }
    
    m_hoverChartType = type;
    
    QChart *chart = new QChart();
    chart->setTitle(type == MetricButton::CPU ? tr("CPU Usage") : 
                    type == MetricButton::Memory ? tr("Memory Usage") : tr("Network Usage"));
    chart->legend()->hide();
    chart->setBackgroundBrush(QBrush(QColor("#2b2b2b")));
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setMargins(QMargins(5, 5, 5, 5));
    
    m_hoverChartSeries = new QLineSeries();
    m_hoverChartSeries->setPen(QPen(QColor("#00d4aa"), 2));
    
    // Use history data from the button
    MetricButton *button = nullptr;
    switch (type) {
        case MetricButton::CPU: button = m_cpuButton; break;
        case MetricButton::Memory: button = m_memoryButton; break;
        case MetricButton::Network: button = m_networkButton; break;
    }
    
    if (button) {
        const QVector<MetricDataPoint> &data = button->historyData();
        for (int i = 0; i < data.size(); ++i) {
            m_hoverChartSeries->append(i, data[i].value);
        }
        // Add current value
        m_hoverChartSeries->append(data.size(), button->currentValue());
    }
    
    chart->addSeries(m_hoverChartSeries);
    
    QValueAxis *axisX = new QValueAxis();
    axisX->setVisible(false);
    chart->addAxis(axisX, Qt::AlignBottom);
    m_hoverChartSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setLabelFormat("%.0f");
    axisY->setGridLineColor(QColor("#3c3c3c"));
    axisY->setLabelsColor(Qt::white);
    chart->addAxis(axisY, Qt::AlignLeft);
    m_hoverChartSeries->attachAxis(axisY);
    
    m_hoverChartView = new QChartView(chart);
    m_hoverChartView->setRenderHint(QPainter::Antialiasing);
    m_hoverChartView->setMinimumSize(280, 150);
    m_hoverChartView->setStyleSheet("background-color: #2b2b2b; border: 1px solid #3c3c3c; border-radius: 4px;");
    
    m_hoverChartWidget = new QWidget(nullptr, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    // Don't use WA_DeleteOnClose - we manage lifetime manually
    auto *layout = new QVBoxLayout(m_hoverChartWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_hoverChartView);
    m_hoverChartWidget->setStyleSheet("background-color: #2b2b2b; border: 1px solid #3c3c3c; border-radius: 4px;");
    
    // Position near the button, ensure it's on screen
    int x = globalPos.x() - 140;
    int y = globalPos.y() - 160;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    m_hoverChartWidget->move(x, y);
    m_hoverChartWidget->show();
    
    // Start update timer for real-time updates
    m_hoverChartUpdateTimer.start();
}

void ServerMonitoringBar::hideHoverChart(MetricButton::MetricType type)
{
    if (m_hoverChartWidget && m_hoverChartType == type) {
        m_hoverChartUpdateTimer.stop();
        m_hoverChartWidget->close();
        m_hoverChartWidget->deleteLater();
        m_hoverChartWidget = nullptr;
        m_hoverChartView = nullptr;
        m_hoverChartSeries = nullptr;
    }
}

void ServerMonitoringBar::showHistoryDialog(MetricButton::MetricType type)
{
    if (m_destroying) return;
    
    // Close existing history dialog if open
    if (m_historyDialog) {
        m_historyDialog->close();
        m_historyDialog->deleteLater();
        m_historyDialog = nullptr;
        m_historyDialogChartView = nullptr;
        m_historyDialogSeries = nullptr;
    }
    
    m_historyDialogType = type;
    
    m_historyDialog = new QDialog(this, Qt::Window);
    m_historyDialog->setWindowTitle(type == MetricButton::CPU ? tr("CPU History (24h)") : 
                                    type == MetricButton::Memory ? tr("Memory History (24h)") : tr("Network History (24h)"));
    m_historyDialog->setMinimumSize(600, 400);
    m_historyDialog->setAttribute(Qt::WA_DeleteOnClose, false);
    m_historyDialog->setStyleSheet("background-color: #2b2b2b; color: #e0e0e0;");
    
    QChart *chart = new QChart();
    chart->setTitle(type == MetricButton::CPU ? tr("CPU Usage - Last 24 Hours") : 
                    type == MetricButton::Memory ? tr("Memory Usage - Last 24 Hours") : tr("Network Usage - Last 24 Hours"));
    chart->legend()->hide();
    chart->setBackgroundBrush(QBrush(QColor("#2b2b2b")));
    chart->setTitleBrush(QBrush(Qt::white));
    
    m_historyDialogSeries = new QLineSeries();
    m_historyDialogSeries->setPen(QPen(QColor("#00d4aa"), 2));
    
    MetricButton *button = nullptr;
    switch (type) {
        case MetricButton::CPU: button = m_cpuButton; break;
        case MetricButton::Memory: button = m_memoryButton; break;
        case MetricButton::Network: button = m_networkButton; break;
    }
    
    if (button) {
        const QVector<MetricDataPoint> &data = button->historyData();
        for (int i = 0; i < data.size(); ++i) {
            m_historyDialogSeries->append(data[i].timestamp.toMSecsSinceEpoch(), data[i].value);
        }
    }
    
    chart->addSeries(m_historyDialogSeries);
    
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("HH:mm");
    axisX->setTitleText(tr("Time"));
    axisX->setGridLineColor(QColor("#3c3c3c"));
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(Qt::white));
    chart->addAxis(axisX, Qt::AlignBottom);
    m_historyDialogSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText(tr("Usage %"));
    axisY->setGridLineColor(QColor("#3c3c3c"));
    axisY->setLabelsColor(Qt::white);
    axisY->setTitleBrush(QBrush(Qt::white));
    chart->addAxis(axisY, Qt::AlignLeft);
    m_historyDialogSeries->attachAxis(axisY);
    
    m_historyDialogChartView = new QChartView(chart);
    m_historyDialogChartView->setRenderHint(QPainter::Antialiasing);
    m_historyDialogChartView->setRubberBand(QChartView::HorizontalRubberBand);
    m_historyDialogChartView->setStyleSheet("background-color: #2b2b2b; border: none;");
    
    auto *layout = new QVBoxLayout(m_historyDialog);
    layout->addWidget(m_historyDialogChartView);
    
    // Connect close event to clean up
    connect(m_historyDialog, &QDialog::finished, this, [this]() {
        m_historyDialogUpdateTimer.stop();
        m_historyDialog = nullptr;
        m_historyDialogChartView = nullptr;
        m_historyDialogSeries = nullptr;
    });
    
    m_historyDialog->show();
    
    // Start update timer for real-time updates
    m_historyDialogUpdateTimer.start();
}

void ServerMonitoringBar::updateHoverChart()
{
    if (m_destroying || !m_hoverChartWidget || !m_hoverChartSeries) {
        m_hoverChartUpdateTimer.stop();
        return;
    }
    
    MetricButton *button = nullptr;
    switch (m_hoverChartType) {
        case MetricButton::CPU: button = m_cpuButton; break;
        case MetricButton::Memory: button = m_memoryButton; break;
        case MetricButton::Network: button = m_networkButton; break;
    }
    
    if (!button) {
        m_hoverChartUpdateTimer.stop();
        return;
    }
    
    const QVector<MetricDataPoint> &data = button->historyData();
    
    // Clear and rebuild series with updated data
    m_hoverChartSeries->clear();
    for (int i = 0; i < data.size(); ++i) {
        m_hoverChartSeries->append(i, data[i].value);
    }
    // Add current value as the latest point
    m_hoverChartSeries->append(data.size(), button->currentValue());
}

void ServerMonitoringBar::updateHistoryDialog()
{
    if (m_destroying || !m_historyDialog || !m_historyDialogSeries) {
        m_historyDialogUpdateTimer.stop();
        return;
    }
    
    MetricButton *button = nullptr;
    switch (m_historyDialogType) {
        case MetricButton::CPU: button = m_cpuButton; break;
        case MetricButton::Memory: button = m_memoryButton; break;
        case MetricButton::Network: button = m_networkButton; break;
    }
    
    if (!button) {
        m_historyDialogUpdateTimer.stop();
        return;
    }
    
    const QVector<MetricDataPoint> &data = button->historyData();
    
    // Clear and rebuild series with updated data
    m_historyDialogSeries->clear();
    for (int i = 0; i < data.size(); ++i) {
        m_historyDialogSeries->append(data[i].timestamp.toMSecsSinceEpoch(), data[i].value);
    }
}