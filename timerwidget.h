#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>
#include "configmanager.h"
#include "uimanager.h"

// 前向声明 KeyboardHook，避免循环包含
class KeyboardHook;

class TimerWidget : public QWidget
{
    Q_OBJECT

public:
    // 新增 allowCreateChild 参数，用于控制是否允许创建子计时器
    explicit TimerWidget(QWidget *parent = nullptr, bool allowCreateChild = true);
    ~TimerWidget();

    void startTimer();      // 开始计时
    void resetTimer();      // 重置计时
    void pauseTimer();      // 暂停计时器
    void stopAllTimers();   // 停止所有计时器

    int getStartKey() const;    // 获取开始键
    int getResetKey() const;    // 获取重置键
    int getExitKey() const;     // 获取退出键

private slots:
    void updateTimer();     // 更新计时器显示
    void clearPrompt();     // 清除提示信息

signals:
    void timerReachedThreeMinutes(); // 信号：计时器达到3分钟

private:
    void applyConfig();     // 应用配置到界面

    QLabel *timerLabel;     // 显示计时器
    QLabel *promptLabel;    // 显示提示信息
    QPushButton *exitButton; // 退出按钮（仅主计时器）
    QTimer *promptTimer;    // 控制提示信息显示的定时器
    QTimer *updateTimer_;   // 控制计时器更新的定时器
    int elapsedPausedTime;  // 保存暂停时已消耗的时间
    bool timerRunning;      // 计时器是否在运行
    bool timerPaused;       // 计时器是否暂停
    bool allowCreateChild;  // 是否允许创建子计时器

    ConfigManager configManager;  // 管理配置文件
    UIManager uiManager;          // 管理UI配置

    // 通过前向声明引用 KeyboardHook 类
    KeyboardHook *keyboardHookInstance;

    QElapsedTimer elapsedTimer;    // 计时器

    // 添加一个布局来容纳子 TimerWidgets
    QVBoxLayout *childTimersLayout;

    // 添加一个列表来跟踪子计时器
    QList<TimerWidget*> childTimers;
};

#endif // TIMERWIDGET_H
