#include "uimanager.h"
#include "configdialog.h"
#include <QCoreApplication>
#include "timerwidget.h"
#include "keyboardhook.h"  // 添加这一行以包含 KeyboardHook 的声明

void UIManager::showConfigUI(ConfigManager &configManager, QWidget *parent)
{
    ConfigDialog dialog(configManager, parent);

    // 调用 exec()，程序会阻塞在这里，直到用户点击“确定”或“取消”
    int result = dialog.exec();

    // 如果用户点击了“取消”，先停止所有定时器并退出程序
    if (result == QDialog::Rejected) {
        // 强制停止计时器
        TimerWidget *timerWidget = qobject_cast<TimerWidget*>(parent);
        if (timerWidget) {
            timerWidget->stopAllTimers();  // 手动停止所有计时器
        }

        // 释放键盘钩子
        KeyboardHook::releaseKeyboardHook();

        // 然后退出程序
        QCoreApplication::quit();
    }
}
