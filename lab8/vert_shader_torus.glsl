#version 130

in vec3 vp;

void main () {
	gl_Position = gl_ModelViewProjectionMatrix * vec4 (vp, 1.0);
}
