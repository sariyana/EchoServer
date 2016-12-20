#include "mainview.h"
#include "ui_mainview.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

static int PORT_NUMBER = 23;        //set the port number to be 23, makes easier to determine later
static int WAIT_FOR_DATA_MS = 200;

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView),
    m_server(new QTcpServer(this)),
    m_socket(nullptr)
{
    ui->setupUi(this);
    ui->btnStopServer->setEnabled(false);
    connect(m_server, & QTcpServer::newConnection, this, &MainView::ExchangeData);
}

MainView::~MainView()
{
    delete ui;
}

void MainView::on_btnStartServer_clicked()
{
    if (StartServer())
    {
        ui->btnStartServer->setEnabled(false);
        ui->btnStopServer->setEnabled(true);
    }
}

void MainView::on_btnStopServer_clicked()
{
    StopServer();
    ui->btnStartServer->setEnabled(true);
    ui->btnStopServer->setEnabled(false);
}

bool MainView::StartServer()
{
    //makes the server listen for a tcp/ip address
    bool result = m_server->listen(QHostAddress::Any, PORT_NUMBER);

    //if result fails, inform the user
    if (!result)
    {
        QMessageBox::critical(this, "Echo Server", "Unable to start the server: %1");
        return false;
    }
    return true;
}

void MainView::StopServer()
{
    m_server->close();
    if (m_socket != nullptr && m_socket -> isOpen())
    {
        m_socket->close();
    }
}

void MainView::ExchangeData()
{
    m_socket = m_server->nextPendingConnection();
    if (m_socket->isOpen())
    {
        connect(m_socket, &QTcpSocket::readyRead, this, &MainView::EchoReadData);
    }
}

//Read the data and send it back wrapped in tags to ensure that it came from the proper server
void MainView::EchoReadData()
{
    m_socket->write("<echoserver>n");
    QString result;
    while (!m_socket->atEnd())
    {
        result.append(m_socket->readAll());
        m_socket->waitForReadyRead(WAIT_FOR_DATA_MS);
    }
    m_socket->write(qPrintable(result));
    m_socket->write("\n</echoserver>\n");
}
