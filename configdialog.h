#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H
#include <QLabel>
#include <QDialog>
#include <QKeySequenceEdit> // 使用 QKeySequenceEdit
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include <QColor>
#include "configmanager.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(ConfigManager &configManager, QWidget *parent = nullptr);

private slots:
    void chooseTimerColor();
    void choosePromptColor();
    void accept() override;

private:
    ConfigManager &configManager;

    QKeySequenceEdit *startKeyEdit; // 使用 QKeySequenceEdit
    QKeySequenceEdit *resetKeyEdit; // 使用 QKeySequenceEdit
    QKeySequenceEdit *exitKeyEdit;  // 新增退出键编辑框
    QPushButton *timerColorButton;
    QPushButton *promptColorButton;
    QSpinBox *timerFontSizeSpin;
    QSpinBox *promptFontSizeSpin;

    QColor timerColor;
    QColor promptColor;
    // 新增作者标签
    QLabel *authorLabel;
    void setupUI();
};

#endif // CONFIGDIALOG_H
