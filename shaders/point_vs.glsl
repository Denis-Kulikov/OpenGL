#version 330

layout (location = 0) in vec3 Position;

uniform mat4 gWorld;
uniform vec3 FragPos;
uniform float Cluster;
out vec4 Color;

void main()
{
    gl_Position = gWorld * vec4(Position, 1.0);
    if (Cluster == 0) {
        Color = vec4(FragPos.z / 4, 1 - FragPos.z / 4, 0, 1.0);
    } else {
        Color = vec4(Cluster == 1, Cluster == 2, Cluster == 3, 1.0);
        // switch (Cluster) {
        // case 1:
        //     Color = vec4(1, 0, 0, 1.0);
        //     breck;
        // case 2:
        //     Color = vec4(0, 1, 0, 1.0);
        //     breck;
        // case 3:
        //     Color = vec4(0, 0, 1, 1.0);
        //     breck;
        // default:
        //     breck;
        // }
    }
}
