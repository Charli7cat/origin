#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_db = QSqlDatabase::addDatabase("QPSQL", "films_connection");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("filmsdb");
    m_db.setUserName("postgres");
    m_db.setPassword("postgres");
    m_db.setPort(5432);

    if (!m_db.open()) {
        QMessageBox::critical(this, "Ошибка БД",
                              "Не удалось подключиться:\n" + m_db.lastError().text());
        return;
    }

    m_tableModel = new QSqlTableModel(this, m_db);
    m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    m_queryModel = new QSqlQueryModel(this);

    ui->comboBoxFilter->setCurrentIndex(0);
    loadAllFilms();
}

MainWindow::~MainWindow()
{
    if (m_db.isOpen()) m_db.close();
    delete ui;
}

void MainWindow::loadAllFilms()
{
    m_tableModel->setTable("films");
    m_tableModel->setSort(m_tableModel->fieldIndex("title"), Qt::AscendingOrder);
    m_tableModel->select();

    int idCol   = m_tableModel->fieldIndex("id");
    int nameCol = m_tableModel->fieldIndex("title");
    int descCol = m_tableModel->fieldIndex("description");

    m_tableModel->setHeaderData(nameCol, Qt::Horizontal, "Название фильма");
    m_tableModel->setHeaderData(descCol, Qt::Horizontal, "Описание фильма");

    ui->tableView->setModel(m_tableModel);

    ui->tableView->setColumnHidden(idCol, true);

    for (int i = 0; i < m_tableModel->columnCount(); ++i) {
        if (i != nameCol && i != descCol) {
            ui->tableView->setColumnHidden(i, true);
        }
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::loadFilmsByGenre(const QString &genre)
{
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT title       AS \"Название фильма\", "
        "       description AS \"Описание фильма\" "
        "FROM films "
        "WHERE genre = :genre "
        "ORDER BY title");
    query.bindValue(":genre", genre);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка",
                             "Не удалось выполнить запрос:\n" +
                                 query.lastError().text());
        return;
    }

    m_queryModel->setQuery(query);
    ui->tableView->setModel(m_queryModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
}

void MainWindow::on_btnApply_clicked()
{
    const QString filter = ui->comboBoxFilter->currentText();

    if (filter == "Все") {
        loadAllFilms();
    } else if (filter == "Комедия") {
        loadFilmsByGenre("Комедия");
    } else if (filter == "Ужасы") {
        loadFilmsByGenre("Ужасы");
    }
}

void MainWindow::on_btnClear_clicked()
{
    ui->tableView->setModel(nullptr);
    m_tableModel->clear();
    m_queryModel->clear();
}