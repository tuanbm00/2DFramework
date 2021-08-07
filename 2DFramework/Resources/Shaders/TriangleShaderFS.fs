precision mediump float;

uniform sampler2D u_texture;

varying vec2 v_uv;

void main()
{
//	gl_FragColor = vec4(v_color);
//	float fogValue = clamp((dis - fog_start) / fog_length, 0.0, 1.0);
	gl_FragColor = texture2D(u_texture, v_uv);
//	gl_FragColor = fogValue*vec4(fog_color, 1.0) + (1.0-fogValue)*textColor;
}
