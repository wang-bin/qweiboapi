#include "QWeiboAPI/weibo.h"
#include "QWeiboAPI/requestparameter.h"
#include <QInputDialog>
#include <QApplication>
#include <QMessageBox>

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


    Weibo weibo;
    QObject::connect(&weibo, SIGNAL(loginOk()), &okbox, SLOT(exec()));
    QObject::connect(&weibo, SIGNAL(loginFail()), &failbox, SLOT(exec()));
    weibo.setUSer(user);
    weibo.setPassword(passwd);
    //weibo.login();
    Request *request = new statuses_public_timeline();
    request->prepare();
    weibo.createRequest(request);

    return app.exec();
}
