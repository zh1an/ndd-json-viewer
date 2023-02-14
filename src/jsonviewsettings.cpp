//
// Created by Zh1an on 2023/2/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_JsonViewSettings.h" resolved

#include "jsonviewsettings.h"
#include "ui_JsonViewSettings.h"

#include <QDir>
#include <QSettings>

JsonViewSettings::JsonViewSettings(const QString &pluginPath, QWidget *parent)
    : QWidget(parent), ui(new Ui::JsonViewSettings), configSetting_(nullptr)
{
    ui->setupUi(this);

    auto pluginDllName = pluginPath;

    init(pluginDllName.replace("ndd-json-viewer.dll", ""));

    initConnection();
}

JsonViewSettings::~JsonViewSettings()
{
    delete ui;
}

void JsonViewSettings::slot_ConfirmClicked()
{
    //! other settings for ini file
    configSetting_->setValue(INI_OTHER_FOLLOW_TAB_NAME, (int)ui->cb_follow_current_tab->isChecked());
    configSetting_->setValue(INI_OTHER_AUTO_FORMAT_NAME, (int)ui->cb_auto_format_when_opened->isChecked());
    configSetting_->setValue(INI_OTHER_IGNORE_COMMA_NAME, (int)ui->cb_ignore_trailing_comma->isChecked());
    configSetting_->setValue(INI_OTHER_IGNORE_COMMENT_NAME, (int)ui->cb_ignore_comments->isChecked());

    //! formatting settings for ini file
    configSetting_->setValue(INI_FORMATTING_INDENT_NAME, getIndentation());
    configSetting_->setValue(INI_FORMATTING_EOL_NAME, getLineEnding());
    configSetting_->setValue(INI_FORMATTING_LINE_NAME, getLineFormatting());

    //! other settings for PlugSettings
    pluginSetting_.bFollowCurrentTab = ui->cb_follow_current_tab->isChecked();
    pluginSetting_.bAutoFormat = ui->cb_auto_format_when_opened->isChecked();
    pluginSetting_.parseOptions.bIgnoreTrailingComma = ui->cb_ignore_trailing_comma->isChecked();
    pluginSetting_.parseOptions.bIgnoreComment = ui->cb_ignore_comments->isChecked();

    //! formatting settings for PlugSettings
    pluginSetting_.indent.style = static_cast<IndentStyle>(getIndentation());
    pluginSetting_.lineEnding = static_cast<LineEnding>(getLineEnding());
    pluginSetting_.lineFormat = static_cast<LineFormat>(getLineFormatting());

    this->hide();
}

