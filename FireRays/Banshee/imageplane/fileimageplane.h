/*
    Banshee and all code, documentation, and other materials contained
    therein are:

        Copyright 2013 Dmitry Kozlov
        All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the software's owners nor the names of its
        contributors may be used to endorse or promote products derived from
        this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    (This is the Modified BSD License)
*/
#ifndef FILEIMAGEPLANE_H
#define FILEIMAGEPLANE_H

#include <string>
#include <vector>

#include "../imageio/imageio.h"
#include "imageplane.h"

///< File image plane is designed to 
///< collect rendering results and output
///< those into an image file
///<
class FileImagePlane : public ImagePlane
{
public:
    FileImagePlane(std::string filename, int2 res, ImageIo& io)
        : filename_(filename)
        , res_(res)
        , io_(io)
        , imgbuf_(res.x * res.y)
    {
    }

    // This method is called by the renderer prior to adding samples
    void Prepare();

    // This method is called by the renderer after adding all the samples
    void Finalize();

    // Add color contribution to the image plane
    void AddSample(float2 const& sample, float w, float3 value);

    // This is used by the renderer to decide on the number of samples needed
    int2 resolution() const { return res_; }

    // File name to write to
    std::string filename_;
    // Image resolution
    int2 res_;
    // IO object
    ImageIo& io_;
    // Intermediate image buffer
    std::vector<float3> imgbuf_;
};


#endif // FILEIMAGEPLANE_H