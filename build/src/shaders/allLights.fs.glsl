#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextCoords;

out vec4 fFragColor;

uniform sampler2D uTexture;

uniform vec3      uKdCenter;
uniform vec3      uKsCenter;
uniform float     uShininessCenter;
uniform vec3      uLightPosCenter;
uniform vec3      uLightIntensityCenter;

uniform vec3      uKdSelf;
uniform vec3      uKsSelf;
uniform float     uShininessSelf;
uniform vec3      uLightPosSelf;
uniform vec3      uLightIntensitySelf;

vec3 blinnPhongPos(vec3 uKd, vec3 uKs, float uShininess, vec3 uLightPos, vec3 uLightIntensity);

void main() {
    // Calculating lighting contributions
    vec3 light = blinnPhongPos(uKdCenter, uKsCenter, uShininessCenter, uLightPosCenter, uLightIntensityCenter)
               + blinnPhongPos(uKdSelf, uKsSelf, uShininessSelf, uLightPosSelf, uLightIntensitySelf);

    // Combining lighting with texture color
    vec4 textureColor = texture(uTexture, vTextCoords);
    fFragColor = vec4(textureColor.xyz * 0.3 + light, textureColor.a);
}

vec3 blinnPhongPos(vec3 uKd, vec3 uKs, float uShininess, vec3 uLightPos, vec3 uLightIntensity) {
    vec3 normalNormalise = normalize(vNormal);
    vec3 lightPos = normalize(uLightPos - vPosition); // Calculating the direction to light
    float d = distance(uLightPos, vPosition);
    vec3 LightIntensity = uLightIntensity / (d * d); // Applying attenuation

    // Calculating halfway vector
    vec3 halfVector = normalize(lightPos + normalize(-vPosition));

    // Calculating diffuse and specular factors
    float diffuseFactor = max(dot(normalNormalise, lightPos), 0.f);
    float specularFactor = pow(max(dot(normalNormalise, halfVector), 0.f), uShininess);

    // Calculating diffuse and specular colors
    vec3 diffuseColor = uKd * diffuseFactor;
    vec3 specularColor = uKs * specularFactor;

    // Combining diffuse and specular colors with light intensity and returning
    return LightIntensity * (diffuseColor + specularColor);
}

// Trying to add a directional light
// vec3 blinnPhongDir() {
//     vec3 normalNormalise = normalize(vNormal);

//     // Normalizing light direction
//     vec3 lightDirection = normalize(uLightDir);

//     // Calculating halfway vector
//     vec3 halfVector = normalize(lightDirection + normalize(-vPosition));

//     // Calculating diffuse and specular factors
//     float diffuseFactor = max(dot(normalNormalise, lightDirection),0.f);
//     float specularFactor = pow(max(dot(normalNormalise, halfVector),0.f), uShininessDir);

//     // Calculating diffuse and specular colors
//     vec3 diffuseColor = uKdDir * diffuseFactor;
//     vec3 specularColor = uKsDir * specularFactor;

//     // Combining diffuse and specular colors and returning
//     //return uLightIntensity * (diffuseColor + specularColor);
//     return uLightIntensityDir * (diffuseColor + specularColor);
// }