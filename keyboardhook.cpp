#include "keyboardhook.h"
#include "timerwidget.h"
#include <QApplication>
#include <QDebug>

// 初始化静态成员
HHOOK KeyboardHook::keyboardHook = nullptr;
TimerWidget* KeyboardHook::timerWidgetInstance = nullptr;
bool KeyboardHook::startKeyPressed = false;
bool KeyboardHook::resetKeyPressed = false;
bool KeyboardHook::exitKeyPressed = false; // 初始化退出键状态

// Helper function to map Qt::Key to Windows virtual key codes
int qtKeyToVirtualKey(int qtKey)
{
    switch(qtKey){
    case Qt::Key_Escape: return VK_ESCAPE;
    case Qt::Key_F1: return VK_F1;
    case Qt::Key_F2: return VK_F2;
    case Qt::Key_F3: return VK_F3;
    case Qt::Key_F4: return VK_F4;
    case Qt::Key_F5: return VK_F5;
    case Qt::Key_F6: return VK_F6;
    case Qt::Key_F7: return VK_F7;
    case Qt::Key_F8: return VK_F8;
    case Qt::Key_F9: return VK_F9;
    case Qt::Key_F10: return VK_F10;
    case Qt::Key_F11: return VK_F11;
    case Qt::Key_F12: return VK_F12;
        // Add more special keys as needed

    default:
        if(qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
            return 'A' + (qtKey - Qt::Key_A);
        else if(qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9)
            return '0' + (qtKey - Qt::Key_0);
        else
            return 0;
    }
}

void KeyboardHook::setupGlobalKeyboardHooks(TimerWidget *widget)
{
    timerWidgetInstance = widget;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);

    if (!keyboardHook) {
        qDebug() << "无法安装键盘钩子！";
    } else {
        qDebug() << "键盘钩子安装成功。";
    }
}

void KeyboardHook::releaseKeyboardHook()
{
    if (keyboardHook)
    {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = nullptr;
        timerWidgetInstance = nullptr;
        qDebug() << "键盘钩子已卸载。";
    }
}

// 确保使用 CALLBACK 调用约定
LRESULT CALLBACK KeyboardHook::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && timerWidgetInstance)
    {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        int vkCode = p->vkCode;

        // Map Qt::Key to virtual key code
        int qtStartKey = timerWidgetInstance->getStartKey();
        int qtResetKey = timerWidgetInstance->getResetKey();
        int qtExitKey = timerWidgetInstance->getExitKey();

        int startKey = qtKeyToVirtualKey(qtStartKey);
        int resetKey = qtKeyToVirtualKey(qtResetKey);
        int exitKey = qtKeyToVirtualKey(qtExitKey);

        if (wParam == WM_KEYDOWN)
        {
            if (vkCode == startKey && !startKeyPressed)
            {
                startKeyPressed = true;
                timerWidgetInstance->startTimer();  // 启动或暂停计时
                qDebug() << "启动/暂停键按下:" << vkCode;
            }
            else if (vkCode == resetKey && !resetKeyPressed)
            {
                resetKeyPressed = true;
                timerWidgetInstance->resetTimer();  // 重置计时
                qDebug() << "重置键按下:" << vkCode;
            }
            else if (vkCode == exitKey && !exitKeyPressed)
            {
                exitKeyPressed = true;
                qDebug() << "退出键按下:" << vkCode;
                QApplication::quit();  // 退出程序
            }
        }
        else if (wParam == WM_KEYUP)
        {
            if (vkCode == startKey)
            {
                startKeyPressed = false;
            }
            else if (vkCode == resetKey)
            {
                resetKeyPressed = false;
            }
            else if (vkCode == exitKey)
            {
                exitKeyPressed = false;
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}
