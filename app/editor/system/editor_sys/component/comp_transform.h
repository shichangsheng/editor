#pragma once

#include "component.h"

class CompTransform : public Component {
public:
    CompTransform();
    ~CompTransform();
    virtual void OnAdd() override;
    virtual void OnDel() override;
    virtual void OnUpdate(float dt) override;

    void Position(float x, float y);
    void Scale(float x, float y);
    void Angle(float r);

    CompTransform & AddPosition(float x, float y);
    CompTransform & AddScale(float x, float y);
    CompTransform & AddAngle(float r);

    const glm::vec2 & GetPosition() const;
    const glm::vec2 & GetScale() const;
    float GetAngle() const;

    const glm::mat3 & GetMatrix();
    glm::mat3 GetMatrixFromRoot();
    glm::mat3 GetRotateFromRoot();
    glm::vec2 GetWorldPosition();
    glm::mat4 GetAngleMatrix();

    virtual const std::string & GetName() override;

    virtual bool OnModifyProperty(
        const std::any &    value, 
        const std::string & title, 
        const std::any &    backup) override;

    virtual bool ParseProperty(
        const std::string & key, 
        const std::string & val) override;

    virtual std::vector<Property> CollectProperty() override;

private:
    void UpdateMatrix();

private:
    float     _angle;
    glm::vec2 _scale;
    glm::mat3 _matrix;
    glm::vec2 _position;
    bool _isChange;
};