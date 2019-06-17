#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

#define NO_OF_CURVES 10

  int m_dCurveIndex;

  QVector<double> m_vecCloudCurves[NO_OF_CURVES];
  QVector<double> m_vecX;

  QSerialPort* m_pComPort;

private slots:

  void connectSerialPort();
  void readyRead();
  void saveData();


};

#endif // MAINWINDOW_H
