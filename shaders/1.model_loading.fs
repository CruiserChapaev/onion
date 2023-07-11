#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 sourceLightPos;

void main()
{    
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(sourceLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    FragColor = texture(texture_diffuse1, TexCoords);
    vec3 result = vec3(diffuse.x * FragColor.x, diffuse.y * FragColor.y, diffuse.z * FragColor.z);
    FragColor = vec4(result, 1.0);
}