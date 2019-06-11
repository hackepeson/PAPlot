#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

#define NO_OF_CURVES 2

  QVector<double> m_vecCloudCurves[NO_OF_CURVES];
  QVector<double> m_vecX;

};

#endif // MAINWINDOW_H