void JsonViewSettings::init(const QString &pluginPath)
{
    QDir pluginDir(pluginPath);
    if (!pluginDir.cd("config"))
    {
        pluginDir.mkdir("config");
        pluginDir.cd("config");
    }

    configSetting_ = new QSettings(pluginDir.absolutePath() + "/JSONViewer.ini", QSettings::IniFormat);
    if (!configSetting_->contains(INI_FORMATTING_EOL_NAME))
    {
        configSetting_->setValue(INI_FORMATTING_EOL_NAME, (int)LE::kCrLf);
    }
    setLineEnding(configSetting_->value(INI_FORMATTING_EOL_NAME).toInt());

    if (!configSetting_->contains(INI_FORMATTING_LINE_NAME))
    {
        configSetting_->setValue(INI_FORMATTING_LINE_NAME, (int)LF::kFormatDefault);
    }
    setLineFormatting(configSetting_->value(INI_FORMATTING_LINE_NAME).toInt());

    if (!configSetting_->contains(INI_FORMATTING_INDENT_NAME))
    {
        configSetting_->setValue(INI_FORMATTING_INDENT_NAME, (int)IndentStyle::AUTO);
    }
    setIndentation(configSetting_->value(INI_FORMATTING_INDENT_NAME).toInt());

    if (!configSetting_->contains(INI_FORMATTING_INDENTCOUNT_NAME))
    {
        configSetting_->setValue(INI_FORMATTING_INDENTCOUNT_NAME, 4);
    }
    pluginSetting_.indent.len = configSetting_->value(INI_FORMATTING_INDENTCOUNT_NAME).toInt();

    if (!configSetting_->contains(INI_OTHER_FOLLOW_TAB_NAME))
    {
        configSetting_->setValue(INI_OTHER_FOLLOW_TAB_NAME, 1);
    }
    pluginSetting_.bFollowCurrentTab = configSetting_->value(INI_OTHER_FOLLOW_TAB_NAME).toBool();
    ui->cb_follow_current_tab->setChecked((int)pluginSetting_.bFollowCurrentTab);

    if (!configSetting_->contains(INI_OTHER_AUTO_FORMAT_NAME))
    {
        configSetting_->setValue(INI_OTHER_AUTO_FORMAT_NAME, 0);
    }
    pluginSetting_.bAutoFormat = configSetting_->value(INI_OTHER_AUTO_FORMAT_NAME).toBool();
    ui->cb_auto_format_when_opened->setChecked(pluginSetting_.bAutoFormat);

    if (!configSetting_->contains(INI_OTHER_IGNORE_COMMENT_NAME))
    {
        configSetting_->setValue(INI_OTHER_IGNORE_COMMENT_NAME, 1);
    }
    pluginSetting_.parseOptions.bIgnoreComment = configSetting_->value(INI_OTHER_IGNORE_COMMENT_NAME).toBool();
    ui->cb_ignore_comments->setChecked(pluginSetting_.parseOptions.bIgnoreComment);

    if (!configSetting_->contains(INI_OTHER_IGNORE_COMMA_NAME))
    {
        configSetting_->setValue(INI_OTHER_IGNORE_COMMA_NAME, 1);
    }
    pluginSetting_.parseOptions.bIgnoreTrailingComma = configSetting_->value(INI_OTHER_IGNORE_COMMA_NAME).toBool();
    ui->cb_ignore_trailing_comma->setChecked(pluginSetting_.parseOptions.bIgnoreTrailingComma);
}

