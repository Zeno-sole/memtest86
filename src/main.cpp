/*
 * memtest86 - Memory testing utility for deepin 25
 *
 * Main entry point. Defaults to the DTK GUI; --cli switches to the
 * command line interface. Translation follows the system language
 * (zh_CN / en, falling back to English).
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <QCoreApplication>
#include <QGuiApplication>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>
#include <QDebug>
#include <QIcon>

#include <DApplication>

#include "cli/cli.h"
#include "gui/mainwindow.h"

DWIDGET_USE_NAMESPACE

static QString translationDir()
{
    // 1. Source/build tree (development) — prefer a tree-local translations/
    //    dir when present so rebuilt .qm files are picked up before any
    //    installed copy (which may be older).
    QStringList candidates;
    candidates << QStringLiteral("translations");
    // 2. Installed locations
    candidates << QStringLiteral("/usr/share/memtest86/translations")
               << QStringLiteral("/usr/local/share/memtest86/translations");
    for (const QString &dir : candidates) {
        if (QDir(dir).exists())
            return dir;
    }
    return QString();
}

static bool loadTranslator(QTranslator *qt, QTranslator *app, const QLocale &locale)
{
    const QString lang = locale.name(); // e.g. zh_CN / en_US
    const QString dir = translationDir();
    if (dir.isEmpty())
        return false;

    // Qt base translations (for standard dialogs)
    if (qt->load(locale, QStringLiteral("qtbase"), QStringLiteral("_"),
                 QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        QCoreApplication::installTranslator(qt);

    if (app->load(locale, QStringLiteral("memtest86"), QStringLiteral("_"), dir)) {
        QCoreApplication::installTranslator(app);
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    // Must be set before any QGuiApplication-like instance is created.
    // Some DTK internals may create one during static init, so set it first.
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Detect CLI mode before creating any GUI objects.
    bool cliMode = false;
    for (int i = 1; i < argc; ++i) {
        const QString a = QString::fromLocal8Bit(argv[i]);
        if (a == QLatin1String("--cli")) {
            cliMode = true;
            break;
        }
    }

    int rc = 0;
    if (cliMode) {
        QCoreApplication app(argc, argv);
        QCoreApplication::setApplicationName(QStringLiteral("memtest86"));
        QCoreApplication::setApplicationVersion(QStringLiteral("1.1.0"));

        QTranslator qt, appTr;
        loadTranslator(&qt, &appTr, QLocale::system());

        memtest::CliOptions opts;
        const QStringList args = app.arguments().mid(1);
        if (memtest::parseCliOptions(args, &opts)) {
            if (opts.help) {
                memtest::printHelp();
            } else {
                rc = memtest::runCli(opts);
            }
        } else {
            QTextStream err(stderr);
            err << QObject::tr("Error: %1\n").arg(opts.err);
            memtest::printHelp();
            rc = 2;
        }
    } else {
        DApplication app(argc, argv);
        DApplication::setApplicationName(QStringLiteral("memtest86"));
        DApplication::setApplicationDisplayName(QObject::tr("Memory Test"));
        DApplication::setApplicationVersion(QStringLiteral("1.1.0"));
        DApplication::setOrganizationName(QStringLiteral("deepin"));
        DApplication::setOrganizationDomain(QStringLiteral("deepin.org"));
        app.setProductName(QObject::tr("Memory Test"));
        app.setProductIcon(QIcon::fromTheme(QStringLiteral("memtest86"),
                                             QIcon(QStringLiteral(":/icons/memtest86.svg"))));

        QTranslator qt, appTr;
        loadTranslator(&qt, &appTr, QLocale::system());

        memtest::MainWindow w;
        w.show();
        rc = app.exec();
    }

    return rc;
}
