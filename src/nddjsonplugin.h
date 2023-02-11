//
// Created by Zh1an on 2023/2/11.
//

#ifndef NDD_JSON_VIEWER_NDDJSONPLUGIN_H
#define NDD_JSON_VIEWER_NDDJSONPLUGIN_H

#include <QObject>
#include <QWidget>

#include <jsonhandler.h>

class QsciScintilla;
class ScintillaEditor;
class JsonViewSettings;

class NDDJsonPlugin : public QObject
{
    Q_OBJECT
public:
    explicit NDDJsonPlugin(QWidget *mainWidget, const QString &pluginPath, QsciScintilla *pEdit,
                           QObject *parent = nullptr);
    ~NDDJsonPlugin() override = default;

private:
    void addJsonViewMenu();
    static int showMessage(const std::string &title, const std::string &msg, int flag, bool bDontShow = false);

private:
    [[nodiscard]] auto GetFormatSetting() const -> std::tuple<LE, LF, char, unsigned>;
    void reportError(const Result &result);

    void formattingJson();
    void compressJson();

private:
    JsonViewSettings *jsonViewSettings_;
    ScintillaEditor *scintillaEditor_;

    QWidget *mainWidget_;
};

#endif  // NDD_JSON_VIEWER_NDDJSONPLUGIN_H
