/*
 * IRCGW - Internet Relay Chat Gateway, src/gw_ssl.c
 * Copyright (C) 2009 Evilnet Development
 *
 * This file is part of IRCGW
 *
 * IRCGW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IRCGW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IRCGW.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */
#include "gw_ssl.h"

void gw_ssl_init() {
	SSL_METHOD *meth;

	sslenabled = 1;

	SSL_library_init();
	SSL_load_error_strings();

	meth = SSLv23_method();
	gw_sslctx = SSL_CTX_new(meth);

	if(!(SSL_CTX_use_certificate_chain_file(gw_sslctx, config_get(CONF_SSLCERT)))) {
		alog(LOG_ERROR, "Unable to load SSL certificate file, disabling SSL");
		sslenabled = 0;
	}

	if(sslenabled && !(SSL_CTX_use_PrivateKey_file(gw_sslctx, config_get(CONF_SSLKEY), SSL_FILETYPE_PEM))) {
		alog(LOG_ERROR, "Unable to load SSL key file, disabling SSL");
		sslenabled = 0;
	}
}

void gw_ssl_deinit() {
	if (sslenabled)
		SSL_CTX_free(gw_sslctx);
}

SSL* gw_ssl_connect(int fd) {
	SSL *ssl;
	
	if (sslenabled) {
		ssl=SSL_new(gw_sslctx);
		SSL_set_fd(ssl, fd);
		if (SSL_connect(ssl)<=0) {
			alog(LOG_ERROR, "SSL Connect error");
			return NULL;
		}
		alog(LOG_DEBUG, "Ssl: new()");
		return ssl;
	} else {
		return NULL;
	}
}

SSL* gw_ssl_accept(int fd) {
	SSL *ssl;

	if (sslenabled) {
		ssl=SSL_new(gw_sslctx);
		SSL_set_fd(ssl, fd);
		if (SSL_accept(ssl)<=0) {
			alog(LOG_ERROR, "SSL Accept error");
			return NULL;
		}
		alog(LOG_DEBUG, "Ssl: new()");
		return ssl;
	} else {
		return NULL;
	}
}

