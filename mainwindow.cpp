#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->generateButton, SIGNAL(clicked()), SLOT(generate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate()
{
    ui->outputEdit->setText(genrator.toDiagram(ui->inputEdit->toPlainText()));
}
