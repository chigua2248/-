#include "timerwidget.h"
#include "keyboardhook.h"
#include <QApplication>
#include <QDebug>
#include <QPushButton>

TimerWidget::TimerWidget(QWidget *parent, bool allowCreateChild)
    : QWidget(parent),
    elapsedPausedTime(0),
    timerRunning(false),
    timerPaused(false),
    keyboardHookInstance(nullptr),
    allowCreateChild(allowCreateChild)
{
    // 加载配置文件
    configManager.loadConfig();

    // 设置窗口属性 (透明背景、窗口置顶等)
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 设置布局和UI元素
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(2);


    // 初始化计时器标签
    timerLabel = new QLabel("00:00", this);
    timerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(timerLabel);

    // 初始化提示文本标签
    promptLabel = new QLabel("", this);
    promptLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(promptLabel);

    // 初始化 childTimersLayout（仅在允许创建子计时器的主 TimerWidget 中添加）
    if (allowCreateChild)
    {
        childTimersLayout = new QVBoxLayout();
        mainLayout->addLayout(childTimersLayout);
    }

    // 初始化 Exit 按钮（仅在允许创建子计时器的主 TimerWidget 中添加）
    if (allowCreateChild)
    {
        exitButton = new QPushButton("退出", this);
        connect(exitButton, &QPushButton::clicked, QApplication::quit);
        mainLayout->addWidget(exitButton);
    }

    setLayout(mainLayout);

    // 将配置文件中的设置应用到UI界面
    applyConfig();

    // 设置计时器和提示信息定时器
    updateTimer_ = new QTimer(this);
    connect(updateTimer_, &QTimer::timeout, this, &TimerWidget::updateTimer);

    promptTimer = new QTimer(this);
    promptTimer->setSingleShot(true);  // 设为一次性定时器，3秒后清除提示信息
    connect(promptTimer, &QTimer::timeout, this, &TimerWidget::clearPrompt);

    // 将窗口位置设置在左上角
    this->move(10, 10);

    // 初始化 QElapsedTimer
    elapsedTimer.start();  // 启动计时器

    // 仅当允许创建子计时器时（即主计时器），设置键盘钩子
    if (allowCreateChild)
    {
        // 设置键盘钩子来监听按键事件
        KeyboardHook::setupGlobalKeyboardHooks(this);
    }
}

TimerWidget::~TimerWidget()
{
    // 仅当允许创建子计时器时（即主计时器），释放键盘钩子
    if (allowCreateChild)
    {
        KeyboardHook::releaseKeyboardHook();
    }

    // 删除所有子计时器
    qDeleteAll(childTimers);
    childTimers.clear();
}

void TimerWidget::applyConfig()
{
    // 更新计时器标签样式（颜色和字体大小）
    QFont timerFont("Arial", configManager.getTimerFontSize());
    timerLabel->setFont(timerFont);
    timerLabel->setStyleSheet(QString("color: %1;").arg(configManager.getTimerColor().name()));

    // 更新提示文本标签样式（颜色和字体大小）
    QFont promptFont("Arial", configManager.getPromptFontSize());
    promptLabel->setFont(promptFont);
    promptLabel->setStyleSheet(QString("color: %1;").arg(configManager.getPromptColor().name()));
}

void TimerWidget::updateTimer()
{
    if (timerRunning && !timerPaused)
    {
        // 计算经过的秒数
        int elapsedSeconds = (elapsedTimer.elapsed() / 1000) + elapsedPausedTime;
        int minutes = elapsedSeconds / 60;
        int seconds = elapsedSeconds % 60;
        timerLabel->setText(QString("%1:%2")
                                .arg(minutes, 2, 10, QChar('0'))
                                .arg(seconds, 2, 10, QChar('0')));

        // 检查是否达到3分钟
        if (elapsedSeconds >= 180)
        {
            // 停止计时器
            pauseTimer();

            if (allowCreateChild)
            {
                // 提示用户计时达到3分钟并创建新计时器
                promptLabel->setText("创建新计时器");
                qDebug() << "计时已达到3分钟，创建新计时器";

                // 调整窗口大小
                this->adjustSize();

                // 创建新的子计时器
                TimerWidget *newTimer = new TimerWidget(this, false); // 设置 allowCreateChild 为 false，防止递归创建
                childTimersLayout->addWidget(newTimer);
                childTimers.append(newTimer); // 跟踪子计时器
                newTimer->startTimer();
            }
            else
            {
                // 对于子计时器，仅提示已达到3分钟
                promptLabel->setText("已达到3分钟");
                qDebug() << "子计时器已达到3分钟，停止计时。";

                // 调整窗口大小
                this->adjustSize();
            }
        }
    }
}

