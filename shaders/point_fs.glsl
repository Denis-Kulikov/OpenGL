#version 330

in vec3 Normal;

out vec4 Color;
out vec4 FragColor;

uniform vec3 FragPos;
uniform vec3 lightPos; 

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

void main()
{
    // Вычисляем вектор света
    vec3 lightDir = normalize(lightPos - FragPos);

    // Вычисляем диффузную составляющую
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * materialDiffuse;

    // Вычисляем зеркальную составляющую
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, viewDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = materialSpecular * spec;

    Color = vec4(FragPos.z / 4, 1 - FragPos.z / 4, 0, 1.0);

    // Смешиваем диффузную и зеркальную составляющие
    vec3 result = materialAmbient + diffuse + specular;

    FragColor = vec4(result, 1.0) * Color; // абсолютчно белые сферы
    // FragColor = Color; // раскрашение сферы без освещения 
    // FragColor = vec4(result * Color.rgb, 1.0); // жёлтые сферы (1, 1, 0, 1) почему-то
    // FragColor = vec4(specular * Color.rgb, 1.0); // сферы становятся чёрными
}
