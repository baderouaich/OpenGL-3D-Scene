#version 460

struct Material
{
//Colors
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
//textures id's
    sampler2D diffuse_texture; //real color data
    sampler2D specular_texture;//real color data
};

/*Struct for Light.h class PointLight data*/
struct PointLight
{
    float intensity;
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

//we expecting these outputs from vertex shader
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;


///=== Uniforms (from cpp)===///
uniform Material material;
uniform PointLight pointLight;
//Lights
uniform vec3 camera_position;


//Functions: SEE https://learnopengl.com/Lighting/Basic-Lighting
vec3 calculateAmbient(Material material)
{
    return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPosition)
{
    vec3 posToLightDirVec = normalize(lightPosition - vs_position);
    float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
    vec3 diffuseFinal = material.diffuse * diffuse;

    return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPosition, vec3 camera_position)
{
    vec3 lightToPosDirVec = normalize(vs_position - lightPosition);
    vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(camera_position - vs_position);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
    //uncoment to use specular lighting based on texture specular (to look more realistic)
    //vec3 specularFinal = material.specular * specularConstant * texture(material.specular_texture, vs_texcoord).rgb;
    vec3 specularFinal = material.specular * specularConstant;

    return specularFinal;
}


// Final pixel color
out vec4 final_pixel_color;

void main()
{
    //final pixel color (TODO: enable to see texture skin on mesh)
    //final_pixel_color = vec4(vs_color, 1.0f);
    //final_pixel_color = texture(texture0, vs_texcoord) + texture(texture1, vs_texcoord); // * vec4(vs_color, 1.0f);


    //Ambiant Light
    vec3 ambientFinal = calculateAmbient(material); // ambient light color

    //Diffuse Light
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position);

    //Specular Light
    vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, camera_position);


    //Attenuation Light (Limiting light radius of light)
    //distance 
    float dist = length(pointLight.position - vs_position); //calculate length between light and vertex positions
    //constant linear quadratic
    float attenuation = pointLight.constant / (1.0f + pointLight.linear * dist + pointLight.quadratic * (dist * dist));

    //Final Light
    ambientFinal *= attenuation;
    diffuseFinal *= attenuation;
    specularFinal *= attenuation;

    const bool TEXTURE_SKIN = true;
    if(TEXTURE_SKIN)
    {
        final_pixel_color = texture(material.diffuse_texture, vs_texcoord) *
        //vec4(vs_color, 1.0f) *
        (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
    }
    else {
        final_pixel_color = (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
    }

}