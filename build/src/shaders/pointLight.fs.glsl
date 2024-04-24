#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextCoords;

out vec4 fFragColor;

uniform sampler2D uTexture;

uniform vec3      uKd;
uniform vec3      uKs;
uniform float     uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong();

void main() {
    // fFragColor = vec4(texture(uTexture, vTextCoords).wxy * blinnPhong(),1.f);
    vec3 bp = blinnPhong();
    fFragColor = vec4(texture(uTexture, vTextCoords)[0] * bp[0], texture(uTexture, vTextCoords)[1] * bp[1], texture(uTexture, vTextCoords)[2] * bp[2], texture(uTexture, vTextCoords)[3]);
    // fFragColor = blinnPhong();
}

vec3 blinnPhong() {
    //return uLightIntensity *( uKd * max( dot( normalize(vNormal) , normalize(uLightDir_vs) ), 0.f) + uKs * pow( max( dot( (normalize(uLightDir_vs) + normalize(-vPosition))/2 , normalize(vNormal) ), 0.f) , uShininess)  );
    vec3 normalNormalise = normalize(vNormal);
    float d = distance(uLightPos_vs,vPosition);
    vec3 LightIntensity = uLightIntensity / (d * d);

    vec3 lightDir = normalize(uLightPos_vs - vPosition); // Direction incidente de la lumiÃ¨re
    vec3 halfVector = normalize(lightDir + normalize(-vPosition)); // Vecteur halfVector
    float diffuseFactor = max(dot(normalNormalise, lightDir), 0.0); // Produit scalaire entre la normale et la direction incidente
    float specularFactor = pow(max(dot(normalNormalise, halfVector), 0.0), uShininess); // Produit scalaire entre le vecteur halfVector et la normale
    vec3 diffuseColor = uKd * diffuseFactor; // Calcul de la couleur diffuse
    vec3 specularColor = uKs * specularFactor; // Calcul de la couleur glossy    
    return uLightIntensity / (distance(uLightPos_vs,vPosition) * distance(uLightPos_vs,vPosition)) * (diffuseColor + specularColor); // Couleur finale en combinant diffuse et glossy
}