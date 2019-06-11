#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);


/////////////////////////////// PLOT ////////////////////////////////////////
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

////////////////////////// COM PORT ////////////////////////////////////////////
  m_pComPort = new QSerialPort(this);
  QSerialPortInfo port;
  QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
  foreach (port, ports)
  {
    ui->comboBoxSerialPorts->addItem(port.portName() + " " + port.description());
  }

  connect(ui->pushButtonConnectComport, SIGNAL(clicked(bool)), SLOT(connectSerialPort()));
  connect(m_pComPort, SIGNAL(readyRead()), SLOT(readyRead()));
}


void MainWindow::readyRead()
{
#define MAX_LINE_LENGTH 20000

  while (m_pComPort->canReadLine())
  {
    QByteArray dataLine = m_pComPort->readLine(MAX_LINE_LENGTH);
    if (dataLine.length() > what)
    {

    }
  }

}


void MainWindow::connectSerialPort()
{

  if (ui->pushButtonConnectComport->isChecked())
  {
    if (!m_pComPort->isOpen())
    {
      QString comPort = ui->comboBoxSerialPorts->currentText();
      QStringList nameList = comPort.split(" ");
      m_pComPort->setPortName(nameList.at(0));

      //m_pComPort->setReadBufferSize(1000);
      bool ok;
      if (m_pComPort->open(QIODevice::ReadWrite))
      {
        m_pComPort->setBaudRate(ui->comboBoxBaudrate->currentText().toInt(&ok));
        m_pComPort->setFlowControl(QSerialPort::NoFlowControl);
        m_pComPort->setDataBits(QSerialPort::Data8);
        m_pComPort->setParity(QSerialPort::NoParity);
        ui->statusBar->showMessage("COM port openened : " + comPort, 3000);

        ui->pushButtonConnectComport->setText("Disconnect");
        m_pComPort->flush();
      }
      else
      {
        ui->statusBar->showMessage("Failed to open COM port");
      }

    }
  }
  else
  {
    m_pComPort->close();
    ui->pushButtonConnectComport->setText("Connect");
  }
}




MainWindow::~MainWindow()
{
  delete ui;
}
