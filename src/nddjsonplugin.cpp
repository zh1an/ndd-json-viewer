//
// Created by Zh1an on 2023/2/11.
//

#include "nddjsonplugin.h"

#include <jsonviewsettings.h>
#include <scintillaeditor.h>

#include <QAction>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>

NDDJsonPlugin::NDDJsonPlugin(QWidget *mainWidget, const QString &pluginPath, QsciScintilla *pEdit, QObject *parent)
    : QObject(parent),
      scintillaEditor_(nullptr),
      jsonViewSettings_(new JsonViewSettings(pluginPath)),
      mainWidget_(mainWidget)
{
    // addJsonViewMenu();
    // jsonViewSettings_->show();
}
void NDDJsonPlugin::getJsonViewMenu(QMenu *menu)
{
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
void NDDJsonPlugin::setScintilla(const std::function<QsciScintilla *()> & cb)
{
    if (scintillaEditor_)
    {
        delete scintillaEditor_;
        scintillaEditor_ = nullptr;
    }

    scintillaEditor_ = new ScintillaEditor(cb);
}
