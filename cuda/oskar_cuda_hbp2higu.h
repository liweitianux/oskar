/*
 * Copyright (c) 2011, The University of Oxford
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

#ifndef OSKAR_CUDA_HBP2HIGU_H_
#define OSKAR_CUDA_HBP2HIGU_H_

/**
 * @file oskar_cuda_bp2hig.h
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Computes a hierarchical (tile/station) beam pattern using CUDA in the
 * horizontal coordinate system.
 *
 * @details
 * Computes a beam pattern using CUDA, assuming isotropic antennas,
 * generating the geometric beamforming weights separately.
 * The beamforming weights are NOT normalised to the number of antennas.
 *
 * The function must be supplied with the antenna x- and y-positions, the
 * test source longitude and latitude positions, the beam direction, and
 * the wavenumber.
 *
 * The \p x1 and \p x2 are effectively two-dimensional arrays of \p n2 by \p n1
 * elements, with the length of the fastest-varying dimension given by the
 * value of n1.
 *
 * The computed beam pattern is returned in the \p image array, which
 * must be pre-sized to length 2*ns. The values in the \p image array
 * are alternate (real, imag) pairs for each position of the test source.
 *
 * @param[in] n2 The number of tiles.
 * @param[in] n1 An array containing the number of antennas in each tile.
 * @param[in] x1 The antenna x-positions for each tile in metres, relative to tile centres.
 * @param[in] y1 The antenna y-positions for each tile in metres, relative to tile centres.
 * @param[in] x2 The tile x-positions for each tile in metres, relative to station centre.
 * @param[in] y2 The tile y-positions for each tile in metres, relative to station centre.
 * @param[in] ns The number of test source positions.
 * @param[in] sa The azimuth coordinates of the test source in radians.
 * @param[in] se The elevation coordinates of the test source in radians.
 * @param[in] ba1 The tile beam azimuth direction in radians
 * @param[in] be1 The tile beam elevation direction in radians.
 * @param[in] ba2 The station beam azimuth direction in radians
 * @param[in] be2 The station beam elevation direction in radians.
 * @param[in] k The wavenumber (rad / m).
 * @param[out] image The computed beam pattern (see note, above).
 */
void oskar_cuda_hbp2higu(int n2, int* n1, const float* x1, const float* y1,
        const float* x2, const float* y2, int ns, const float* sa,
        const float* se, float ba1, float be1, float ba2, float be2,
        float k, float* image);

#ifdef __cplusplus
}
#endif

#endif // OSKAR_CUDA_HBP2HIGU_H_
