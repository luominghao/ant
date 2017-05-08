#ifndef ANTCOMMANDER_H
#define ANTCOMMANDER_H

#include <QString>
#include <QStringList>


class AntCommander
{
public:
    AntCommander();
    AntCommander(const QString &host, const QString &username, const QString &password);

    void init(const QString &host, const QString &username, const QString &password);
    QString find(const QString &path);

    bool exec(const QString &cmd, const QStringList &param, QString &stdout, QString &stderr);
    bool scpTo(const QString &src, const QString &des);
    bool scpFrom(const QString &src, const QString &des);

private:
    LIBSSH2_SESSION *m_session;
    LIBSSH2_CHANNEL *m_channel;
    int m_socket;
    QString m_host, m_username, m_password;

    bool checkConnect();
    bool connect(const QString &host, const QString &username, const QString &password);
};

#endif // ANTCOMMANDER_H