/* ====================================================================
 * Copyright (c) 2003 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string.h>
#include <openssl/err.h>
#ifdef OPENSSL_FIPS
# include <openssl/fips.h>
#endif
#include <openssl/evp.h>
#include <openssl/sha.h>

#ifdef OPENSSL_FIPS
static const char test[][60] = {
    "",
    "abc",
    "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
};

static const unsigned char ret[][SHA_DIGEST_LENGTH] = {
    {0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55,
     0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09},
    {0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e,
     0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d},
    {0x84, 0x98, 0x3e, 0x44, 0x1c, 0x3b, 0xd2, 0x6e, 0xba, 0xae,
     0x4a, 0xa1, 0xf9, 0x51, 0x29, 0xe5, 0xe5, 0x46, 0x70, 0xf1},
};

static int corrupt_sha;

void FIPS_corrupt_sha1()
{
    corrupt_sha = 1;
}

int FIPS_selftest_sha1()
{
    int n;

    for (n = 0; n < sizeof(test) / sizeof(test[0]); ++n) {
        unsigned char md[SHA_DIGEST_LENGTH];

        EVP_Digest(test[n], strlen(test[n]) + corrupt_sha, md, NULL,
                   EVP_sha1(), NULL);
        if (memcmp(md, ret[n], sizeof md)) {
            FIPSerr(FIPS_F_FIPS_SELFTEST_SHA1, FIPS_R_SELFTEST_FAILED);
            return 0;
        }
    }
    return 1;
}

static const unsigned char msg_sha256[] =
    { 0xfa, 0x48, 0x59, 0x2a, 0xe1, 0xae, 0x1f, 0x30,
    0xfc
};

static const unsigned char dig_sha256[] =
    { 0xf7, 0x26, 0xd8, 0x98, 0x47, 0x91, 0x68, 0x5b,
    0x9e, 0x39, 0xb2, 0x58, 0xbb, 0x75, 0xbf, 0x01,
    0x17, 0x0c, 0x84, 0x00, 0x01, 0x7a, 0x94, 0x83,
    0xf3, 0x0b, 0x15, 0x84, 0x4b, 0x69, 0x88, 0x8a
};

static const unsigned char msg_sha512[] =
    { 0x37, 0xd1, 0x35, 0x9d, 0x18, 0x41, 0xe9, 0xb7,
    0x6d, 0x9a, 0x13, 0xda, 0x5f, 0xf3, 0xbd
};

static const unsigned char dig_sha512[] =
    { 0x11, 0x13, 0xc4, 0x19, 0xed, 0x2b, 0x1d, 0x16,
    0x11, 0xeb, 0x9b, 0xbe, 0xf0, 0x7f, 0xcf, 0x44,
    0x8b, 0xd7, 0x57, 0xbd, 0x8d, 0xa9, 0x25, 0xb0,
    0x47, 0x25, 0xd6, 0x6c, 0x9a, 0x54, 0x7f, 0x8f,
    0x0b, 0x53, 0x1a, 0x10, 0x68, 0x32, 0x03, 0x38,
    0x82, 0xc4, 0x87, 0xc4, 0xea, 0x0e, 0xd1, 0x04,
    0xa9, 0x98, 0xc1, 0x05, 0xa3, 0xf3, 0xf8, 0xb1,
    0xaf, 0xbc, 0xd9, 0x78, 0x7e, 0xee, 0x3d, 0x43
};

int FIPS_selftest_sha2(void)
{
    unsigned char md[SHA512_DIGEST_LENGTH];

    EVP_Digest(msg_sha256, sizeof(msg_sha256), md, NULL, EVP_sha256(), NULL);
    if (memcmp(dig_sha256, md, sizeof(dig_sha256))) {
        FIPSerr(FIPS_F_FIPS_MODE_SET, FIPS_R_SELFTEST_FAILED);
        return 0;
    }

    EVP_Digest(msg_sha512, sizeof(msg_sha512), md, NULL, EVP_sha512(), NULL);
    if (memcmp(dig_sha512, md, sizeof(dig_sha512))) {
        FIPSerr(FIPS_F_FIPS_MODE_SET, FIPS_R_SELFTEST_FAILED);
        return 0;
    }

    return 1;
}

#endif
