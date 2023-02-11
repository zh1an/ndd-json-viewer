//
// Created by Zh1an on 2023/2/11.
//

#include <pluginGl.h>

#include <Qsci/qsciscintilla.h>
#include <QWidget>

#include <nddjsonplugin.h>

#include <functional>

#define NDD_EXPORTDLL

#if defined(Q_OS_WIN)
#if defined(NDD_EXPORTDLL)
#define NDD_EXPORT __declspec(dllexport)
#else
#define NDD_EXPORT __declspec(dllimport)
#endif
#else
#define NDD_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    NDD_EXPORT bool NDD_PROC_IDENTIFY(NDD_PROC_DATA *pProcData);
    NDD_EXPORT int NDD_PROC_MAIN(QWidget *pNotepad, const QString &strFileName,
                                 std::function<QsciScintilla *()> getCurEdit);

#ifdef __cplusplus
}
#endif

bool NDD_PROC_IDENTIFY(NDD_PROC_DATA *pProcData)
{
    if (pProcData == nullptr)
    {
        return false;
    }
    pProcData->m_strPlugName = QObject::tr("JsonView Plug");
    pProcData->m_strComment = QObject::tr("json format");

    pProcData->m_version = QString("0.0.0.1");
    pProcData->m_auther = QString("Zh1an");
    return true;
}

NDDJsonPlugin *nddJsonPlugin = nullptr;

// 插件的入口点函数
int NDD_PROC_MAIN(QWidget *pNotepad, const QString &strFileName, std::function<QsciScintilla *()> getCurEdit)
{
    QsciScintilla *pEdit = getCurEdit();

    if (!nddJsonPlugin)
    {
        nddJsonPlugin = new NDDJsonPlugin(pNotepad, strFileName, pEdit, pNotepad);
    }

    return 0;
}
