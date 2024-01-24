#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// It seems that QFileDialog::getOpenFileContent() doesn't work well on the native (not wasm) platform.
// The application crashed for me as this code returns.
// So we need to code two versions:
// - For WASM/EMSCRIPTEN, use GetOpenFileContent to read files safely in sandboxed environments.
// - For native, use the more straighforward QFileDialog::GetOpenFileName to get the file name.
//
#if defined(__EMSCRIPTEN__)
void MainWindow::on_pushButton_clicked()
{
    auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            qDebug() << "No file was selected";
            return;
        } else {
            // Use fileName and fileContent
            qDebug() << fileName;
        }
        QFileInfo fi(fileName);
        qDebug() << "path: " << fi.path();
        qDebug() << fileContent;
        ui->plainTextEdit->setPlainText(QString(fileContent));

        // This is the best we can do in the sandboxed environment.
        // On Chrome, this starts a download process that the user can control.
        QFileDialog::saveFileContent(fileContent, fileName+".tmp");
    };
    QFileDialog::getOpenFileContent(tr("*.*"),  fileContentReady);

}
#else
void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath() );
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "title", "file not open for reading");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->plainTextEdit->setPlainText(text);
    file.close();

    file_name += ".tmp";
    file.setFileName(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "title", "file not open for writing");
        return;
    }
    QTextStream out(&file);
    out << text;
    file.close();
}
#endif

