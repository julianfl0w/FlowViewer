/*
** Copyright (C) 2001-2022 by Carnegie Mellon University.
**
** @OPENSOURCE_LICENSE_START@
** See license information in ../../LICENSE.txt
** @OPENSOURCE_LICENSE_END@
*/

#include <silk/silk.h>

RCSIDENT("$SiLK: rwslammer.c ba4a308de770 2022-03-02 22:29:28Z mthomas $");

#include <silk/skplugin.h>
#include <silk/rwrec.h>


/* DEFINES AND TYPEDEFS */

/* Plugin protocol version */
#define PLUGIN_API_VERSION_MAJOR 1
#define PLUGIN_API_VERSION_MINOR 0


/* FUNCTION DECLARATIONS */

static skplugin_err_t check(const rwRec *rwrec, void *cbdata, void **extra);


/* FUNCTION DEFINITIONS */

/* the registration function called by skplugin.c */
skplugin_err_t
SKPLUGIN_SETUP_FN(
    uint16_t            major_version,
    uint16_t            minor_version,
    void        UNUSED(*pi_data))
{
    skplugin_err_t rv;
    skplugin_callbacks_t regdata;

    /* Check API version */
    rv = skpinSimpleCheckVersion(major_version, minor_version,
                                 PLUGIN_API_VERSION_MAJOR,
                                 PLUGIN_API_VERSION_MINOR,
                                 skAppPrintErr);
    if (rv != SKPLUGIN_OK) {
        return rv;
    }

    /* Register the function to use for filtering */
    memset(&regdata, 0, sizeof(regdata));
    regdata.filter = check;
    return skpinRegFilter(NULL, &regdata, NULL);
}


/*
 *  status = check(rwrec, data, NULL);
 *
 *    Check whether 'rwrec' passes the filter.  Return
 *    SKPLUGIN_FILTER_PASS if it does; SKPLUGIN_FILTER_FAIL otherwise.
 *
 *    Pass if dPort is 1434/udp
 */
static skplugin_err_t
check(
    const rwRec            *rwrec,
    void            UNUSED(*cbdata),
    void           UNUSED(**extra))
{
    if ((rwRecGetProto(rwrec) == 17)
        && (rwRecGetDPort(rwrec) == 1434))
    {
        return SKPLUGIN_FILTER_PASS;
    }
    return SKPLUGIN_FILTER_FAIL;
}


/*
** Local Variables:
** mode:c
** indent-tabs-mode:nil
** c-basic-offset:4
** End:
*/
