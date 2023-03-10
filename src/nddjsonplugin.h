//
// Created by Zh1an on 2023/2/11.
//

#ifndef NDD_JSON_VIEWER_NDDJSONPLUGIN_H
#define NDD_JSON_VIEWER_NDDJSONPLUGIN_H

#include <QAction>
#include <QObject>
#include <QWidget>

#include <jsonhandler.h>

class QsciScintilla;
class ScintillaEditor;
class JsonViewSettings;
class QDockWidget;
class QJsonModel;
class QTreeView;

class NDDJsonPlugin : public QObject
{
    Q_OBJECT
public:
    explicit NDDJsonPlugin(QWidget *mainWidget, const QString &pluginPath, QsciScintilla *pEdit,
                           QObject *parent = nullptr);
    ~NDDJsonPlugin() override = default;

public:
    void getJsonViewMenu(QMenu *menu);
    void setScintilla(const std::function<QsciScintilla *()> &cb);

private:
    static int showMessage(const std::string &title, const std::string &msg, int flag, bool bDontShow = false);

private:
    [[nodiscard]] auto GetFormatSetting() const -> std::tuple<LE, LF, char, unsigned>;
    void reportError(const Result &result);

    void formattingJson();
    void compressJson();

    void refreshTableJson();
    void validateJson();
    void findNode(const QString &str);
    bool searchNode(QModelIndex index, const QString& str);

private:
    JsonViewSettings *jsonViewSettings_;
    ScintillaEditor *scintillaEditor_;

    QWidget *mainWidget_;

    QDockWidget *dockWidget_;
    QJsonModel *jsonModel_;
    QTreeView *treeView_;
};

#endif  // NDD_JSON_VIEWER_NDDJSONPLUGIN_H
