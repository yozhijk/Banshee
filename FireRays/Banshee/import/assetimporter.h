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
#ifndef ASSETIMPORTER_H
#define ASSETIMPORTER_H

#include <functional>

class Primitive;
class Light;
class Camera;
class Material;
class TextureSystem;

///< AssetImporter defines a callback interface for various 
///< asset file loaders. Set appropriate callbacks and call
///< Import() method to start import process
///<
class AssetImporter
{
public:
    AssetImporter(TextureSystem const& texsys)
        : onprimitive_(nullptr)
        , onlight_(nullptr)
        , oncamera_(nullptr)
        , onmaterial_(nullptr)
        , texsys_(texsys)
    {
    }

    // Destructor
    virtual ~AssetImporter(){}
    // Import asset
    virtual void Import() = 0;

    // New primitive callback
    std::function<void (Primitive*)> onprimitive_;
    // New light callback
    std::function<void (Light*)> onlight_;
    // New camera callback
    std::function<void (Camera*)> oncamera_;
    // New material callback
    std::function<int (Material*)> onmaterial_;

private:
    AssetImporter(AssetImporter const&);
    AssetImporter& operator = (AssetImporter const&);

protected:
    TextureSystem const& texsys_;
};


#endif //ASSETIMPORTER_H