void TimerWidget::startTimer()
{
    if (!timerRunning)
    {
        timerRunning = true;
        timerPaused = false;
        elapsedTimer.start();  // 启动 QElapsedTimer
        elapsedPausedTime = 0;  // 重置暂停时间
        updateTimer_->start(100);  // 启动计时器
        promptLabel->setText("计时开始");
        qDebug() << "计时开始:" << elapsedTimer.elapsed() << "ms";
        promptTimer->start(3000);
    }
    else if (timerPaused)
    {
        // 如果计时器处于暂停状态，则恢复计时
        timerPaused = false;
        elapsedTimer.restart();  // 重新启动 QElapsedTimer
        updateTimer_->start(100);          // 重新启动计时器更新
        promptLabel->setText("计时继续");
        qDebug() << "计时继续:" << elapsedTimer.elapsed() << "ms";
        promptTimer->start(3000);
    }
    else
    {
        // 启动暂停功能
        pauseTimer();
    }
}

void TimerWidget::pauseTimer()
{
    if (timerRunning && !timerPaused)
    {
        timerPaused = true;
        updateTimer_->stop();  // 停止计时器更新
        elapsedPausedTime += elapsedTimer.elapsed() / 1000;  // 记录已运行的时间（秒）
        promptLabel->setText("计时已暂停");
        qDebug() << "计时已暂停, 已运行秒数:" << elapsedPausedTime;
        promptTimer->start(3000);

        // 调整窗口大小
        this->adjustSize();
    }
}

void TimerWidget::resetTimer()
{
    timerRunning = false;
    timerPaused = false;
    updateTimer_->stop();
    elapsedPausedTime = 0;
    timerLabel->setText("00:00");
    promptLabel->setText("计时已重置");
    qDebug() << "计时已重置";
    promptTimer->start(3000);

    // 调整窗口大小
    this->adjustSize();

    // 删除所有子计时器
    if (!childTimers.isEmpty())
    {
        for (TimerWidget* child : childTimers)
        {
            // 停止子计时器的计时器
            child->updateTimer_->stop();
            child->promptTimer->stop();
            child->timerRunning = false;
            child->timerPaused = false;
            child->elapsedPausedTime = 0;
            child->timerLabel->setText("00:00");
            child->promptLabel->setText("");

            // 断开子计时器的所有信号槽连接
            disconnect(child, nullptr, nullptr, nullptr);

            // 从布局中移除子计时器
            if (child->parent() == this)
            {
                child->setParent(nullptr);
            }

            // 删除子计时器
            child->deleteLater();
        }
        childTimers.clear();
    }

    // 从布局中移除子计时器
    if (childTimersLayout)
    {
        QLayoutItem *item;
        while ((item = childTimersLayout->takeAt(0)) != nullptr)
        {
            delete item;
        }
    }
}

void TimerWidget::clearPrompt()
{
    promptLabel->setText("");
    this->adjustSize();
}

void TimerWidget::stopAllTimers()
{
    // 停止所有定时器
    if (updateTimer_->isActive()) {
        updateTimer_->stop();
    }
    if (promptTimer->isActive()) {
        promptTimer->stop();
    }

    // 重置计时器状态
    timerRunning = false;
    timerPaused = false;
    elapsedPausedTime = 0;
    timerLabel->setText("00:00");
    promptLabel->setText("程序已退出");
    qDebug() << "所有定时器已停止，程序即将退出。";

    // 仅当允许创建子计时器时（即主计时器），释放键盘钩子
    if (allowCreateChild)
    {
        KeyboardHook::releaseKeyboardHook();
    }

    // 删除所有子计时器
    if (!childTimers.isEmpty())
    {
        for (TimerWidget* child : childTimers)
        {
            child->stopAllTimers(); // 确保子计时器停止并释放资源
        }
        childTimers.clear();
    }

    // 从布局中移除子计时器
    if (childTimersLayout)
    {
        QLayoutItem *item;
        while ((item = childTimersLayout->takeAt(0)) != nullptr)
        {
            delete item;
        }
    }

    // 关闭主窗口
    this->close();

    // 退出应用程序
    QApplication::quit();
}

// 实现 getStartKey、getResetKey 和 getExitKey 方法
int TimerWidget::getStartKey() const
{
    return configManager.getStartKey();
}

int TimerWidget::getResetKey() const
{
    return configManager.getResetKey();
}

int TimerWidget::getExitKey() const
{
    return configManager.getExitKey();
}
