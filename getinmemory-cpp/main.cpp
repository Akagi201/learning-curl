/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Shows how the write callback function can be used to download data into a
 * chunk of memory instead of storing it in a file.
 * </DESC>
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <curl/curl.h>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t new_len = size * nmemb;
    size_t old_len = s->size();

    try {
        s->resize(old_len + new_len);
    } catch (std::bad_alloc &e) {
        // handle memory problem
        return 0;
    }

    std::copy((char *) contents, (char *) contents + new_len, s->begin() + old_len);

    return new_len;
}

int main(void) {
    CURL *curl_handle = NULL;
    CURLcode res;
    std::string s;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init the curl session */
    curl_handle = curl_easy_init();
    if (curl_handle != NULL) {
        /* specify URL to get */
        curl_easy_setopt(curl_handle, CURLOPT_URL, "http://httpbin.org/get");

        /* send all data to this function  */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &s);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get it! */
        res = curl_easy_perform(curl_handle);

        /* check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            /*
             * Now, our chunk.memory points to a memory block that is chunk.size
             * bytes big and contains the remote file.
             *
             * Do something nice with it!
             */
            std::cout << s << std::endl;
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl_handle);
    }

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    std::cout << "Program finished!" << std::endl;

    return 0;
}
