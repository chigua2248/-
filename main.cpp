// main.cpp
#include <QApplication>
#include "timerwidget.h"
#include "configmanager.h"
#include "configdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建 ConfigManager 实例并加载配置
    ConfigManager configManager;
    configManager.loadConfig();

    // 创建并显示 ConfigDialog
    ConfigDialog configDialog(configManager);
    int result = configDialog.exec(); // 以模态方式显示对话框

    if (result == QDialog::Accepted) {
        // 用户点击“确定”，保存配置
        configManager.saveConfig();

        // 创建并显示主定时器窗口
        TimerWidget timer(nullptr, true); // 主定时器允许创建子定时器
        timer.show();

        return app.exec();
    } else {
        // 用户点击“取消”，退出程序
        return 0;
    }
}
