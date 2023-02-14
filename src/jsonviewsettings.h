//
// Created by Zh1an on 2023/2/11.
//

#ifndef NDD_JSON_VIEWER_JSONVIEWSETTINGS_H
#define NDD_JSON_VIEWER_JSONVIEWSETTINGS_H

#include <QWidget>

#include <define.h>
#include <jsonhandler.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
class JsonViewSettings;
}
QT_END_NAMESPACE

class QSettings;

class JsonViewSettings : public QWidget
{
    Q_OBJECT

public:
    explicit JsonViewSettings(const QString &pluginPath, QWidget *parent = nullptr);
    ~JsonViewSettings() override;

public:
    [[nodiscard]] const Setting &getPluginSetting() const { return pluginSetting_; }

private slots:
    void slot_ConfirmClicked();

private:
    void init(const QString &pluginPath);
    void setIndentation(int var);
    void setLineEnding(int var);
    void setLineFormatting(int var);

    [[nodiscard]] int getIndentation() const;
    [[nodiscard]] int getLineEnding() const;
    [[nodiscard]] int getLineFormatting() const;

    void initConnection();

private:
    Ui::JsonViewSettings *ui;

    QSettings *configSetting_;

    Setting pluginSetting_;
};

#endif  // NDD_JSON_VIEWER_JSONVIEWSETTINGS_H
