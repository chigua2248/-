#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QColor>

class ConfigManager
{
public:
    ConfigManager();
    void loadConfig();
    void saveConfig();

    int getStartKey() const;
    int getResetKey() const;
    int getExitKey() const; // 新增退出键的获取方法
    QColor getTimerColor() const;
    QColor getPromptColor() const;
    int getTimerFontSize() const;
    int getPromptFontSize() const;

    void setStartKey(int key);
    void setResetKey(int key);
    void setExitKey(int key); // 新增退出键的设置方法
    void setTimerColor(const QColor &color);
    void setPromptColor(const QColor &color);
    void setTimerFontSize(int size);
    void setPromptFontSize(int size);

private:
    QString configFilePath;
    int startKey;
    int resetKey;
    int exitKey; // 新增退出键变量
    QColor timerColor;
    QColor promptColor;
    int timerFontSize;
    int promptFontSize;
};

#endif // CONFIGMANAGER_H
