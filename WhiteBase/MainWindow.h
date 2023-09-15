#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateWindowsTitle(bool isChanged);

private slots:
    void on_cmdOpenApplication_clicked();

    void on_cmdOpenFolder_clicked();

    void on_cmdOpenDocument_clicked();

    void on_cmdOpenLink_clicked();

    void on_actionUser_Guide_triggered();

private:
    Ui::MainWindow *ui;
    QString m_windowsTitle;
};
#endif // MAINWINDOW_H
