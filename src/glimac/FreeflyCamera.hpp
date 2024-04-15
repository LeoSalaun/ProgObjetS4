#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "glm/fwd.hpp"

class FreeflyCamera {
    private:
        glm::vec3 m_Position;
        float m_Phi;
        float m_Theta;

        glm::vec3 m_FrontVector;
        glm::vec3 m_LeftVector;
        glm::vec3 m_UpVector;

        void computeDirectionVectors();
    
    public:
        FreeflyCamera();

        void moveFront(float t);
        void moveLeft(float t);

        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        glm::mat4 getViewMatrix() const;
};