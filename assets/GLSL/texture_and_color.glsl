#type vertex
#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texcoord;

out vec2 v_texcoord;
out vec2 v_screenpos;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

void main()
{
	v_texcoord = a_texcoord;
	gl_Position = u_view_projection * u_transform * vec4(a_pos, 1.0);
	v_screenpos = gl_Position.xy;
}

#type fragment
#version 330 core

in vec2 v_texcoord;
in vec2 v_screenpos;

out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_tiling_factor;

void main()
{
	float dist = 1.0f - distance(v_screenpos * 0.8f, vec2(0.0f));
	dist = clamp(dist, 0.0f, 1.0f);
	dist = sqrt(dist);
        color = texture(u_texture, v_texcoord * u_tiling_factor) * u_color;
}

