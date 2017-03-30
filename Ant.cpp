#include "Ant.h"

#include "CheckboxTable.h"
#include "CheckboxHeaderView.h"
#include <QTimerEvent>
#include <QIcon>
#include <QStringList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QProcess>
#include <QString>
#include <QRegExp>

Ant::Ant(QWidget *parent)
    : QMainWindow(parent)
{
    m_table = new CheckboxTable(10, 5);

    m_table->setHeaderString(m_headerTitle);
    m_table->setHeaderWidth(m_headerWidth);

    setFixedSize(1300, 800);

    setWindowIcon(QIcon(":/icon/icon/ant_128px.ico"));

    QHBoxLayout *handleLayout = new QHBoxLayout();
    QVBoxLayout *bodyLayout   = new QVBoxLayout();
    QVBoxLayout *mainLayout   = new QVBoxLayout();

    //初始化按钮
    m_addrEdit = new QLineEdit();
    m_addrEdit->setFixedWidth(220);

    m_addrLabel         = new QLabel("集群地址：");
    m_updateButton      = new QPushButton("更新列表");
    m_syncButton        = new QPushButton("同步");

    handleLayout->addWidget(m_addrLabel);
    handleLayout->addWidget(m_addrEdit);
    handleLayout->addWidget(m_updateButton);
    handleLayout->addWidget(m_syncButton);
    handleLayout->addStretch();

    bodyLayout->addWidget(m_table);

    mainLayout->addLayout(handleLayout);
    mainLayout->addLayout(bodyLayout);

    m_mainWidget = new QWidget();
    m_mainWidget->setLayout(mainLayout);
    setCentralWidget(m_mainWidget);

    connect(m_syncButton, SIGNAL(clicked(bool)), this, SLOT(syncSvnChangeFile()));
}

Ant::~Ant()
{

}

void Ant::timerEvent(QTimerEvent *event)
{

}

void Ant::syncSvnChangeFile()
{
    m_svnAddr = m_addrEdit->text();
    QStringList changeList = getSvnChangeList(m_svnAddr);
    int changeCount = changeList.count();
    for(int i = 0; i< changeCount; i++)
    {
        m_table->setItem(i, 1, new QTableWidgetItem(changeList.at(i)));
    }
}

QStringList Ant::getSvnChangeList(QString &path)
{
    QString cmd("git");
    QStringList param = {
        "-C",
        path,
        "status",
        "-s"
    };

    QProcess process;
    process.start(cmd, param);
    process.waitForStarted();
    process.waitForFinished();

    QString changeString = process.readAllStandardOutput();
    QStringList fileList;
    QStringList changeList = changeString.split("\n");
    int changeCount = changeList.count();
    for(int i=0;i < changeCount; i++)
    {
        QStringList entryList = changeList.at(i).split(QRegExp("\s*"));
        if(entryList.count() < 4)
        {
            continue;
        }
        fileList.append(entryList.at(3));
    }


    return fileList;
}
