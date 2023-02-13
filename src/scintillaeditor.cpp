//
// Created by Zh1an on 2023/2/9.
//

#include "scintillaeditor.h"

#include <Qsci/qsciscintilla.h>

ScintillaEditor::ScintillaEditor(const std::function<QsciScintilla *()> &cb) : scintillaCallback_(cb)
{
}

std::string ScintillaEditor::getJsonText()
{
    auto scintilla_ = scintillaCallback_();

    if (!scintilla_)
    {
        return {};
    }
    refreshSelectionPos();
    auto text = scintilla_->selectedText();
    if (text.isEmpty())
    {
        scintilla_->selectAll(true);
        return scintilla_->text().toStdString();
    }
    refreshSelectionPos();
    return text.toStdString();
}

bool ScintillaEditor::isJsonFile() const
{
    return true;
}

void ScintillaEditor::replaceSelection(const std::string &text)
{
    auto scintilla_ = scintillaCallback_();
    if (!scintilla_)
    {
        return;
    }

    scintilla_->replaceSelectedText(text.c_str());

    scintilla_->setCursorPosition(0, 0);

    refreshSelectionPos();
}

void ScintillaEditor::makeSelection(size_t start, size_t end)
{
    auto scintilla_ = scintillaCallback_();
    if (!scintilla_)
    {
        return;
    }

    scintilla_->SendScintilla(QsciScintillaBase::SCI_SETSEL, start, end);
}

auto ScintillaEditor::getSelectionStart() const -> size_t
{
    return startPos_;
}

auto ScintillaEditor::getSelectionEnd() const -> size_t
{
    return endPos_;
}

auto ScintillaEditor::getEOL() const -> unsigned
{
    auto scintilla_ = scintillaCallback_();
    auto eolMode = scintilla_->SendScintilla(QsciScintillaBase::SCI_GETEOLMODE);
    return eolMode;
}

auto ScintillaEditor::getIndent() const -> std::tuple<char, unsigned>
{
    auto scintilla_ = scintillaCallback_();
    bool useTabs = scintilla_->SendScintilla(QsciScintillaBase::SCI_GETUSETABS);
    char indentChar = useTabs ? '\t' : ' ';
    unsigned indentLen =
        useTabs ? 1 : static_cast<unsigned>(scintilla_->SendScintilla(QsciScintillaBase::SCI_GETTABWIDTH));

    return {indentChar, indentLen};
}

void ScintillaEditor::refreshSelectionPos()
{
    auto scintilla_ = scintillaCallback_();
    startPos_ = scintilla_->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);
    endPos_ = scintilla_->SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);

    if (endPos_ < startPos_)
    {
        std::swap(startPos_, endPos_);
    }
}
void ScintillaEditor::replaceAll(const std::string &text) const
{
    auto scintilla_ = scintillaCallback_();
    scintilla_->selectAll();
    scintilla_->replaceSelectedText(text.c_str());
    scintilla_->setCursorPosition(0, 0);
}
