#ifndef HEADER_CURL_AXTLS_H
#define HEADER_CURL_AXTLS_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 2010, DirecTV, Contact: Eric Hu <ehu@directv.com>
 * Copyright (C) 2010 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#ifdef USE_AXTLS
#include "curl/curl.h"
#include "curl/urldata.h"
namespace youmecommon
{
	int Curl_axtls_init(void);
	int Curl_axtls_cleanup(void);
	CURLcode Curl_axtls_connect(struct connectdata *conn, int sockindex);
	CURLcode Curl_axtls_connect_nonblocking(
	struct connectdata *conn,
		int sockindex,
		bool *done);

 /* close a SSL connection */
void Curl_axtls_close(struct connectdata *conn, int sockindex);

void Curl_axtls_session_free(void *ptr);
size_t Curl_axtls_version(char *buffer, size_t size);
int Curl_axtls_shutdown(struct connectdata *conn, int sockindex);
int Curl_axtls_check_cxn(struct connectdata *conn);
int Curl_axtls_random(struct SessionHandle *data,
                      unsigned char *entropy,
                      size_t length);

/* Set the API backend definition to axTLS */
#define CURL_SSL_BACKEND CURLSSLBACKEND_AXTLS

/* API setup for axTLS */

#define youme_curlssl_close_all(x) ((void)x)

#define youme_curlssl_set_engine(x,y) ((void)x, (void)y, CURLE_NOT_BUILT_IN)
#define youme_curlssl_set_engine_default(x) ((void)x, CURLE_NOT_BUILT_IN)
#define youme_curlssl_engines_list(x) ((void)x, (struct curl_slist *)NULL)


#define youme_curlssl_data_pending(x,y) ((void)x, (void)y, 0)

}
#endif /* USE_AXTLS */
#endif /* HEADER_CURL_AXTLS_H */

