#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>

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
    ui->widgetPlot->addGraph();
    ui->widgetPlot->graph(i)->setData(m_vecX,m_vecCloudCurves[i]);
    if (i==0)
    {
      ui->widgetPlot->graph(i)->setPen(QPen(Qt::cyan));
    }
    else
    {
      ui->widgetPlot->graph(i)->setPen(QPen(Qt::red));
    }


  }

  ui->widgetPlot->rescaleAxes();
  ui->widgetPlot->replot();

  m_pComPort = new QSerialPort(this);
 QSerialPortInfo port;
 QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
 foreach (port, ports)
 {
   ui->comboBoxSerialPorts->addItem(port.portName() + " " + port.description());
     //qDebug() << "port name:"       << info.portName;
     //qDebug() << "friendly name:"   << info.friendName;
     //qDebug() << "physical name:"   << info.physName;
     //qDebug() << "enumerator name:" << info.enumName;
     //qDebug() << "vendor ID:"       << info.vendorID;
     //qDebug() << "product ID:"      << info.productID;
     //qDebug() << "===================================";
 }




}

MainWindow::~MainWindow()
{
  delete ui;
}
