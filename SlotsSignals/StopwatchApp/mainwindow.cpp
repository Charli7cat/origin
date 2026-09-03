#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_stopwatch(new Stopwatch(this))
    , m_lapCounter(0)
    , m_lastLapTimeMs(0)
{
    ui->setupUi(this);

    ui->timeLabel->setText("00:00.0");
    ui->lapButton->setEnabled(false);

    connect(ui->startStopButton, &QPushButton::clicked,
            this, &MainWindow::onStartStopButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked,
            this, &MainWindow::onResetButtonClicked);
    connect(ui->lapButton, &QPushButton::clicked,
            this, &MainWindow::onLapButtonClicked);
    connect(m_stopwatch, &Stopwatch::timeUpdated,
            this, &MainWindow::updateTimeDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartStopButtonClicked()
{
    if (m_stopwatch->isRunning()) {
        m_stopwatch->stop();
        ui->startStopButton->setText("Старт");
        ui->lapButton->setEnabled(false);
    } else {
        m_stopwatch->start();
        ui->startStopButton->setText("Стоп");
        ui->lapButton->setEnabled(true);
        m_lapCounter = 0;
        m_lastLapTimeMs = 0;
    }
}

void MainWindow::onResetButtonClicked()
{
    m_stopwatch->reset();
    ui->timeLabel->setText("00:00.0");
    ui->startStopButton->setText("Старт");
    ui->lapButton->setEnabled(false);
    ui->lapBrowser->clear();
    m_lapCounter = 0;
    m_lastLapTimeMs = 0;
}

void MainWindow::onLapButtonClicked()
{
    if (!m_stopwatch->isRunning())
        return;

    m_lapCounter++;

    qint64 currentLapTime = m_stopwatch->getLapTimeMs();

    QString lapText = QString("Круг %1, время: %2 сек")
                          .arg(m_lapCounter)
                          .arg(formatTime(currentLapTime));
    ui->lapBrowser->append(lapText);

    // Сбрасываем время для следующего круга
    m_stopwatch->resetLapTime();
    m_lastLapTimeMs = currentLapTime;
}

void MainWindow::updateTimeDisplay(qint64 milliseconds)
{
    ui->timeLabel->setText(formatTime(milliseconds));
}

QString MainWindow::formatTime(qint64 milliseconds) const
{
    qint64 totalSeconds = milliseconds / 1000;
    qint64 minutes = totalSeconds / 60;
    qint64 seconds = totalSeconds % 60;
    qint64 tenths = (milliseconds % 1000) / 100;

    return QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(tenths);
}