#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dCurveIndex(0)
{
    ui->setupUi(this);


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

    /////////////////////////////// PLOT ////////////////////////////////////////
#define CLOUD_VECTOR_LENGTH  1200
    for (int i = 0; i < CLOUD_VECTOR_LENGTH; i++)
    {
        m_vecX.append(i);
    }

    for (int i = 0; i < NO_OF_CURVES; i++)
    {

        ui->widgetPlot->addGraph();
        /*
        if (i==0)
        {
            ui->widgetPlot->graph(i)->setPen(QPen(Qt::cyan));
        }
        else
        {
            ui->widgetPlot->graph(i)->setPen(QPen(Qt::red));
        }
        */
    }

    ////////////////////// SAVE DATA ///////////////////////////////////////////////

    connect(ui->pushButtonSaveData, SIGNAL(clicked(bool)), SLOT(saveData()));





}


void MainWindow::readyRead()
{
#define MAX_LINE_LENGTH 20000

    while (m_pComPort->canReadLine())
    {
        QByteArray dataLine = m_pComPort->readLine(MAX_LINE_LENGTH);

        if ((dataLine.at(0) == 2) && (dataLine.at(dataLine.length()-3) == 3))
        {
            QString data(QString::fromStdString(dataLine.toStdString()).simplified());
            ui->textEditInfo->append(data);

            QStringList qsl = data.split(" ");

            if (qsl.length() == 768)
            {
                m_vecCloudCurves[m_dCurveIndex].clear();
                for (int i = 1; i < 761; i++)
                {
                    m_vecCloudCurves[m_dCurveIndex].append(qsl.at(i).toDouble());
                }
                ui->widgetPlot->graph(m_dCurveIndex)->setData(m_vecX,m_vecCloudCurves[m_dCurveIndex]);
                ui->widgetPlot->rescaleAxes();
                ui->widgetPlot->replot();
                m_dCurveIndex++;
                if (m_dCurveIndex >= NO_OF_CURVES)
                {
                    m_dCurveIndex = 0;
                }
            }
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
                ui->statusBar->showMessage("COM port opened : " + comPort, 3000);

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


void MainWindow::saveData()
{

    static QString path;
    QFileDialog qfd;
    QString fileName = qfd.getSaveFileName(this, tr("Save file"),path, tr("CSV (*.csv)"));

    ui->textEditInfo->append(fileName);
    QFile saveFile(fileName);

    if (saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out (&saveFile);

        for (int dataIndex = 0; dataIndex < m_vecCloudCurves[0].count(); dataIndex++)
        {
            QString writeLine;
            for (int curve = 0; curve < ui->widgetPlot->graphCount(); curve++)
            {
                if (m_vecCloudCurves[curve].length() != 0)
                {
                    writeLine.append(QString::number(m_vecCloudCurves[curve].at(dataIndex)));
                    writeLine.append(" , ");
                }
            }
            writeLine.remove(writeLine.length()-3,3);

            ui->textEditInfo->append(writeLine);
            writeLine.append("\r");
            out << writeLine;

        }
        ui->statusBar->showMessage("Saving data to file " + fileName);
        saveFile.close();
    }
    else
    {
        qDebug() << "Failed";
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
