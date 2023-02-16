#ifndef DEFINE_H
#define DEFINE_H

#define INI_FORMATTING_SEC_NAME "Formatting"
#define INI_FORMATTING_EOL_NAME "Formatting/EOL"
#define INI_FORMATTING_LINE_NAME "Formatting/LINE_FORMATTING"
#define INI_FORMATTING_INDENT_NAME "Formatting/INDENTATION"
#define INI_FORMATTING_INDENTCOUNT_NAME "Formatting/INDENTATION_COUNT"

#define INI_OTHER_SEC_NAME "Others"
#define INI_OTHER_FOLLOW_TAB_NAME "Others/FOLLOW_TAB"
#define INI_OTHER_AUTO_FORMAT_NAME "Others/AUTO_FORMAT"
#define INI_OTHER_IGNORE_COMMENT_NAME "Others/IGNORE_COMMENT"
#define INI_OTHER_IGNORE_COMMA_NAME "Others/IGNORE_TRAILING_COMMA"

#define INI_OTHER_USE_TABLE_VIEW_NAME "Others/USE_TABLE_VIEW"

enum class LineEnding
{
    AUTO,
    WINDOWS,
    UNIX,
    MAC
};

enum class LineFormat
{
    DEFAULT,
    SINGLELINE
};

enum class IndentStyle
{
    AUTO,
    TAB,
    SPACE
};

struct Indent
{
    unsigned len = 4;
    IndentStyle style = IndentStyle::AUTO;
};

struct ParseOptions
{
    bool bIgnoreComment = true;
    bool bIgnoreTrailingComma = true;
};

struct Setting
{
    LineEnding lineEnding = LineEnding::AUTO;
    LineFormat lineFormat = LineFormat::DEFAULT;
    Indent indent{};
    bool bFollowCurrentTab = false;
    bool bAutoFormat = false;
    ParseOptions parseOptions{};
    bool useTableView = true;
};

#endif  // DEFINE_H
