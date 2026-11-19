#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
#include <QMessageBox>
#include <QRegularExpression>

MainWindow::MainWindow(const QString &iniPath, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), iniPath_(iniPath) {
    ui->setupUi(this);

    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->queryEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);
    connect(ui->showDocsButton, &QPushButton::clicked, this, &MainWindow::onShowDocumentsClicked);

    ui->showDocsButton->setEnabled(false);
    loadWordsTab();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showError(const QString &msg) {
    QMessageBox::critical(this, "Ошибка", msg);
}

void MainWindow::loadWordsTab() {
    QVector<WordStat> stats;
    QString err;
    if (!SearchService::allWordsWithFrequency(&stats, &err)) { showError(err); return; }
    ui->wordsList->clear();
    for (const auto &s : stats) {
        ui->wordsList->addItem(QString("%1\t%2").arg(s.word).arg(s.frequency));
    }
}

void MainWindow::onSearchClicked() {
    const QString text = ui->queryEdit->text().trimmed();
    if (text.isEmpty()) { ui->resultsList->clear(); return; }

    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QString err;
    QVector<SearchHit> hits;
    if (!SearchService::search(words, &hits, &err)) { showError(err); return; }

    lastHits = hits;
    ui->resultsList->clear();
    if (hits.isEmpty()) {
        ui->resultsList->addItem("Ничего не найдено.");
        ui->showDocsButton->setEnabled(false);
        return;
    }
    for (const auto &h : hits) {
        ui->resultsList->addItem(QString("%1 [релевантность: %2]").arg(h.path).arg(h.relevance));
    }
    ui->showDocsButton->setEnabled(true);
}

void MainWindow::onShowDocumentsClicked() {
    ui->documentsList->clear();
    for (const auto &h : lastHits) {
        ui->documentsList->addItem(QString("%1 — %2").arg(h.path).arg(h.relevance));
    }
}