void JsonViewSettings::setIndentation(int var)
{
    // Indentation options
    switch (static_cast<IndentStyle>(var))
    {
        case IndentStyle::TAB: {
            ui->rb_indentation_auto_detect->setChecked(false);
            ui->rb_use_space->setChecked(false);
            ui->rb_use_tab->setChecked(true);

            pluginSetting_.indent.style = IndentStyle::TAB;

            break;
        }
        case IndentStyle::SPACE: {
            ui->rb_indentation_auto_detect->setChecked(false);
            ui->rb_use_space->setChecked(true);
            ui->rb_use_tab->setChecked(false);

            pluginSetting_.indent.style = IndentStyle::SPACE;

            break;
        }

        case IndentStyle::AUTO:
        default: {
            ui->rb_indentation_auto_detect->setChecked(true);
            ui->rb_use_space->setChecked(false);
            ui->rb_use_tab->setChecked(false);

            pluginSetting_.indent.style = IndentStyle::AUTO;

            break;
        }
    }
}
void JsonViewSettings::setLineEnding(int var)
{
    // End of line options
    switch (static_cast<LineEnding>(var))
    {
        case LineEnding::WINDOWS: {
            ui->rb_line_ending_auto_detect->setChecked(false);
            ui->rb_line_ending_unix->setChecked(false);
            ui->rb_line_ending_window->setChecked(true);
            ui->rb_line_ending_macintosh->setChecked(false);

            pluginSetting_.lineEnding = LineEnding::WINDOWS;

            break;
        }
        case LineEnding::UNIX: {
            ui->rb_line_ending_auto_detect->setChecked(false);
            ui->rb_line_ending_unix->setChecked(true);
            ui->rb_line_ending_window->setChecked(false);
            ui->rb_line_ending_macintosh->setChecked(false);

            pluginSetting_.lineEnding = LineEnding::UNIX;

            break;
        }

        case LineEnding::MAC: {
            ui->rb_line_ending_auto_detect->setChecked(false);
            ui->rb_line_ending_unix->setChecked(false);
            ui->rb_line_ending_window->setChecked(false);
            ui->rb_line_ending_macintosh->setChecked(true);

            pluginSetting_.lineEnding = LineEnding::MAC;

            break;
        }

        case LineEnding::AUTO:
        default: {
            ui->rb_line_ending_auto_detect->setChecked(true);
            ui->rb_line_ending_unix->setChecked(false);
            ui->rb_line_ending_window->setChecked(false);
            ui->rb_line_ending_macintosh->setChecked(false);

            pluginSetting_.lineEnding = LineEnding::AUTO;

            break;
        }
    }
}
void JsonViewSettings::setLineFormatting(int var)
{
    if (var == (int)LF::kFormatDefault)
    {
        ui->rb_line_formatting_default_formatting->setChecked(true);
        ui->rb_line_formatting_single_line->setChecked(false);

        pluginSetting_.lineFormat = LineFormat::DEFAULT;
    }
    else if (var == (int)LF::kFormatSingleLineArray)
    {
        ui->rb_line_formatting_default_formatting->setChecked(false);
        ui->rb_line_formatting_single_line->setChecked(true);

        pluginSetting_.lineFormat = LineFormat::SINGLELINE;
    }

    ui->rb_line_formatting_default_formatting->setChecked(true);
    ui->rb_line_formatting_single_line->setChecked(false);

    pluginSetting_.lineFormat = LineFormat::DEFAULT;
}
void JsonViewSettings::initConnection()
{
    connect(ui->pb_confirm, &QPushButton::clicked, this, &JsonViewSettings::slot_ConfirmClicked);

    //! indentation
    connect(ui->rb_indentation_auto_detect, &QRadioButton::clicked, this, [this] {
        ui->rb_use_tab->setChecked(false);
        ui->rb_use_space->setChecked(false);
    });
    connect(ui->rb_use_tab, &QRadioButton::clicked, this, [this] {
        ui->rb_indentation_auto_detect->setChecked(false);
        ui->rb_use_space->setChecked(false);
    });
    connect(ui->rb_use_space, &QRadioButton::clicked, this, [this] {
        ui->rb_use_tab->setChecked(false);
        ui->rb_indentation_auto_detect->setChecked(false);
    });

    //! Line Ending
    connect(ui->rb_line_ending_macintosh, &QRadioButton::clicked, this, [this] {
        ui->rb_line_ending_window->setChecked(false);
        ui->rb_line_ending_unix->setChecked(false);
        ui->rb_line_ending_auto_detect->setChecked(false);
    });
    connect(ui->rb_line_ending_window, &QRadioButton::clicked, this, [this] {
        ui->rb_line_ending_macintosh->setChecked(false);
        ui->rb_line_ending_unix->setChecked(false);
        ui->rb_line_ending_auto_detect->setChecked(false);
    });
    connect(ui->rb_line_ending_unix, &QRadioButton::clicked, this, [this] {
        ui->rb_line_ending_window->setChecked(false);
        ui->rb_line_ending_macintosh->setChecked(false);
        ui->rb_line_ending_auto_detect->setChecked(false);
    });
    connect(ui->rb_line_ending_auto_detect, &QRadioButton::clicked, this, [this] {
        ui->rb_line_ending_window->setChecked(false);
        ui->rb_line_ending_unix->setChecked(false);
        ui->rb_line_ending_macintosh->setChecked(false);
    });

    //! Line formatting
    connect(ui->rb_line_formatting_default_formatting, &QRadioButton::clicked, this,
            [this] { ui->rb_line_formatting_single_line->setChecked(false); });
    connect(ui->rb_line_formatting_single_line, &QRadioButton::clicked, this,
            [this] { ui->rb_line_formatting_default_formatting->setChecked(false); });
}
int JsonViewSettings::getIndentation() const
{
    if (ui->rb_use_tab->isChecked())
    {
        return (int)IndentStyle::TAB;
    }
    else if (ui->rb_use_space->isChecked())
    {
        return (int)IndentStyle::SPACE;
    }
    return (int)IndentStyle::AUTO;
}
int JsonViewSettings::getLineEnding() const
{
    if (ui->rb_line_ending_unix->isChecked())
    {
        return (int)LineEnding::UNIX;
    }
    else if (ui->rb_line_ending_window->isChecked())
    {
        return (int)LineEnding::WINDOWS;
    }
    else if (ui->rb_line_ending_macintosh->isChecked())
    {
        return (int)LineEnding::MAC;
    }

    return (int)LineEnding::AUTO;
}
int JsonViewSettings::getLineFormatting() const
{
    if (ui->rb_line_formatting_single_line->isChecked())
    {
        return (int)LineFormat::SINGLELINE;
    }

    return (int)LineFormat::DEFAULT;
}
