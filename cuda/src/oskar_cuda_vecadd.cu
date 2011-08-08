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

#include "cuda/oskar_cuda_vecadd.h"
#include "math/cudak/oskar_cudak_vec_add_rr.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void oskar_cudaf_vecadd(int n, const float* a, const float* b, float* c)
{
    // Allocate vectors in device memory.
    float *da, *db, *dc;
    cudaMalloc((void**)&da, n * sizeof(float));
    cudaMalloc((void**)&db, n * sizeof(float));
    cudaMalloc((void**)&dc, n * sizeof(float));

    // Copy input vectors from host memory to device memory.
    cudaMemcpy(da, a, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(db, b, n * sizeof(float), cudaMemcpyHostToDevice);

    // Invoke kernel.
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    oskar_cudak_vec_add_rr_f <<<blocksPerGrid, threadsPerBlock>>>
            (n, da, db, dc);
    cudaThreadSynchronize();
    cudaError_t err = cudaPeekAtLastError();
    if (err != cudaSuccess)
        printf("CUDA Error: %s\n", cudaGetErrorString(err));

    // Copy result from device memory to host memory.
    cudaMemcpy(c, dc, n * sizeof(float), cudaMemcpyDeviceToHost);

    // Free device memory.
    if (da) cudaFree(da);
    if (db) cudaFree(db);
    if (dc) cudaFree(dc);
}

#ifdef __cplusplus
}
#endif
