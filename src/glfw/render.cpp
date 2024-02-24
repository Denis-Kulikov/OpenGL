#include <render.hpp>

Render::Render(const Pipeline &_pipeline)
{
    pipeline.camera.SetCamera(_pipeline.camera);
    pipeline.object.SetTransform(_pipeline.object);
    pipeline.m_transformation.SetMatrix4f(_pipeline.m_transformation);
}

void Render::clearRender()
{

}

void Render::drawObeject()
{

}
