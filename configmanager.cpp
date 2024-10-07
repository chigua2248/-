#include "configmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>

ConfigManager::ConfigManager()
{
    // 使用当前目录
    configFilePath = QDir::currentPath() + "/config.json";
    qDebug() << "初始路径:"<< configFilePath;
    // 初始化默认值
    startKey = 'R';
    resetKey = 'X';
    exitKey = Qt::Key_Escape;
    timerColor = QColor("#0000FF");
    promptColor = QColor("#FF0000");
    timerFontSize = 24;
    promptFontSize = 20;
}

void ConfigManager::loadConfig()
{
    QFile file(configFilePath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull()) {
            QJsonObject obj = doc.object();
            startKey = obj.value("startKey").toInt(startKey);
            resetKey = obj.value("resetKey").toInt(resetKey);
            exitKey = obj.value("exitKey").toInt(exitKey); // 加载退出键
            timerColor = QColor(obj.value("timerColor").toString(timerColor.name()));
            promptColor = QColor(obj.value("promptColor").toString(promptColor.name()));
            timerFontSize = obj.value("timerFontSize").toInt(timerFontSize);
            promptFontSize = obj.value("promptFontSize").toInt(promptFontSize);
        }
        else {
            qDebug() << "配置文件解析错误，使用默认值";
        }
    } else {
        qDebug() << "配置文件不存在，使用默认值";
        saveConfig(); // 创建默认配置文件
    }
}

void ConfigManager::saveConfig()
{
    QJsonObject obj;
    obj["startKey"] = startKey;
    obj["resetKey"] = resetKey;
    obj["exitKey"] = exitKey; // 保存退出键
    obj["timerColor"] = timerColor.name();
    obj["promptColor"] = promptColor.name();
    obj["timerFontSize"] = timerFontSize;
    obj["promptFontSize"] = promptFontSize;

    QFile file(configFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "无法写入配置文件";
    }
}

// Getter 和 Setter 方法
int ConfigManager::getStartKey() const { return startKey; }
int ConfigManager::getResetKey() const { return resetKey; }
int ConfigManager::getExitKey() const { return exitKey; } // 新增 getter
QColor ConfigManager::getTimerColor() const { return timerColor; }
QColor ConfigManager::getPromptColor() const { return promptColor; }
int ConfigManager::getTimerFontSize() const { return timerFontSize; }
int ConfigManager::getPromptFontSize() const { return promptFontSize; }

void ConfigManager::setStartKey(int key) { startKey = key; }
void ConfigManager::setResetKey(int key) { resetKey = key; }
void ConfigManager::setExitKey(int key) { exitKey = key; } // 新增 setter
void ConfigManager::setTimerColor(const QColor &color) { timerColor = color; }
void ConfigManager::setPromptColor(const QColor &color) { promptColor = color; }
void ConfigManager::setTimerFontSize(int size) { timerFontSize = size; }
void ConfigManager::setPromptFontSize(int size) { promptFontSize = size; }
