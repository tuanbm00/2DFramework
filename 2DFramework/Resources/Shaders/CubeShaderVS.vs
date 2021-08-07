attribute vec3 a_posL;

uniform mat4 u_mvpMatrix;

varying vec3 v_pos;
varying float dis;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = u_mvpMatrix*posL;
dis = length(gl_Position.xyz);
v_pos = a_posL;
}
   