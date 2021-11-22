#include "HelpCmd.h"

#include <gio/gio.h>
#include <glib.h>

#include <iostream>

#include "../utils/LogUtils.h"
//#include <stdlib.h>
//#include <unistd.h>
using namespace std;

static int opt_verbose = 0;
static gboolean opt_ostree_verbose;
static gboolean opt_version;
static gboolean opt_default_arch;
static gboolean opt_supported_arches;
static gboolean opt_gl_drivers;
static gboolean opt_list_installations;
static gboolean opt_user;
static gboolean opt_system;
static char **opt_installations;
static gboolean opt_help;

#define PACKAGE_NAME "llcmd"
#define PACKAGE_STRING "llcmd V1.0.0"
#define FLATPAK_ANSI_BOLD_ON "\x1b[1m"
#define FLATPAK_ANSI_BOLD_OFF "\x1b[22m"
#define FLATPAK_ANSI_FAINT_ON "\x1b[2m"
#define FLATPAK_ANSI_FAINT_OFF "\x1b[22m"
#define FLATPAK_ANSI_RED "\x1b[31m"
#define FLATPAK_ANSI_GREEN "\x1b[32m"
#define FLATPAK_ANSI_COLOR_RESET "\x1b[0m"

static gboolean opt_verbose_cb = TRUE;
// static gboolean opt_verbose_cb(const gchar *option_name, const gchar *value, gpointer data, GError **error)
// {
//     opt_verbose++;
//     return TRUE;
// }

GOptionEntry global_entries[] = {
    {"verbose", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, &opt_verbose_cb,
     "Show debug information, -vv for more detail", NULL},
    {"ostree-verbose", 0, 0, G_OPTION_ARG_NONE, &opt_ostree_verbose, "Show OSTree debug information", NULL},
    {"help", '?', G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_NONE, &opt_help, NULL, NULL},
    {NULL}};

static GOptionEntry empty_entries[] = {
    {"version", 0, 0, G_OPTION_ARG_NONE, &opt_version, "Print version information and exit", NULL},
    {"default-arch", 0, 0, G_OPTION_ARG_NONE, &opt_default_arch, "Print default arch and exit", NULL},
    {"supported-arches", 0, 0, G_OPTION_ARG_NONE, &opt_supported_arches, "Print supported arches and exit", NULL},
    {"gl-drivers", 0, 0, G_OPTION_ARG_NONE, &opt_gl_drivers, "Print active gl drivers and exit", NULL},
    {"installations", 0, 0, G_OPTION_ARG_NONE, &opt_list_installations, "Print paths for system installations and exit",
     NULL},
    {NULL}};

GOptionEntry user_entries[] = {
    {"user", 0, 0, G_OPTION_ARG_NONE, &opt_user, "Work on the user installation", NULL},
    {"system", 0, 0, G_OPTION_ARG_NONE, &opt_system, "Work on the system-wide installation (default)", NULL},
    {"installation", 0, 0, G_OPTION_ARG_STRING_ARRAY, &opt_installations,
     "Work on a non-default system-wide installation", "NAME"},
    {NULL}};

string flatpak_get_arch() { return "x86_64"; }

static GOptionContext *flatpak_option_context_new_with_commands()
{
    GOptionContext *context;
    GString *summary;

    context = g_option_context_new("COMMAND");
    g_option_context_set_translation_domain(context, PACKAGE_NAME);

    summary = g_string_new("Builtin Commands:");
    // while (commands->name != NULL)
    // {
    //     if (!commands->deprecated)
    //     {
    //         if (commands->fn != NULL)
    //         {
    //             g_string_append_printf(summary, "\n  %s", commands->name);
    //             /* Note: the 23 is there to align command descriptions with
    //              * the option descriptions produced by GOptionContext.
    //              */
    //             if (commands->description)
    //                 g_string_append_printf(summary, "%*s%s", (int)(23 - strlen(commands->name)), "",
    //                                        _(commands->description));
    //         }
    //         else
    //         {
    //             g_string_append_printf(summary, "\n%s", _(commands->name));
    //         }
    //     }
    //     commands++;
    // }

    g_option_context_set_summary(context, summary->str);

    g_string_free(summary, TRUE);

    return context;
}

