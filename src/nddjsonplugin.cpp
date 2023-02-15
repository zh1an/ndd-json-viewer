//
// Created by Zh1an on 2023/2/11.
//

#include "nddjsonplugin.h"

#include <docktitlewidget.h>
#include <jsonviewsettings.h>
#include <qjsonmodel.h>
#include <scintillaeditor.h>

#include <QDockWidget>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QTreeView>

NDDJsonPlugin::NDDJsonPlugin(QWidget *mainWidget, const QString &pluginPath, QsciScintilla *pEdit, QObject *parent)
    : QObject(parent),
      scintillaEditor_(nullptr),
      jsonViewSettings_(new JsonViewSettings(pluginPath)),
      mainWidget_(mainWidget),
      dockWidget_(new QDockWidget),
      jsonModel_(new QJsonModel),
      treeView_(new QTreeView)
{
    dockWidget_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);

    dockWidget_->setAllowedAreas(Qt::LeftDockWidgetArea);
    dockWidget_->hide();

    auto dockWidgetTitle = new DockTitleWidget;
    dockWidget_->setTitleBarWidget(dockWidgetTitle);

    connect(dockWidgetTitle, &DockTitleWidget::sigRefreshClicked, this, [this] { refreshTableJson(); });
    connect(dockWidgetTitle, &DockTitleWidget::sigValidateClicked, this, [this] { validateJson(); });
    connect(dockWidgetTitle, &DockTitleWidget::sigFormatClicked, this, [this] { formattingJson(); });
    connect(dockWidgetTitle, &DockTitleWidget::sigCloseClicked, this, [this] { dockWidget_->hide(); });
    connect(dockWidgetTitle, &DockTitleWidget::sigFindClicked, this, [this](const QString &str) { findNode(str); });

    treeView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView_->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    treeView_->setModel(jsonModel_);

    dockWidget_->setWidget(treeView_);

    auto mainWindow = dynamic_cast<QMainWindow *>(mainWidget);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_);
}
void NDDJsonPlugin::getJsonViewMenu(QMenu *menu)
{
    menu->addAction("Show Json Table", this, [this] {
        refreshTableJson();
        dockWidget_->show();
    });

    menu->addAction(
        "Formatting Json(Ctrl+F8)", this, [this] { formattingJson(); }, Qt::CTRL + Qt::Key_F8);
    menu->addAction(
        "Compress Json(Ctrl+F9)", this, [this] { compressJson(); }, Qt::CTRL + Qt::Key_F9);
    menu->addAction("Settings", this, [this] { this->jsonViewSettings_->show(); });
}

