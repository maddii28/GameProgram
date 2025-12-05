#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float strength;   // 0.0 → normal, 1.0 → effect
uniform float time;       // for pulsing

out vec4 finalColor;

void main() {
    vec4 tex = texture(texture0, fragTexCoord);

    // Red pulse effect
    float pulse = abs(sin(time * 4.0)) * strength;

    vec3 redTint = mix(tex.rgb, vec3(1.0, 0.1, 0.1), pulse);

    finalColor = vec4(redTint, tex.a);
}
