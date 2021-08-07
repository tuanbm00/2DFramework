precision mediump float;

uniform sampler2D u_texture; //fire
uniform sampler2D r_texture; // displacement
uniform sampler2D g_texture; //firemask

uniform float u_time;
uniform float fog_start;
uniform float fog_length;
uniform vec3 fog_color;

varying vec2 v_uv;
varying float dis;

void main()
{
	float fogValue = clamp((dis - fog_start) / fog_length, 0.0, 1.0);
	vec2 disp = texture2D(r_texture, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * 0.1;
	vec2 displaced = v_uv + offset;
	vec4 fireColor = texture2D(u_texture, displaced);
	vec4 AlphaValue = texture2D(g_texture, v_uv);
	vec4 textColor = fireColor *(1.0, 1.0, 1.0, AlphaValue.r);
	gl_FragColor = fogValue*vec4(fog_color, 1.0) + (1.0-fogValue)*textColor;
}
