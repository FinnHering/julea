/*
 * Copyright (c) 2010 Michael Kuhn
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <glib.h>

#include <mongo.h>

#include "connection.h"

struct JConnection
{
	mongo_connection connection;

	guint ref_count;
};

JConnection*
j_connection_new (void)
{
	JConnection* connection;

	connection = g_new(JConnection, 1);
	connection->ref_count = 1;

	return connection;
}

JConnection*
j_connection_ref (JConnection* connection)
{
	connection->ref_count++;

	return connection;
}

void
j_connection_unref (JConnection* connection)
{
	connection->ref_count--;

	if (connection->ref_count == 0)
	{
		mongo_destroy(&(connection->connection));

		g_free(connection);
	}
}

gboolean
j_connection_connect (JConnection* connection, const gchar* server)
{
	mongo_connection_options opts;
	mongo_conn_return status;

	g_strlcpy(opts.host, server, 255);
	opts.port = 27017;

	status = mongo_connect(&(connection->connection), &opts);

	return (status == mongo_conn_success);
}

JStore*
j_connection_get (JConnection* connection, const gchar* name)
{
	return j_store_new(connection, name);
}

/*
	ScopedDbConnection* _Connection::GetMongoDB ()
	{
		return new ScopedDbConnection(m_servers_string);
	}
*/
