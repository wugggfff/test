#ifndef SECMAINWINDOW_H
#define SECMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SecMainWindow;
}

class SecMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SecMainWindow(QWidget *parent = nullptr);
    ~SecMainWindow();

private:
    Ui::SecMainWindow *ui;
};

#endif // SECMAINWINDOW_H
