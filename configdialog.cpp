#include "configdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>

ConfigDialog::ConfigDialog(ConfigManager &configManager, QWidget *parent)
    : QDialog(parent), configManager(configManager)
{
    setWindowTitle("设置配置");

    timerColor = configManager.getTimerColor();
    promptColor = configManager.getPromptColor();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 开始/暂停键
    QHBoxLayout *startKeyLayout = new QHBoxLayout();
    QLabel *startKeyLabel = new QLabel("开始/暂停键:", this);
    startKeyEdit = new QKeySequenceEdit(this);
    QKeySequence startSeq(configManager.getStartKey());
    startKeyEdit->setKeySequence(startSeq);
    startKeyLayout->addWidget(startKeyLabel);
    startKeyLayout->addWidget(startKeyEdit);
    mainLayout->addLayout(startKeyLayout);

    // 重置键
    QHBoxLayout *resetKeyLayout = new QHBoxLayout();
    QLabel *resetKeyLabel = new QLabel("重置键:", this);
    resetKeyEdit = new QKeySequenceEdit(this);
    QKeySequence resetSeq(configManager.getResetKey());
    resetKeyEdit->setKeySequence(resetSeq);
    resetKeyLayout->addWidget(resetKeyLabel);
    resetKeyLayout->addWidget(resetKeyEdit);
    mainLayout->addLayout(resetKeyLayout);

    // 退出键
    QHBoxLayout *exitKeyLayout = new QHBoxLayout();
    QLabel *exitKeyLabel = new QLabel("退出键:", this);
    exitKeyEdit = new QKeySequenceEdit(this);
    QKeySequence exitSeq(configManager.getExitKey());
    exitKeyEdit->setKeySequence(exitSeq);
    exitKeyLayout->addWidget(exitKeyLabel);
    exitKeyLayout->addWidget(exitKeyEdit);
    mainLayout->addLayout(exitKeyLayout);

    // 计时器颜色
    QHBoxLayout *timerColorLayout = new QHBoxLayout();
    QLabel *timerColorLabel = new QLabel("计时器颜色:", this);
    timerColorButton = new QPushButton(this);
    timerColorButton->setStyleSheet(QString("background-color: %1").arg(timerColor.name()));
    connect(timerColorButton, &QPushButton::clicked, this, &ConfigDialog::chooseTimerColor);
    timerColorLayout->addWidget(timerColorLabel);
    timerColorLayout->addWidget(timerColorButton);
    mainLayout->addLayout(timerColorLayout);

    // 提示文本颜色
    QHBoxLayout *promptColorLayout = new QHBoxLayout();
    QLabel *promptColorLabel = new QLabel("提示文本颜色:", this);
    promptColorButton = new QPushButton(this);
    promptColorButton->setStyleSheet(QString("background-color: %1").arg(promptColor.name()));
    connect(promptColorButton, &QPushButton::clicked, this, &ConfigDialog::choosePromptColor);
    promptColorLayout->addWidget(promptColorLabel);
    promptColorLayout->addWidget(promptColorButton);
    mainLayout->addLayout(promptColorLayout);

    // 计时器字体大小
    QHBoxLayout *timerFontSizeLayout = new QHBoxLayout();
    QLabel *timerFontSizeLabel = new QLabel("计时器字体大小:", this);
    timerFontSizeSpin = new QSpinBox(this);
    timerFontSizeSpin->setRange(10, 100);
    timerFontSizeSpin->setValue(configManager.getTimerFontSize());
    timerFontSizeLayout->addWidget(timerFontSizeLabel);
    timerFontSizeLayout->addWidget(timerFontSizeSpin);
    mainLayout->addLayout(timerFontSizeLayout);

    // 提示文本字体大小
    QHBoxLayout *promptFontSizeLayout = new QHBoxLayout();
    QLabel *promptFontSizeLabel = new QLabel("提示文本字体大小:", this);
    promptFontSizeSpin = new QSpinBox(this);
    promptFontSizeSpin->setRange(10, 100);
    promptFontSizeSpin->setValue(configManager.getPromptFontSize());
    promptFontSizeLayout->addWidget(promptFontSizeLabel);
    promptFontSizeLayout->addWidget(promptFontSizeSpin);
    mainLayout->addLayout(promptFontSizeLayout);

    // 对话框按钮
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    // 设置“确定”和“取消”按钮的文本为中文
    buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfigDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigDialog::reject);
    mainLayout->addWidget(buttonBox);
    // 添加作者标签上方的间距
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    // ----------------------------
    // 添加作者标签
    // ----------------------------
    QLabel *authorLabel = new QLabel(this);
    authorLabel->setText("<a href='https://example.com' style='color:black; text-decoration:none;'>作者: 吃瓜mua</a>");
    authorLabel->setAlignment(Qt::AlignCenter); // 居中对齐
    authorLabel->setTextFormat(Qt::RichText); // 启用富文本格式
    authorLabel->setTextInteractionFlags(Qt::TextBrowserInteraction); // 使链接可点击
    authorLabel->setOpenExternalLinks(true); // 使链接打开外部浏览器
    mainLayout->addWidget(authorLabel);

}

void ConfigDialog::chooseTimerColor()
{
    QColor color = QColorDialog::getColor(timerColor, this, "选择计时器颜色");
    if (color.isValid()) {
        timerColor = color;
        timerColorButton->setStyleSheet(QString("background-color: %1").arg(timerColor.name()));
    }
}

void ConfigDialog::choosePromptColor()
{
    QColor color = QColorDialog::getColor(promptColor, this, "选择提示文本颜色");
    if (color.isValid()) {
        promptColor = color;
        promptColorButton->setStyleSheet(QString("background-color: %1").arg(promptColor.name()));
    }
}

void ConfigDialog::accept()
{
    // 获取并设置开始键
    QKeySequence startKeySeq = startKeyEdit->keySequence();
    if (!startKeySeq.isEmpty()) {
        configManager.setStartKey(startKeySeq[0]); // 获取第一个键
    }
    else {
        QMessageBox::warning(this, "配置错误", "开始/暂停键不能为空。");
        return;
    }

    // 获取并设置重置键
    QKeySequence resetKeySeq = resetKeyEdit->keySequence();
    if (!resetKeySeq.isEmpty()) {
        configManager.setResetKey(resetKeySeq[0]); // 获取第一个键
    }
    else {
        QMessageBox::warning(this, "配置错误", "重置键不能为空。");
        return;
    }

    // 获取并设置退出键
    QKeySequence exitKeySeq = exitKeyEdit->keySequence();
    if (!exitKeySeq.isEmpty()) {
        configManager.setExitKey(exitKeySeq[0]); // 获取第一个键
    }
    else {
        QMessageBox::warning(this, "配置错误", "退出键不能为空。");
        return;
    }

    // 检查键位冲突
    if (configManager.getStartKey() == configManager.getResetKey() ||
        configManager.getStartKey() == configManager.getExitKey() ||
        configManager.getResetKey() == configManager.getExitKey()) {
        // 提示用户键位冲突
        QMessageBox::warning(this, "配置错误", "开始/暂停键、重置键和退出键必须互不相同。");
        return;  // 阻止保存配置并关闭对话框
    }

    // 保存颜色和字体大小
    configManager.setTimerColor(timerColor);
    configManager.setPromptColor(promptColor);
    configManager.setTimerFontSize(timerFontSizeSpin->value());
    configManager.setPromptFontSize(promptFontSizeSpin->value());

    // 保存配置
    configManager.saveConfig();

    QDialog::accept();
}
