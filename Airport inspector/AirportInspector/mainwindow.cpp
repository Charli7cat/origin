#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_db = new DatabaseManager(this);
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(5000); // ФТ6 - 5 секунд

    connect(m_reconnectTimer, &QTimer::timeout, this, &MainWindow::tryConnect);
    connect(ui->getFlightsButton, &QPushButton::clicked, this, &MainWindow::onGetFlightsClicked);
    connect(ui->showLoadButton, &QPushButton::clicked, this, &MainWindow::onShowLoadClicked);

    // ФТ4 - блокируем кнопки до подключения
    ui->getFlightsButton->setEnabled(false);
    ui->showLoadButton->setEnabled(false);
    ui->arrivalRadio->setChecked(true); // По умолчанию прилеты

    // Ограничение даты (O2)
    ui->dateEdit->setDateRange(QDate(2016, 8, 15), QDate(2017, 9, 14));
    ui->dateEdit->setDate(QDate(2017, 8, 15));

    updateConnectionStatus(false);
    tryConnect(); // ФТ1 - автоподключение при старте
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::tryConnect() {
    if (m_db->connectToDb()) {
        m_reconnectTimer->stop();
        updateConnectionStatus(true);
        ui->getFlightsButton->setEnabled(true);  // ФТ4 - разблокировка
        ui->showLoadButton->setEnabled(true);
        populateAirports();
    } else {
        // ФТ5 - показываем ошибку
        QMessageBox::critical(this, "Ошибка подключения", m_db->lastError());
        updateConnectionStatus(false);
        // ФТ6 - повтор через 5 сек
        if (!m_reconnectTimer->isActive()) m_reconnectTimer->start();
    }
}

void MainWindow::updateConnectionStatus(bool connected) {
    // ФТ3 - статус в нижней части окна
    if (connected) {
        ui->statusLabel->setText("Подключено");
        ui->statusLabel->setStyleSheet("color: green;");
    } else {
        ui->statusLabel->setText("Отключено");
        ui->statusLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::populateAirports() {
    m_airports = m_db->getAirports();
    ui->airportComboBox->clear();
    // ФТ8, ФТ9 - выпадающий список с русскими названиями
    for (auto it = m_airports.begin(); it != m_airports.end(); ++it) {
        ui->airportComboBox->addItem(it.value(), it.key());
    }
}

void MainWindow::onGetFlightsClicked() {
    QString code = ui->airportComboBox->currentData().toString();
    QDate date = ui->dateEdit->date();
    bool isArrival = ui->arrivalRadio->isChecked();

    QVector<FlightInfo> flights;
    if (isArrival)
        flights = m_db->getArrivals(code, date);
    else
        flights = m_db->getDepartures(code, date);

    if (flights.isEmpty() && !m_db->lastError().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", m_db->lastError());
        return;
    }
    fillTable(flights, isArrival);
}

void MainWindow::fillTable(const QVector<FlightInfo> &flights, bool isArrival) {
    ui->flightsTable->setRowCount(0);
    ui->flightsTable->setColumnCount(3);
    ui->flightsTable->setHorizontalHeaderLabels(
        {"Номер рейса", isArrival ? "Время прилета" : "Время вылета",
         isArrival ? "Аэропорт отправления" : "Аэропорт назначения"});

    for (int i = 0; i < flights.size(); ++i) {
        ui->flightsTable->insertRow(i);
        ui->flightsTable->setItem(i, 0, new QTableWidgetItem(flights[i].flightNo));
        ui->flightsTable->setItem(i, 1, new QTableWidgetItem(
                                            flights[i].time.toString("dd.MM.yyyy HH:mm")));
        ui->flightsTable->setItem(i, 2, new QTableWidgetItem(flights[i].airportName));
    }
}

void MainWindow::onShowLoadClicked() {
    QString code = ui->airportComboBox->currentData().toString();
    QString name = ui->airportComboBox->currentText();

    // ФТ15 - блокируем главное окно
    this->setEnabled(false);

    LoadWindow *lw = new LoadWindow(m_db, code, name, this);
    lw->setAttribute(Qt::WA_DeleteOnClose);
    // ФТ16 - разблокировка при закрытии
    connect(lw, &QDialog::finished, this, [this]() {
        this->setEnabled(true);
    });
    lw->show();
}