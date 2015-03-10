/*
 * Copyright (c) 2015, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <private_vis_header.h>
#include <oskar_vis_header.h>
#include <oskar_binary_read_mem.h>

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

oskar_VisHeader* oskar_vis_header_read(oskar_Binary* h, int* status)
{
    /* Visibility metadata. */
    int num_channels = 0, num_times_total = 0, num_stations = 0, tag_error = 0;
    int amp_type = 0, max_times_per_block = 0, write_autocorr = 0;
    unsigned char grp = OSKAR_TAG_GROUP_VIS_HEADER;
    oskar_VisHeader* vis = 0;

    /* Check all inputs. */
    if (!h || !status)
    {
        oskar_set_invalid_argument(status);
        return 0;
    }

    /* Check if safe to proceed. */
    if (*status) return 0;

    /* Read essential metadata. */
    oskar_binary_read_int(h, grp, OSKAR_VIS_HEADER_TAG_WRITE_AUTOCORRELATIONS, 0,
            &write_autocorr, status);
    oskar_binary_read_int(h, grp, OSKAR_VIS_HEADER_TAG_AMP_TYPE, 0,
            &amp_type, status);
    oskar_binary_read_int(h, grp, OSKAR_VIS_HEADER_TAG_MAX_TIMES_PER_BLOCK, 0,
            &max_times_per_block, status);
    oskar_binary_read_int(h, grp, OSKAR_VIS_HEADER_TAG_NUM_TIMES_TOTAL, 0,
            &num_times_total, status);
    oskar_binary_read_int(h, grp, OSKAR_VIS_HEADER_TAG_NUM_CHANNELS, 0,
            &num_channels, status);
    oskar_binary_read_int(h, grp, OSKAR_VIS_HEADER_TAG_NUM_STATIONS, 0,
            &num_stations, status);

    /* Check if safe to proceed. */
    if (*status) return 0;

    /* Create the visibility header. */
    vis = oskar_vis_header_create(amp_type, max_times_per_block,
            num_times_total, num_channels, num_stations, write_autocorr,
            status);

    /* Optionally read the settings data (ignore the error code). */
    tag_error = 0;
    oskar_binary_read_mem(h, vis->settings,
            OSKAR_TAG_GROUP_SETTINGS, OSKAR_TAG_SETTINGS, 0, &tag_error);

    /* Read the telescope model path. */
    oskar_binary_read_mem(h, vis->telescope_path,
            grp, OSKAR_VIS_HEADER_TAG_TELESCOPE_PATH, 0, status);

    /* Read other visibility metadata. */
    oskar_binary_read_double(h, grp, OSKAR_VIS_HEADER_TAG_FREQ_START_HZ, 0,
            &vis->freq_start_hz, status);
    oskar_binary_read_double(h, grp, OSKAR_VIS_HEADER_TAG_FREQ_INC_HZ, 0,
            &vis->freq_inc_hz, status);
    oskar_binary_read_double(h, grp,
            OSKAR_VIS_HEADER_TAG_CHANNEL_BANDWIDTH_HZ, 0,
            &vis->channel_bandwidth_hz, status);
    oskar_binary_read_double(h, grp, OSKAR_VIS_HEADER_TAG_TIME_START_MJD_UTC, 0,
            &vis->time_start_mjd_utc, status);
    oskar_binary_read_double(h, grp, OSKAR_VIS_HEADER_TAG_TIME_INC_SEC, 0,
            &vis->time_inc_sec, status);
    oskar_binary_read_double(h, grp,
            OSKAR_VIS_HEADER_TAG_TIME_AVERAGE_SEC, 0,
            &vis->time_average_sec, status);
    oskar_binary_read(h, OSKAR_DOUBLE, grp,
            OSKAR_VIS_HEADER_TAG_PHASE_CENTRE, 0,
            2 * sizeof(double), &vis->phase_centre, status);
    oskar_binary_read(h, OSKAR_DOUBLE, grp,
            OSKAR_VIS_HEADER_TAG_TELESCOPE_CENTRE, 0,
            3 * sizeof(double), vis->telescope_centre, status);

    /* Read the station coordinates. */
    oskar_binary_read_mem(h, vis->station_x_offset_ecef_metres,
            grp, OSKAR_VIS_HEADER_TAG_STATION_X_OFFSET_ECEF, 0, status);
    oskar_binary_read_mem(h, vis->station_y_offset_ecef_metres,
            grp, OSKAR_VIS_HEADER_TAG_STATION_Y_OFFSET_ECEF, 0, status);
    oskar_binary_read_mem(h, vis->station_z_offset_ecef_metres,
            grp, OSKAR_VIS_HEADER_TAG_STATION_Z_OFFSET_ECEF, 0, status);

    /* Return a handle to the new structure. */
    return vis;
}

#ifdef __cplusplus
}
#endif
