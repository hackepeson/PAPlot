#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

#define CLOUD_VECTOR_LENGTH  1200
  for (int i = 0; i < CLOUD_VECTOR_LENGTH; i++)
  {
    m_vecX.append(i);
  }

  for (int i = 0; i < NO_OF_CURVES; i++)
  {
    for (int n = 0; n < CLOUD_VECTOR_LENGTH; n++)
    {
      m_vecCloudCurves[i].append(sin(n*n+i*i*i*i)/100000);
    }
    ui->widget->addGraph();
    ui->widget->graph(i)->setData(m_vecX,m_vecCloudCurves[i]);
    if (i==0)
    {
      ui->widget->graph(i)->setPen(QPen(Qt::cyan));
    }
    else
    {
      ui->widget->graph(i)->setPen(QPen(Qt::red));
    }


  }

  ui->widget->rescaleAxes();
  ui->widget->replot();




}

MainWindow::~MainWindow()
{
  delete ui;
}
