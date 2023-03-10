/*
** Copyright (C) 2006-2022 by Carnegie Mellon University.
**
** @OPENSOURCE_LICENSE_START@
** See license information in ../../LICENSE.txt
** @OPENSOURCE_LICENSE_END@
*/

/*
**  Test a site configuration file.
**
*/


#include <silk/silk.h>

RCSIDENT("$SiLK: sksiteconfig-test.c ba4a308de770 2022-03-02 22:29:28Z mthomas $");

#include <silk/sksite.h>
#include <silk/utils.h>
#include "sksiteconfig.h"


int main(int argc, char **argv)
{
    SILK_FEATURES_DEFINE_STRUCT(features);
    int rv = 0;

    skAppRegister(argv[0]);
    skAppVerifyFeatures(&features, NULL);
    sksiteconfig_testing = 1;

    if ( argc == 2 ) {
        sksiteSetConfigPath(argv[1]);
        rv = sksiteConfigure(1);
    } else {
        fprintf(stderr, "usage: %s <filename>\n", skAppName());
        rv = -1;
    }

    skAppUnregister();
    return rv;
}


/*
** Local Variables:
** mode:c
** indent-tabs-mode:nil
** c-basic-offset:4
** End:
*/
