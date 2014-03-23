#include "QWeiboAPI/weibo.h"
#include "QWeiboAPI/requestparameter.h"
#include <QInputDialog>
#include <QApplication>
#include <QMessageBox>
#include <QTextEdit>

using namespace QWeiboAPI;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QString user, passwd;
    if (app.arguments().size() == 3) {
        user = app.arguments().at(1);
        passwd = app.arguments().at(2);
    } else {
        user = QInputDialog::getText(0, "QWeiboAPI Login", "User name");
        passwd = QInputDialog::getText(0, "QWeiboAPI Login", "Password", QLineEdit::Password);
    }
    if (user.isEmpty() || passwd.isEmpty()) {
        QMessageBox::critical(0, "QWeiboAPI Login", "User or password is empty");
        return 1;
    }
    QMessageBox okbox, failbox;
    okbox.setWindowTitle("QWeiboAPI Login");
    okbox.setText("Ok");
    failbox.setWindowTitle("QWeiboAPI Login");
    failbox.setText("Failed");
    QTextEdit *txt = new QTextEdit();
    txt->show();
    txt->setWindowTitle("Sina weibo api result");

    Weibo weibo;
    QObject::connect(&weibo, SIGNAL(ok(QString)), txt, SLOT(append(QString)));
    QObject::connect(&weibo, SIGNAL(loginFail()), &failbox, SLOT(exec()));
    weibo.setUSer(user);
    weibo.setPassword(passwd);
    //weibo.login();
    Request *request = new statuses_public_timeline();
    request->prepare();
    weibo.createRequest(request);

    return app.exec();
}
