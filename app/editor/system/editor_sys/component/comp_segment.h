#pragma once

#include "component.h"
#include "../../interface/render.h"

class CompSegment : public Component {
public:
    CompSegment();
    virtual void OnUpdate(UIObjectGLCanvas * canvas, float dt) override;

    virtual const std::string & GetName() override;
    virtual void EncodeBinary(std::ofstream & os) override;
    virtual void DecodeBinary(std::ifstream & is) override;
    virtual bool OnModifyProperty(const std::any & oldValue, 
                                  const std::any & newValue, 
                                  const std::string & title) override;

protected:
    virtual std::vector<Property> CollectProperty() override;

private:
    void Update();
    void GenMesh(const std::vector<glm::vec2> & segments);
    void GenSegm(const std::vector<glm::vec2> & segments, std::vector<glm::vec2> & output);
    virtual void OnModifyTrackPoint(const size_t index, const glm::vec2 & point) override;
    virtual void OnInsertTrackPoint(const size_t index, const glm::vec2 & point) override;
    virtual void OnDeleteTrackPoint(const size_t index, const glm::vec2 & point) override;

private:
    float               _smooth;
    float               _width;
    glm::vec4           _color;

    SharePtr<GLMesh>    _mesh;
    SharePtr<GLProgram> _program;
};