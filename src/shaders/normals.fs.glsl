#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextCoords;

out vec3 fFragColor;

void main() {
    fFragColor = normalize(vNormal);
    // fFragColor = vec3(1,0.,0.);
}
