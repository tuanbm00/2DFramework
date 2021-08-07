precision mediump float;

uniform samplerCube u_samplerCubeMap;
uniform float fog_start;
uniform float fog_length;
uniform vec3 fog_color;

varying vec3 v_pos;
varying float dis;

void main()
{
//	gl_FragColor = vec4(v_color);
	float fogValue = clamp((dis - fog_start) / (fog_length * 20.0), 0.0, 1.0);
	vec4 textColor = textureCube(u_samplerCubeMap, v_pos);
	gl_FragColor = fogValue*vec4(fog_color, 1.0) + (1.0-fogValue)*textColor;

}