auto NDDJsonPlugin::GetFormatSetting() const -> std::tuple<LE, LF, char, unsigned>
{
    if (!scintillaEditor_)
    {
        return {};
    }

    LE le = LE::kCrLf;
    LF lf = LF::kFormatDefault;
    char indentChar = ' ';
    unsigned indentLen = 0;

    auto setting_ = jsonViewSettings_->getPluginSetting();

    // Line formatting options
    lf = static_cast<LF>(setting_.lineFormat);

    // End of line options
    switch (setting_.lineEnding)
    {
        case LineEnding::WINDOWS:
            le = LE::kCrLf;
            break;

        case LineEnding::UNIX:
            le = LE::kLf;
            break;

        case LineEnding::MAC:
            le = LE::kCr;
            break;

        case LineEnding::AUTO:
        default: {
            const auto eol = scintillaEditor_->getEOL();
            switch (eol)
            {
                case 0:
                    le = LE::kCrLf;
                    break;
                case 1:
                    le = LE::kCr;
                    break;
                default:
                    le = LE::kLf;
                    break;
            }
        }
    }

    // Indentation options
    switch (setting_.indent.style)
    {
        case IndentStyle::TAB:
            indentChar = '\t';
            indentLen = 1;
            break;

        case IndentStyle::SPACE:
            indentChar = ' ';
            indentLen = setting_.indent.len;
            break;

        case IndentStyle::AUTO:
        default:
            const auto [c, l] = scintillaEditor_->getIndent();
            indentChar = '\t';
            indentLen = 1;
            break;
    }

    return {le, lf, indentChar, indentLen};
}
void NDDJsonPlugin::reportError(const Result &result)
{
    if (!scintillaEditor_)
    {
        showMessage("Error", "Editor is invalid.", 1);
        return;
    }

    auto start = scintillaEditor_->getSelectionStart() + result.error_pos;
    auto end = start + scintillaEditor_->getSelectionEnd();

    scintillaEditor_->makeSelection(start, end);

    // Intimate user
    std::string err;
    err += "\n\nError: (" + std::to_string(result.error_code) + " : " + result.error_str + ")";

    showMessage(
        "JSON Viewer: Error",
        std::string(
            "There was an error while parsing JSON. Refer to the current selection for possible problematic area.\n") +
            err,
        1);
}
int NDDJsonPlugin::showMessage(const std::string &title, const std::string &msg, int flag, bool bDontShow)
{
    return QMessageBox::warning(nullptr, title.c_str(), msg.c_str());
}
void NDDJsonPlugin::formattingJson()
{
    if (!scintillaEditor_)
    {
        showMessage("Error", "Editor is invalid.", 1);
        return;
    }

    auto setting_ = jsonViewSettings_->getPluginSetting();
    const auto selectedText = scintillaEditor_->getJsonText();
    const auto [le, lf, indentChar, indentLen] = GetFormatSetting();

    auto result = JsonHandler(setting_.parseOptions).FormatJson(selectedText, le, lf, indentChar, indentLen);
    if (result.success)
    {
        scintillaEditor_->replaceSelection(result.response);
    }
    else
    {
        reportError(result);
    }
}
void NDDJsonPlugin::compressJson()
{
    if (!scintillaEditor_)
    {
        showMessage("Error", "Editor is invalid.", 1);
        return;
    }

    auto setting_ = jsonViewSettings_->getPluginSetting();
    const auto selectedText = scintillaEditor_->getJsonText();
    auto result = JsonHandler(setting_.parseOptions).GetCompressedJson(selectedText);
    if (result.success)
    {
        scintillaEditor_->replaceSelection(result.response);
    }
    else
    {
        reportError(result);
    }
}
void NDDJsonPlugin::refreshTableJson()
{
    if (!scintillaEditor_)
    {
        showMessage("Error", "Editor is invalid.", 1);
        return;
    }

    auto selectedText = scintillaEditor_->getJsonText();
    auto result = jsonModel_->loadJson(QByteArray::fromStdString(selectedText));
    if (!result)
    {
        showMessage("Error", "Json string is not parse.", 1);
    }
}
void NDDJsonPlugin::validateJson()
{
    if (!scintillaEditor_)
    {
        showMessage("Error", "Editor is invalid.", 1);
        return;
    }

    auto setting_ = jsonViewSettings_->getPluginSetting();
    const auto selectedText = scintillaEditor_->getJsonText();
    auto result = JsonHandler(setting_.parseOptions).ValidateJson(selectedText);
    if (result.success)
    {
        showMessage("Tips", "JSON looks good. No errors found while validating it.", 1);
    }
    else
    {
        reportError(result);
    }
}
void NDDJsonPlugin::findNode(const QString &str)
{
    bool isStartSelected = false;
    auto selected = treeView_->currentIndex();
    if (!selected.isValid())
    {
        isStartSelected = true;
        selected = jsonModel_->index(0, 0);
    }

    auto match = jsonModel_->match(selected, Qt::DisplayRole, QVariant::fromValue("*" + str + "*"), 2,
                                   Qt::MatchContains | Qt::MatchRecursive | Qt::MatchRegExp | Qt::MatchWrap);
    if (!match.isEmpty())
    {
        if (match.first() == selected)
        {
            match.removeFirst();
        }

        if (!match.isEmpty())
        {
            treeView_->setExpanded(match.first(), true);
            treeView_->setCurrentIndex(match.first());
        }
        else
        {
            showMessage("Warning", "match is invalid.", 1);
        }
    }
    else
    {
        if (!isStartSelected)
        {
            match = jsonModel_->match(jsonModel_->index(0, 0), Qt::DisplayRole, QVariant::fromValue("*" + str + "*"), 2,
                                      Qt::MatchContains | Qt::MatchRecursive | Qt::MatchRegExp | Qt::MatchWrap);
            if (!match.isEmpty())
            {
                treeView_->setExpanded(match.first(), true);
                treeView_->setCurrentIndex(match.first());
            }
            else
            {
                showMessage("Warning", QString("Cannot found with %1").arg(str).toStdString(), 1);
            }
        }
        else
        {
            showMessage("Warning", QString("Cannot found with %1").arg(str).toStdString(), 1);
        }
    }
}
void NDDJsonPlugin::setScintilla(const std::function<QsciScintilla *()> &cb)
{
    if (scintillaEditor_)
    {
        delete scintillaEditor_;
        scintillaEditor_ = nullptr;
    }

    scintillaEditor_ = new ScintillaEditor(cb);
}
