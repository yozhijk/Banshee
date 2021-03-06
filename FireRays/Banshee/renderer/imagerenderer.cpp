#include "imagerenderer.h"

#include "../world/world.h"
#include "../imageplane/imageplane.h"
#include "../util/progressreporter.h"

#include <cassert>

void ImageRenderer::Render(World const& world) const
{
    int2 imgres = imgplane_.resolution();

    // Get camera 
    Camera const& cam(*world.camera_.get());

    // Prepare image plane
    imgplane_.Prepare();

    // Calculate total number of samples for progress reporting
    int totalsamples = imgsampler_->num_samples() * imgres.y * imgres.x;
    int donesamples = 0;

    // very simple render loop
    for (int y = 0; y < imgres.y; ++y)
        for(int x = 0; x < imgres.x; ++x)
        {
            ray r;

            for (int s = 0; s < imgsampler_->num_samples(); ++s)
            {
                // Generate sample
                float2 sample = imgsampler_->Sample2D();

                // Calculate image plane sample
                float2 imgsample((float)x / imgres.x + (1.f / imgres.x) * sample.x, (float)y / imgres.y + (1.f / imgres.y) * sample.y);

                // Generate ray
                cam.GenerateRay(imgsample, r);

                // Estimate radiance and add to image plane
                imgplane_.AddSample(int2(x,y), tracer_->GetLi(r, world, *lightsampler_, *brdfsampler_));
            }

			imgsampler_->Reset();
			lightsampler_->Reset();
			brdfsampler_->Reset();

            // Update progress
            donesamples += imgsampler_->num_samples();
            // Report progress
            if (progress_)
            {
                progress_->Report((float)donesamples/totalsamples);
            }
        }

    // Finalize image plane
    imgplane_.Finalize();
}

void ImageRenderer::RenderTile(World const& world, int2 const& start, int2 const& dim) const
{
    int2 imgres = imgplane_.resolution();

    // Get camera 
    Camera const& cam(*world.camera_.get());

    // Calculate total number of samples for progress reporting
    int totalsamples = imgsampler_->num_samples() * dim.y * dim.x;
    int donesamples = 0;

    // very simple render loop
    for (int y = start.y; y < start.y + dim.y; ++y)
        for(int x = start.x; x < start.x + dim.x; ++x)
        {
            ray r;
            
            for (int s = 0; s < imgsampler_->num_samples(); ++s)
            {
                // Generate sample
                float2 sample = imgsampler_->Sample2D();

                // Calculate image plane sample
                float2 imgsample((float)x / imgres.x + (1.f / imgres.x) * sample.x, (float)y / imgres.y + (1.f / imgres.y) * sample.y);

                // Generate ray
                cam.GenerateRay(imgsample, r);

                // Estimate radiance and add to image plane
                imgplane_.AddSample(int2(x,y), tracer_->GetLi(r, world, *lightsampler_, *brdfsampler_));
            }

            // Update progress
            donesamples += imgsampler_->num_samples();
            // Report progress
            if (progress_)
            {
                progress_->Report((float)donesamples/totalsamples);
            }
        }
}
