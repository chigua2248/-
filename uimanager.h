#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "configmanager.h"
#include <QWidget>

class UIManager
{
public:
    void showConfigUI(ConfigManager &configManager, QWidget *parent = nullptr);
};

#endif // UIMANAGER_H
