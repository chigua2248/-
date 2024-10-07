// keyboardhook.h
#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <windows.h>

class TimerWidget;

class KeyboardHook
{
public:
    static void setupGlobalKeyboardHooks(TimerWidget *widget);
    static void releaseKeyboardHook();

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static HHOOK keyboardHook;
    static TimerWidget* timerWidgetInstance;

    // 添加静态变量跟踪键状态
    static bool startKeyPressed;
    static bool resetKeyPressed;
    static bool exitKeyPressed; // 新增退出键状态
};

#endif // KEYBOARDHOOK_H
