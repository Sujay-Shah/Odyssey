#version 330 core 

layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormals;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 offsetpos;


uniform mat4 projection_Matrix;
uniform mat4 view_Matrix;
uniform mat4 model_Matrix;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normals;


void main()
{
    Normals     = mat3(transpose(inverse(model_Matrix))) * anormals;
    
    TexCoords   = aTexCoord;

    FragPos = vec3(model_Matrix * vec4(apos,1.0f));

    gl_Position = projection_Matrix * view_Matrix *model_Matrix * vec4(apos,1.0f);

}