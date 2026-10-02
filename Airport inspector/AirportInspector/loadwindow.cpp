#include "loadwindow.h"
#include "ui_loadwindow.h"

LoadWindow::LoadWindow(DatabaseManager *db, const QString &airportCode,
                       const QString &airportName, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LoadWindow),
    m_db(db),
    m_airportCode(airportCode)
{
    ui->setupUi(this);
    ui->airportNameLabel->setText("Аэропорт: " + airportName);
    setWindowTitle("Загруженность аэропорта: " + airportName);


    QStringList months = {"Январь","Февраль","Март","Апрель","Май","Июнь",
                          "Июль","Август","Сентябрь","Октябрь","Ноябрь","Декабрь"};
    ui->monthComboBox->addItems(months);
    ui->monthComboBox->setCurrentIndex(0); // ФТ21 — Январь по умолчанию

    m_yearPlot = new QCustomPlot(ui->yearChartWidget);
    m_yearPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_monthPlot = new QCustomPlot(ui->monthChartWidget);
    m_monthPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *yearLayout = qobject_cast<QVBoxLayout*>(ui->yearChartWidget->layout());
    if (!yearLayout) {
        yearLayout = new QVBoxLayout(ui->yearChartWidget);
    }
    yearLayout->addWidget(m_yearPlot);

    QVBoxLayout *monthLayout = qobject_cast<QVBoxLayout*>(ui->monthChartWidget->layout());
    if (!monthLayout) {
        monthLayout = new QVBoxLayout(ui->monthChartWidget);
    }
    monthLayout->addWidget(m_monthPlot);

    buildYearChart();
    buildMonthChart(1);

    connect(ui->monthComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LoadWindow::onMonthChanged);
    connect(ui->closeButton, &QPushButton::clicked,
            this, &LoadWindow::onCloseClicked);
}

LoadWindow::~LoadWindow() {
    delete ui;
}

void LoadWindow::buildYearChart() {
    QMap<int, int> stats = m_db->getYearStats(m_airportCode);

    QVector<double> positions;
    QVector<double> values;
    QVector<QString> labels;

    QStringList monthNames = {"Январь","Февраль","Март","Апрель","Май","Июнь",
                              "Июль","Август","Сентябрь","Октябрь","Ноябрь","Декабрь"};

    int idx = 1;
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        positions.append(idx);
        values.append(it.value());
        labels.append(monthNames[it.key() - 1]);
        ++idx;
    }

    QCPBars *bars = new QCPBars(m_yearPlot->xAxis, m_yearPlot->yAxis);
    bars->setData(positions, values);
    bars->setName("Рейсы");
    bars->setWidth(0.7);
    bars->setPen(QPen(Qt::darkBlue));
    bars->setBrush(QBrush(QColor(50, 100, 200, 150)));

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for (int i = 0; i < positions.size(); ++i) {
        textTicker->addTick(positions[i], labels[i]);
    }
    m_yearPlot->xAxis->setTicker(textTicker);
    m_yearPlot->xAxis->setRange(0, positions.size() + 1);

    double maxY = 0;
    for (double v : values) if (v > maxY) maxY = v;
    m_yearPlot->yAxis->setRange(0, maxY * 1.15 + 1);

    m_yearPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_yearPlot->replot();
}

void LoadWindow::buildMonthChart(int month) {
    m_monthPlot->clearPlottables();
    m_monthPlot->clearGraphs();

    QMap<int, int> stats = m_db->getMonthStats(m_airportCode, month);

    QVector<double> days;
    QVector<double> values;
    int maxDay = 0;
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        days.append(it.key());
        values.append(it.value());
        if (it.key() > maxDay) maxDay = it.key();
    }

    QCPGraph *graph = m_monthPlot->addGraph();
    graph->setData(days, values);
    graph->setPen(QPen(Qt::blue, 2));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    m_monthPlot->xAxis->setLabel("День месяца");
    m_monthPlot->yAxis->setLabel("Количество рейсов");
    m_monthPlot->xAxis->setRange(1, maxDay > 0 ? maxDay : 31);

    double maxY = 0;
    for (double v : values) if (v > maxY) maxY = v;
    m_monthPlot->yAxis->setRange(0, maxY * 1.15 + 1);

    m_monthPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_monthPlot->replot();
}

void LoadWindow::onMonthChanged(int index) {
    buildMonthChart(index + 1);
}

void LoadWindow::onCloseClicked() {
    this->close();
}