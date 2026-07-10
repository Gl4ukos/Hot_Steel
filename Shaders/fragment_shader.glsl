#version 330 core
out vec4 frag_colour;

in vec3 outColor;
in vec2 TexCoord;  

uniform vec4 another_colour;
uniform sampler2D tex;
uniform int use_texture;
uniform vec2 uvScale;

uniform float fragment_opacity;

void main(){
    vec4 final_colour;

    if(use_texture == 1)
    {
        final_colour = texture(tex, TexCoord * uvScale);
    }
    else
    {
        final_colour = vec4(outColor, 1.0);
    }

    // tint RGB only
    final_colour.rgb =
        (another_colour.rgb * another_colour.a) +
        (final_colour.rgb * (1.0 - another_colour.a));

    // preserve alpha
    frag_colour = final_colour;

    // apply fade
    frag_colour.a *= fragment_opacity;
}