static int flatpak_run(int argc, char **argv, GError **res_error)
{
    GError *error = NULL;
    // GCancellable *cancellable = NULL;
    g_autofree char *prgname = NULL;
    gboolean success = FALSE;
    const char *command_name = NULL;
    // command = extract_command(&argc, argv, &command_name);
    {
        GOptionContext *context;
        g_autofree char *hint = NULL;
        g_autofree char *msg = NULL;

        context = flatpak_option_context_new_with_commands();

        hint = g_strdup_printf("See '%s --help'", g_get_prgname());
        // 查找相似的命令 如输入 flatpak lis
        // if (command_name != NULL)
        // {
        //     const char *similar;

        //     similar = find_similar_command(command_name);
        //     if (similar)
        //         msg = g_strdup_printf(_("'%s' is not a flatpak command. Did you mean '%s'?"), command_name, similar);
        //     else
        //         msg = g_strdup_printf(_("'%s' is not a flatpak command"), command_name);
        // }
        // else
        {
            g_autoptr(GError) local_error = NULL;

            g_option_context_add_main_entries(context, empty_entries, NULL);
            g_option_context_add_main_entries(context, global_entries, NULL);
            if (g_option_context_parse(context, &argc, &argv, &local_error))
            {
                if (opt_version)
                {
                    g_print("%s\n", PACKAGE_STRING);
                    exit(EXIT_SUCCESS);
                }

                if (opt_default_arch)
                {
                    g_print("%s\n", flatpak_get_arch().c_str());
                    exit(EXIT_SUCCESS);
                }

                // if (opt_supported_arches)
                // {
                //     const char **arches = flatpak_get_arches();
                //     int i;
                //     for (i = 0; arches[i] != NULL; i++) g_print("%s\n", arches[i]);
                //     exit(EXIT_SUCCESS);
                // }

                // if (opt_gl_drivers)
                // {
                //     const char **drivers = flatpak_get_gl_drivers();
                //     int i;
                //     for (i = 0; drivers[i] != NULL; i++) g_print("%s\n", drivers[i]);
                //     exit(EXIT_SUCCESS);
                // }

                // if (opt_list_installations)
                // {
                //     GPtrArray *paths;

                //     paths = flatpak_get_system_base_dir_locations(NULL, &local_error);
                //     if (paths)
                //     {
                //         guint i;
                //         for (i = 0; i < paths->len; i++)
                //         {
                //             GFile *file = paths->pdata[i];
                //             g_print("%s\n", flatpak_file_get_path_cached(file));
                //         }
                //         exit(EXIT_SUCCESS);
                //     }
                // }
            }

            if (local_error)
                msg = g_strdup(local_error->message);
            else
                msg = g_strdup("No command specified");
        }

        g_option_context_free(context);

        g_set_error(&error, G_IO_ERROR, G_IO_ERROR_FAILED, "%s\n\n%s", msg, hint);
    }
    g_propagate_error(res_error, error);
    return 1;
}

HelpCmd::HelpCmd() {}

HelpCmd::~HelpCmd() {}

int HelpCmd::extractCmd(int argc, char **argv)
{
    // cout << "HelpCmd extractCmd" << endl;
    LogUtils::log_info("HelpCmd extractCmd");
    return 0;
}

int HelpCmd::verifyCmd()
{
    // cout << "HelpCmd verifyCmd" << endl;
    LogUtils::log_info("HelpCmd verifyCmd");
    return 0;
}

int HelpCmd::execCmd(int argc, char **argv)
{
    // cout << "HelpCmd execCmd" << endl;
    LogUtils::log_info("HelpCmd execCmd");

    g_set_prgname(PACKAGE_NAME);
    GError *error = NULL;
    int ret = flatpak_run(argc, argv, &error);
    if (error != NULL)
    {
        const char *prefix = "";
        const char *suffix = "";
        // if (flatpak_fancy_output())
        {
            prefix = FLATPAK_ANSI_RED FLATPAK_ANSI_BOLD_ON;
            suffix = FLATPAK_ANSI_BOLD_OFF FLATPAK_ANSI_COLOR_RESET;
        }
        g_dbus_error_strip_remote_error(error);
        g_printerr("%s%s %s%s\n", prefix, "error:", suffix, error->message);
        g_error_free(error);
    }
    return 0;
}