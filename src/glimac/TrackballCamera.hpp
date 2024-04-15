#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class TrackballCamera {
    private:
        float m_Distance;
        float m_AngleX;
        float m_AngleY;
    
    public:
        TrackballCamera();

        void moveFront(float delta);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        glm::mat4 getViewMatrix() const;
};