#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class Stopwatch : public QObject
{
    Q_OBJECT

public:
    explicit Stopwatch(QObject *parent = nullptr)
        : QObject(parent)
        , m_timer(new QTimer(this))
        , m_totalElapsedMs(0)
        , m_lapStartMs(0)
        , m_isRunning(false)
    {
        m_timer->setInterval(100);
        connect(m_timer, &QTimer::timeout, this, &Stopwatch::onTimeout);
    }

    ~Stopwatch() {}

    void start() {
        if (m_isRunning)
            return;
        m_elapsedTimer.start();
        m_lapStartMs = 0;
        m_isRunning = true;
        m_timer->start();
    }

    void stop() {
        if (!m_isRunning)
            return;
        m_timer->stop();
        m_totalElapsedMs += m_elapsedTimer.elapsed();
        m_isRunning = false;
    }

    void reset() {
        m_timer->stop();
        m_totalElapsedMs = 0;
        m_lapStartMs = 0;
        m_isRunning = false;
        emit timeUpdated(0);
    }

    bool isRunning() const { return m_isRunning; }

    qint64 getElapsedMs() const {
        if (m_isRunning) {
            return m_totalElapsedMs + m_elapsedTimer.elapsed();
        }
        return m_totalElapsedMs;
    }

    qint64 getLapTimeMs() const {
        if (m_isRunning) {
            return m_elapsedTimer.elapsed() - m_lapStartMs;
        }
        return 0;
    }

    void resetLapTime() {
        if (m_isRunning) {
            m_lapStartMs = m_elapsedTimer.elapsed();
        }
    }

signals:
    void timeUpdated(qint64 milliseconds);

private slots:
    void onTimeout() {
        emit timeUpdated(getElapsedMs());
    }

private:
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
    qint64 m_totalElapsedMs;
    qint64 m_lapStartMs;
    bool m_isRunning;
};

#endif