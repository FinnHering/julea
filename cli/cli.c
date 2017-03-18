/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2010-2017 Michael Kuhn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <julea-config.h>

#include "cli.h"

#include <locale.h>
#include <stdlib.h>

guint
j_cmd_arguments_length (gchar const** arguments)
{
	guint i = 0;

	for (; *arguments != NULL; arguments++)
	{
		i++;
	}

	return i;
}

void
j_cmd_usage (void)
{
	g_print("Usage:\n");
	g_print("  %s COMMAND\n", g_get_prgname());
	g_print("\n");
	g_print("Commands:\n");
	g_print("  create     object://index/namespace/name\n");
	g_print("             julea://collection/[item]\n");
	g_print("  create-all julea://collection/[item]\n");
	g_print("  copy       julea://collection/item julea://collection/item\n");
	g_print("             julea://collection/item file://file\n");
	g_print("             file://file julea://collection/item\n");
	g_print("  delete     julea://collection/[item]\n");
	g_print("  list       julea://[collection]\n");
	g_print("  status     julea://collection/item\n");
	g_print("\n");
}

gboolean
j_cmd_error_last (JURI* uri)
{
	gboolean ret = FALSE;

	if (j_uri_get_collection(uri) == NULL && j_uri_get_collection_name(uri) != NULL)
	{
		ret = (j_uri_get_item_name(uri) == NULL);
	}
	else if (j_uri_get_item(uri) == NULL && j_uri_get_item_name(uri) != NULL)
	{
		ret = TRUE;
	}

	return ret;
}

int
main (int argc, char** argv)
{
	gboolean success;
	gchar* basename;
	gchar const* command = NULL;
	gchar const** arguments = NULL;
	gint i;

	setlocale(LC_ALL, "");

	basename = g_path_get_basename(argv[0]);
	g_set_prgname(basename);
	g_free(basename);

	if (argc <= 2)
	{
		j_cmd_usage();
		return 1;
	}

	command = argv[1];

	j_init();

	arguments = g_new(gchar const*, argc - 1);

	for (i = 2; i < argc; i++)
	{
		arguments[i - 2] = argv[i];
	}

	arguments[argc - 2] = NULL;

	if (g_strcmp0(command, "copy") == 0)
	{
		success = j_cmd_copy(arguments);
	}
	else if (g_strcmp0(command, "create") == 0)
	{
		success = j_cmd_create(arguments, FALSE);
	}
	else if (g_strcmp0(command, "create-all") == 0)
	{
		success = j_cmd_create(arguments, TRUE);
	}
	else if (g_strcmp0(command, "delete") == 0)
	{
		success = j_cmd_delete(arguments);
	}
	else if (g_strcmp0(command, "list") == 0)
	{
		success = j_cmd_list(arguments);
	}
	else if (g_strcmp0(command, "status") == 0)
	{
		success = j_cmd_status(arguments);
	}
	else
	{
		success = FALSE;
		j_cmd_usage();
	}

	g_free(arguments);

	j_fini();

	return (success) ? 0 : 1;
}