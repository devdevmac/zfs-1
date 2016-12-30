/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2016, Brendon Humphrey (brendon.humphrey@mac.com). All rights reserved.
 */

#include <string.h>
#include <libnvpair.h>
#include <libdiskmgt.h>
#include "disks_private.h"


/*
 * Use the heuristics to check for a filesystem on the slice.
 */
int
inuse_fs(char *slice, nvlist_t *attrs, int *errp)
{
	int in_use = 0;
	struct DU_Info info;

	init_diskutil_info(&info);
	get_diskutil_info(slice, &info);

	if (diskutil_info_valid(&info)) {
		char *bundle_type = get_bundle_type(&info);

		if (bundle_type &&
		    strlen(bundle_type) &&
		    (strcmp(bundle_type, "zfs") != 0)) {
			
			if(strcmp(bundle_type, "apfs") == 0) {
				libdiskmgt_add_str(attrs, DM_USED_BY,
				    DM_USE_FS_NO_FORCE, errp);
			} else {
				libdiskmgt_add_str(attrs, DM_USED_BY,
				    DM_USE_FS, errp);
			}

			libdiskmgt_add_str(attrs, DM_USED_NAME,
			    bundle_type, errp);

			free(bundle_type);
			in_use = 1;			
		}
	}
	
	destroy_diskutil_info(&info);
	
	return (in_use);
}
