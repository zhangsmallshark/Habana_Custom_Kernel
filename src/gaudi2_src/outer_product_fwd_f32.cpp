/**********************************************************************
Copyright (c) 2020 Habana Labs.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

*   Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#include <vector>
#include <cstring>
#include <iostream>
#include "outer_product_fwd_f32.hpp"


extern unsigned char _binary___outer_product_fwd_f32_o_start;
extern unsigned char _binary___outer_product_fwd_f32_o_end;

 gcapi::GlueCodeReturn_t OuterProductFwdF32::GetKernelName(
             char kernelName [gcapi::MAX_NODE_NAME])
 {
     strcpy(kernelName,"custom_outer_product_fwd_f32");
     return gcapi::GLUE_SUCCESS;
 }


gcapi::GlueCodeReturn_t OuterProductFwdF32::GetGcDefinitions(
            gcapi::HabanaKernelParams_t* in_defs,
            gcapi::HabanaKernelInstantiation_t* out_defs)
{
    gcapi::GlueCodeReturn_t retVal;

    /*************************************************************************************
    *   Stage I - validate input
    **************************************************************************************/
    //validate correct amount of input tensors
    if (in_defs->inputTensorNr != 2)
    {
        in_defs->inputTensorNr  = 2;
        return gcapi::GLUE_INCOMPATIBLE_INPUT_COUNT;
    }
    //validate correct amount of output tensors
    if (in_defs->outputTensorNr !=1)
    {
        in_defs->outputTensorNr  = 1;
        return gcapi::GLUE_INCOMPATIBLE_OUTPUT_COUNT;
    }
    //validate matrix dimensions
    // if ((in_defs->inputTensors[0].geometry.dims != 2 &&
    //      in_defs->inputTensors[0].geometry.dims != 3) ||
    //     (in_defs->inputTensors[1].geometry.dims != 2 &&
    //      in_defs->inputTensors[1].geometry.dims != 3) ||
    //     in_defs->inputTensors[0].geometry.sizes[0] != in_defs->inputTensors[1].geometry.sizes[1])
    // {
    //     return gcapi::GLUE_INCOMPATIBLE_INPUT_SIZE;
    // }
    // if ((in_defs->outputTensors[0].geometry.dims != 2 &&
    //      in_defs->outputTensors[0].geometry.dims != 3) ||
    //     in_defs->outputTensors[0].geometry.sizes[0] != in_defs->inputTensors[1].geometry.sizes[0] ||
    //     in_defs->outputTensors[0].geometry.sizes[1] != in_defs->inputTensors[0].geometry.sizes[1])
    // {
    //     return gcapi::GLUE_INCOMPATIBLE_OUTPUT_SIZE;
    // }

    // validate input and output data type
    if (in_defs->inputTensors[0].dataType != gcapi::DATA_F32 ||
        in_defs->inputTensors[1].dataType != gcapi::DATA_F32 ||
        in_defs->outputTensors[0].dataType != gcapi::DATA_F32)
    {
        in_defs->inputTensors[0].dataType = gcapi::DATA_F32;
        in_defs->inputTensors[1].dataType = gcapi::DATA_F32;
        in_defs->outputTensors[0].dataType = gcapi::DATA_F32;
        return gcapi::GLUE_INCOMPATIBLE_DATA_TYPE;
    }

    /*************************************************************************************
    *    Stage II-IV -  Define index space geometry. In this example the index space matches
    *    the dimensions of the output tensor, up to dim 0.
    **************************************************************************************/
    // int32_t v_len = 64;
    out_defs->indexSpaceGeometry.dims = 4;
    out_defs->indexSpaceGeometry.sizes[0] = 1u;
    out_defs->indexSpaceGeometry.sizes[1] =
        std::max(in_defs->outputTensors[0].geometry.sizes[1], 1u);
    out_defs->indexSpaceGeometry.sizes[2] =
        std::max(in_defs->outputTensors[0].geometry.sizes[2], 1u);
    out_defs->indexSpaceGeometry.sizes[3] =
        std::max(in_defs->outputTensors[0].geometry.sizes[3], 1u);

    // Matrix C - Tensor Access Pattern
    out_defs->outputTensorAccessPattern[0].dim[0].dim     = 0;
    out_defs->outputTensorAccessPattern[0].dim[0].start_a = 0;
    out_defs->outputTensorAccessPattern[0].dim[0].end_a   = 0;
    out_defs->outputTensorAccessPattern[0].dim[0].start_b = 0;
    out_defs->outputTensorAccessPattern[0].dim[0].end_b   = 
        in_defs->outputTensors[0].geometry.sizes[1];

    // out_defs->outputTensorAccessPattern[0].dim[1].dim     = 1;
    // out_defs->outputTensorAccessPattern[0].dim[1].start_a = 0;
    // out_defs->outputTensorAccessPattern[0].dim[1].end_a   = 0;
    // out_defs->outputTensorAccessPattern[0].dim[1].start_b = 0;
    // out_defs->outputTensorAccessPattern[0].dim[1].end_b   = 
    //     in_defs->outputTensors[0].geometry.sizes[2];

    out_defs->outputTensorAccessPattern[0].dim[1].dim     = 1;
    out_defs->outputTensorAccessPattern[0].dim[1].start_a = 1;
    out_defs->outputTensorAccessPattern[0].dim[1].end_a   = 1;
    out_defs->outputTensorAccessPattern[0].dim[1].start_b = 0;
    out_defs->outputTensorAccessPattern[0].dim[1].end_b   = 0;

    out_defs->outputTensorAccessPattern[0].dim[2].dim     = 2;
    out_defs->outputTensorAccessPattern[0].dim[2].start_a = 1;
    out_defs->outputTensorAccessPattern[0].dim[2].end_a   = 1;
    out_defs->outputTensorAccessPattern[0].dim[2].start_b = 0;
    out_defs->outputTensorAccessPattern[0].dim[2].end_b   = 0;

    out_defs->outputTensorAccessPattern[0].dim[3].dim     = 3;
    out_defs->outputTensorAccessPattern[0].dim[3].start_a = 1;
    out_defs->outputTensorAccessPattern[0].dim[3].end_a   = 1;
    out_defs->outputTensorAccessPattern[0].dim[3].start_b = 0;
    out_defs->outputTensorAccessPattern[0].dim[3].end_b   = 0;

    // Matrix A - Tensor Access Pattern
    out_defs->inputTensorAccessPattern[0].dim[0].dim     = 0;
    out_defs->inputTensorAccessPattern[0].dim[0].start_a = 0;
    out_defs->inputTensorAccessPattern[0].dim[0].end_a   = 0;
    out_defs->inputTensorAccessPattern[0].dim[0].start_b = 0;
    out_defs->inputTensorAccessPattern[0].dim[0].end_b =
        in_defs->inputTensors[0].geometry.sizes[0] - 1;

    out_defs->inputTensorAccessPattern[0].dim[1].dim     = 1;
    out_defs->inputTensorAccessPattern[0].dim[1].start_a = 1;
    out_defs->inputTensorAccessPattern[0].dim[1].end_a   = 1;
    out_defs->inputTensorAccessPattern[0].dim[1].start_b = 0;
    out_defs->inputTensorAccessPattern[0].dim[1].end_b   = 0;

    out_defs->inputTensorAccessPattern[0].dim[2].dim     = 2;
    out_defs->inputTensorAccessPattern[0].dim[2].start_a = 1;
    out_defs->inputTensorAccessPattern[0].dim[2].end_a   = 1;
    out_defs->inputTensorAccessPattern[0].dim[2].start_b = 0;
    out_defs->inputTensorAccessPattern[0].dim[2].end_b   = 0;

    out_defs->inputTensorAccessPattern[0].dim[3].dim     = 3;
    out_defs->inputTensorAccessPattern[0].dim[3].start_a = 1;
    out_defs->inputTensorAccessPattern[0].dim[3].end_a   = 1;
    out_defs->inputTensorAccessPattern[0].dim[3].start_b = 0;
    out_defs->inputTensorAccessPattern[0].dim[3].end_b   = 0;

    // Matrix B - Tensor Access Pattern
    out_defs->inputTensorAccessPattern[1].dim[0].dim     = 0;
    out_defs->inputTensorAccessPattern[1].dim[0].start_a = 0;
    out_defs->inputTensorAccessPattern[1].dim[0].end_a   = 0;
    out_defs->inputTensorAccessPattern[1].dim[0].start_b = 0;
    out_defs->inputTensorAccessPattern[1].dim[0].end_b   = 
        in_defs->inputTensors[1].geometry.sizes[1] - 1;

    out_defs->inputTensorAccessPattern[1].dim[1].dim     = 1;
    out_defs->inputTensorAccessPattern[1].dim[1].start_a = 1;
    out_defs->inputTensorAccessPattern[1].dim[1].end_a   = 1;
    out_defs->inputTensorAccessPattern[1].dim[1].start_b = 0;
    out_defs->inputTensorAccessPattern[1].dim[1].end_b   = 0;

    out_defs->inputTensorAccessPattern[1].dim[2].dim     = 2;
    out_defs->inputTensorAccessPattern[1].dim[2].start_a = 1;
    out_defs->inputTensorAccessPattern[1].dim[2].end_a   = 1;
    out_defs->inputTensorAccessPattern[1].dim[2].start_b = 0;
    out_defs->inputTensorAccessPattern[1].dim[2].end_b   = 0;

    out_defs->inputTensorAccessPattern[1].dim[3].dim     = 3;
    out_defs->inputTensorAccessPattern[1].dim[3].start_a = 1;
    out_defs->inputTensorAccessPattern[1].dim[3].end_a   = 1;
    out_defs->inputTensorAccessPattern[1].dim[3].start_b = 0;
    out_defs->inputTensorAccessPattern[1].dim[3].end_b   = 0;

    /*************************************************************************************
    *    Stage V -  Load ISA into the descriptor.
    **************************************************************************************/
    unsigned IsaSize = (&_binary___outer_product_fwd_f32_o_end - &_binary___outer_product_fwd_f32_o_start);
    unsigned givenBinarySize = out_defs->elfSize;
    out_defs->elfSize = IsaSize;

    if (givenBinarySize >= IsaSize)
    {
        // copy binary out
        memcpy (out_defs->kernelElf,
                &_binary___outer_product_fwd_f32_o_start,
                IsaSize);
    }
    else
    {
       retVal = gcapi::GLUE_INSUFICIENT_ELF_BUFFER;
       return retVal;
    }

    return gcapi::GLUE_SUCCESS;
}

