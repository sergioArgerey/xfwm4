/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either opt_version 2 of the License, or
 * (at your option) any later opt_version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/X.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <stdlib.h>

static void
on_realize (GtkWidget *dialog,
            gpointer  data)
{
    Window xid;

    xid = (Window) GPOINTER_TO_INT (data);
    gdk_error_trap_push ();
    XSetTransientForHint (gdk_display, GDK_WINDOW_XID (dialog->window), xid);
    gdk_error_trap_pop ();
}

int
main (int argc, char **argv)
{
    GtkWidget *dialog;
    gint i;
    gulong xid;
    gchar *title, *newstr;

    xfce_textdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

    gtk_init (&argc, &argv);

    if (argc < 2)
    {
        g_print ("Wrong number of arguments\n");
        return 1;
    }

    xid = strtoul (argv [1], NULL, 16);
    if (!xid)
    {
        g_print ("Invalid XID (%s)\n", argv [1]);
        return 1;
    }

    title = g_strdup ("");
    for (i = 2; i < argc; i++)
    {
        newstr = g_strdup_printf ("%s %s", title, argv[i]);
        g_free (title);
        title = newstr;
    }

    dialog = gtk_message_dialog_new (NULL, 0,
                                     GTK_MESSAGE_WARNING,
                                     GTK_BUTTONS_YES_NO,
                                     _("This window might be busy and is not responding.\n"
                                       "Do you want to terminate the application?"),
                                     NULL);

    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_YES);
    gtk_widget_set (GTK_WIDGET (dialog), "secondary-text", title, NULL);
    gtk_window_set_title (GTK_WINDOW (dialog), title);
    g_signal_connect (G_OBJECT (dialog), "realize",
                      G_CALLBACK (on_realize), (gpointer) GINT_TO_POINTER (xid));
    gtk_widget_realize (dialog);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_YES)
    {
        g_print ("YES");
    }
    else
    {
        g_print ("NO");
    }
    return 0;
}
