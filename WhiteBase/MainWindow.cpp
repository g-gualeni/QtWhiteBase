#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "Version.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_windowsTitle = QString("%1 - %2").arg(PROJECT_NAME, PROJECT_VERSION);
    updateWindowsTitle(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWindowsTitle(bool isChanged)
{
    if(isChanged) {
        setWindowTitle(m_windowsTitle + "*");
    }
    else {
        setWindowTitle(m_windowsTitle);
    }
}




void MainWindow::on_cmdOpenApplication_clicked()
{

    QString folder = "C:/Program Files/Notepad++";
    QString application = "notepad++.exe";
    QUrl url = QUrl::fromLocalFile(folder + "/" + application);
    QDesktopServices::openUrl(url);

}


void MainWindow::on_cmdOpenFolder_clicked()
{
    QString folder = "C:/Program Files/Notepad++";
    QUrl url = QUrl::fromLocalFile(folder);
    QDesktopServices::openUrl(url);

}


void MainWindow::on_cmdOpenDocument_clicked()
{
    QString folder = "C:/Program Files/Notepad++";
    QString file = "readme.txt";
    QUrl url = QUrl::fromLocalFile(folder + "/" + file);
    QDesktopServices::openUrl(url);

}


void MainWindow::on_cmdOpenLink_clicked()
{
    QUrl url = QUrl("https://www.qt.io/");
    QDesktopServices::openUrl(url);
}


void MainWindow::on_actionUser_Guide_triggered()
{
    QFileInfo FI(qApp->applicationDirPath() + "/WhiteBase UserGuide.pdf");
    if(!FI.exists()) {
        QString msg = "Unable to open " + FI.absoluteFilePath();
        QMessageBox::warning(this, "User Guide", msg);
    }

    QUrl url = QUrl::fromLocalFile(FI.absoluteFilePath());
    QDesktopServices::openUrl(url);

}

